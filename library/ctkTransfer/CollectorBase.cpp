#include "CollectorBase.h"
#include "pathbuilder.h"
#include "ctkPublicFun.h"
#include "curlftp.h"
#include "DataBase.h"
#include <QUrl>
#include <QRegExp>
#include <QFileInfoList>
#include <QStringList>
#include <QMutexLocker>
#include <qthread.h>
#include <lastCollectTime.h>
#include <algorithm>
#include <QtConcurrent/QtConcurrent>
#include <QSharedMemory>

// 初始化静态成员（自定义类对象，该类必须有默认构造函数）
TransCollectTimeList CollectorBase::m_lstTCtime;
bool CollectorBase::m_bReadTime = false;
QMutex CollectorBase::m_mutexTime;
//QString CollectorBase::m_strTctimePath;
QString CollectorBase::m_strSttimePath;

CollectorBase::CollectorBase(QWaitCondition &in_oCond, QMutex &in_oLocker, int &in_iLogsize, QObject *pParent)
    : m_oCond(in_oCond),
      m_oLocker(in_oLocker),
      m_iLogsize(in_iLogsize),
      m_oRcfClient(RCF::TcpEndpoint(50001)),
      QObject(pParent)		//从QObject继承的类需要写pParent!!
{
    m_bFinish = true;

    m_pLastTime = NULL;
    m_bRun = false;
    m_nLineState = 0;

    //m_oThread.start();
    //this->moveToThread(&m_oThread);
    connect(this, SIGNAL(begin()), this, SLOT(onBegined()));

    m_oId = QUuid::createUuid();
}

CollectorBase::~CollectorBase()
{
    //m_oThread.requestInterruption();
    //m_oThread.quit();
    //m_oThread.wait();
    //m_oThread.terminate();
    emit showIdentify(m_oId.toString());

}

void CollectorBase::init()
{
    curl_global_init(CURL_GLOBAL_ALL);
}

void CollectorBase::unit()
{
    curl_global_cleanup();
}

bool CollectorBase::filterFileName(const QFileInfo &fi, const QString &strFilePath, FileInfo &fileInfo)
{
    return true;

}

bool CollectorBase::filterFileName(FileInfo &fileInfo)
{
    QRegExp reg;
    reg.setPattern(m_collectSet.fileTemplate);
    if (reg.exactMatch(QString::fromStdString(fileInfo.strFileName)))
    {
        return true;
    }

    return false;
}

bool CollectorBase::filterFileName(const QString &fileName)
{
    QRegExp reg;
    reg.setPattern(m_collectSet.fileTemplate);
    if (reg.exactMatch(fileName))
    {
        return true;
    }

    return false;
}

bool CollectorBase::IsNewFile(FileInfo &fileInfo)
{
    return true;
}

bool CollectorBase::ReadLastTime()
{
    return true;
}


void CollectorBase::showMsg(const QString &str)
{
    emit print(QString("[%1]: %2").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(str));
}

void CollectorBase::emitLog(const QString &info, int infoType)
{
    //m_oLocker.lock();

    //if (m_iLogsize > 100)
    //{
    //	m_oCond.wait(&m_oLocker);
    //}
    //m_oLocker.unlock();
    //m_iLogsize++;
    //emit showLog(m_collectSet, info, infoType);


    try
    {
        m_oRcfClient.print(m_collectSet.dirName.toLocal8Bit().toStdString(),
                           m_collectSet.dirID.toLocal8Bit().toStdString(),
                           info.toLocal8Bit().toStdString(), infoType);
    }
    catch (std::exception &ex)
    {
        QSLOG_ERROR(QString("rcf exception:").arg(ex.what()));
    }



    // 通过发送消息的方式通知控制进程
    // QThread::msleep(20);
}

QString CollectorBase::getDestFilePath(const QString &srcFileFullPath, const QString &fileName, const CollectUser &cUser, const QDateTime &dt, bool enableTimeRule)
{
    QString destPath = cUser.user.rootPath + cUser.rltvPath;
    if (destPath.right(1) != "/")
    {
        destPath += "/";
    }
    else if (destPath == "//")
    {
        destPath = "/";
    }
    if (!cUser.user.keepDir)	// 不保持原目录结构
    {
        //return destPath;
    }
    else			// 保持原目录结构
    {
        // 从原始路径提取相对路径
        int srcRlvtPathLen = m_collectSet.rltvPath.length();
        if (m_collectSet.rltvPath.right(1) != "/")
        {
            srcRlvtPathLen += 1;
        }
        int subLen = srcFileFullPath.length() - srcRlvtPathLen - fileName.length() - 1;
        QString subPath("");
        if (subLen > 0)
        {
            subPath = srcFileFullPath.mid(srcRlvtPathLen, subLen);
        }
        destPath += subPath;
    }

    if (enableTimeRule)
    {
        destPath = CPathBuilder::getFinalPathFromUrl(destPath, dt).at(0);
    }

    if (destPath.right(1) != "/")
    {
        destPath += "/";
    }

    return destPath;
}

void CollectorBase::doWork()
{
    if (m_bFinish)
    {
        emit begin();
    }
}

void CollectorBase::onBegined()
{
    // 将该函数丢到子线程中执行，防止阻塞主线程
    QtConcurrent::run(QThreadPool::globalInstance(), this, &CollectorBase::getNewFiles);
    //getNewFiles();
}

bool CollectorBase::testFileConnection(QString strUrl)
{
    if (strUrl.size() < 2)
    {
        return false;
    }

    QUrl url = QUrl::fromLocalFile(strUrl);
    if (url.isLocalFile())
    {
        QDir qdir(strUrl);
        if (qdir.exists())
        {
            return true;
        }
    }

    return false;
}

bool CollectorBase::testFtpConnection(const QString & ip, int port, const QString & lgUser, const QString & lgPass, int transfermode, int connectmode)
{
    CurlFtp cFtp(this);
    // char url[512] = {0};
    QString url = QString("ftp://%1:%2").arg(ip).arg(port);
    //char usrPwd[100] = {0};
    QString usrPwd = QString("%1:%2").arg(lgUser).arg(lgPass);
    //sprintf(url, "ftp://%s:%d", ip.toStdString().c_str(), port);
    // sprintf(url, "sftp://%s", ip.toStdString().c_str(), port);
    //sprintf(usrPwd, "%s:%s", lgUser.toStdString().c_str(), lgPass.toStdString().c_str());
    cFtp.setFtpTransferMode(m_collectSet.ftp_transferMode);
    cFtp.setFtpConnectMode(m_collectSet.ftp_connectMode);

    return cFtp.connectToHost(url.toLocal8Bit().toStdString().c_str(), usrPwd.toLocal8Bit().toStdString().c_str());
}

bool CollectorBase::readSet()
{
    // 从数据库读取收集设置
    if (!DataBase::getInstance()->QueryCollectTask(m_collectSet))
    {
        QSLOG_ERROR("Fail to QueryCollectTask.");
        return false;
    }
    m_tUser.taskID = m_collectSet.dirID;
    m_tUser.colTaskInfo = m_collectSet;
    // 读取分发用户设置
    if (!DataBase::getInstance()->QueryUserInfo(m_tUser))
    {
        QSLOG_ERROR("Fail to QueryUserInfo.");
        return false;
    }

    return true;
}

void CollectorBase::recordLatestTime(const QString &dburl, const QString &dir, QString &latesttime)
{
    DataBase::getInstance()->queryDirLatestTime(dburl, dir, latesttime);
}

void CollectorBase::updateLatestTime(const QString &dburl, const QString &dir, const QString &latesttime)
{
    DataBase::getInstance()->updateDirLatestTime(dburl, dir, latesttime);
}


bool CollectorBase::containsFile(list<string> &files, const QString &file)
{
    list<string>::const_iterator iter = std::find(files.begin(), files.end(), file.toLocal8Bit().toStdString());
    if (iter != files.end())
    {
        return true;
    }
    else
    {
        files.push_back(file.toLocal8Bit().toStdString());
        return false;
    }
}

bool CollectorBase::checkProcessFinished(const QString &dirId)
{
    QString strLockPath = qApp->applicationDirPath() + "/lock/";
    QDir oLockDir(strLockPath);
    if (!oLockDir.exists())
    {
        oLockDir.mkpath(strLockPath);
    }

    QLockFile oMem(strLockPath + dirId);
    if (oMem.tryLock())
    {
        return true;
    }
    return false;

    //QString strLockPath = qApp->applicationDirPath() + "/lock/";
    //QDir oLockDir(strLockPath);
    //QSharedMemory oMem(dirId);
    //if (!oMem.attach())
    //{
    //    return true;
    //}
    //return false;
}


// void CollectorBase::setEnable(bool bFlag)
// {
//
// }
