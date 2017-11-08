#include "shareddirCollector.h"
#include "ctkPublicFun.h"
#include "CltDispatch.h"
//#include "IClientMessage.h"
//#include "INofity.h"
#include "pathbuilder.h"
#include "IRadarBaseDataParse.h"
#include "ctkLog.h"
#include "toVtbThread.h"
#include "pathbuilder.h"
#include <time.h>
//#ifdef _WIN32
//#include <WinNetWk.h>
//#endif
#include <QTimer>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QUrl>
#include <QDesktopServices>
#include "publicthread.h"
#include "DistributeFile.h"


bool _olderThen(const FileInfo &f1, const FileInfo &f2)
{
    if (!f1.strFileTime.empty() && !f2.strFileTime.empty())
    {
        return f1.strFileTime < f2.strFileTime;
    }
    else
    {
        return f1.strMdyTime < f2.strMdyTime;
    }
}

// 线程函数
// void SharedDirCollector::makeVtbFun(SharedDirCollector *pParent, FileInfo fileinfo)
// {
// }

SharedDirCollector::SharedDirCollector(CollectManager *pManager,  QWaitCondition &in_oCond, QMutex &in_oLocker, int &in_iLogsize)
    : CollectorBase(pManager, in_oCond, in_oLocker, in_iLogsize/*, NULL*/)
{
    m_pTsctTime = NULL;
    m_bConnected = false;

    //m_pFileWatcher = QSharedPointer<FileWatcher>(new FileWatcher());
    //QObject::connect(m_pFileWatcher.data(), SIGNAL(checkDir(QSet<QString>)), this, SLOT(onCheckDir(QSet<QString>)));
}

SharedDirCollector::~SharedDirCollector()
{
    QMutexLocker autoLocker(&m_oMutex);

    for (int i=0; i<m_threadFun.size(); ++i)
    {
        m_threadFun[i].waitForFinished();
    }
}

int SharedDirCollector::start()
{
    // 先只考虑定时调度
    QObject *op = m_pCtkManager->getService("IDispatchTimer");
    IDispatchTimer *iDt = qobject_cast<IDispatchTimer *>(op);
    if (iDt)
    {
        QSharedPointer<CltDispatch> cDispatch = QSharedPointer<CltDispatch>(new CltDispatch(this));
        //m_oThread.start();
        cDispatch->moveToThread(&m_oThread);

        bool bFlag = iDt->SetDispatchTimer(m_collectSet.dispatch.toStdString(), cDispatch.data(), QSharedPointer<TimerCallBackParam>(new TimerCallBackParam()), m_pTimerObject);
        if (bFlag)
        {
            m_oProcessor = cDispatch;
        }
        else
        {
            QSLOG_ERROR(QString("SharedDirCollector SetDispatchTimer failed: %1").arg(m_collectSet.dispatch));
        }
    }

    // 实时监控
    /* op = m_pCtkManager->getService("INotify");
    INotify *iNy = qobject_cast<INotify *>(op);
    if (iNy != NULL)
    {
    m_pWatcher = iNy->createWatcher();
    m_pWatcher->addDirWatch(m_collectSet.rltvPath);
    QObject::connect(m_pWatcher.data(), SIGNAL(fileCreated(const QString &)), this, SLOT(fileCreated(const QString &)));
    QObject::connect(m_pWatcher.data(), SIGNAL(fileModified(const QString &)), this, SLOT(fileCreated(const QString &)));
    QObject::connect(m_pWatcher.data(), SIGNAL(filerename(const QString&, const QString&)), this, SLOT(fileCreated(const QString&)));
    }*/

    m_bRun = true;

    return 0;
}

int SharedDirCollector::reStart()
{
    if (m_pWatcher.isNull())
    {
        return start();
    }

    QObject *op = m_pCtkManager->getService("IDispatchTimer");
    IDispatchTimer *iDt = qobject_cast<IDispatchTimer *>(op);
    if (iDt)
    {
        QSharedPointer<CltDispatch> cDispatch = QSharedPointer<CltDispatch>(new CltDispatch(this));
        cDispatch->moveToThread(&m_oThread);

        bool bFlag = iDt->SetDispatchTimer(m_collectSet.dispatch.toStdString(), cDispatch.data(), QSharedPointer<TimerCallBackParam>(new TimerCallBackParam()), m_pTimerObject);
        if (bFlag)
        {
            m_oProcessor = cDispatch;
        }
        else
        {
            QSLOG_ERROR(QString("SharedDirCollector SetDispatchTimer failed: %1").arg(m_collectSet.dispatch));
        }
    }

    // 实时监控
    //QObject::connect(m_pWatcher.data(), SIGNAL(fileCreated(const QString &)), this, SLOT(fileCreated(const QString &)));
    //QObject::connect(m_pWatcher.data(), SIGNAL(fileModified(const QString &)), this, SLOT(fileCreated(const QString &)));
    //QObject::connect(m_pWatcher.data(), SIGNAL(filerename(const QString&, const QString&)), this, SLOT(fileCreated(const QString&)));

    m_bRun = true;

    return 0;
}

void SharedDirCollector::getNewFiles()
{
    if (!readSet())
    {
        return;
    }

    m_bRun = (bool)m_collectSet.enable;
    if (!m_bRun)
    {
        return;
    }

    emit print(QStringLiteral("[%1]: 开始收集任务 %2[%3]").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(m_collectSet.dirName)
               .arg(m_collectSet.rltvPath));

    // E:/workspace/DataTransfer/DataTransfer_code_20170831/vs2013/apps/DataTransfer/%T-1H%t%y/%t%m/%td
    // modified by liubojun. 支持按照特定时间获取数据
    QStringList finalDirs = CPathBuilder::getFinalPathFromUrl(m_collectSet.rltvPath);
    //m_collectSet.rltvPath =

    bool bConnect = true;
    // 先测试源路径是否正常

    bConnect = testFileConnection(m_collectSet.rltvPath);

    if (!bConnect)
    {
        m_nLineState = 1;
        //emit taskState(m_collectSet, m_userInfo.user.sendType, m_nLineState);
        emit taskState(m_collectSet, 0, m_nLineState);
        return;
    }

    if (bConnect)
    {
        //QSLOG_INFO(QString("[%1] begin to getNewFiles.").arg(m_collectSet.dirName));
        m_nLineState = 0;
        //emit taskState(m_collectSet, m_userInfo.user.sendType, m_nLineState);
        emit taskState(m_collectSet, 0, m_nLineState);
        emit startGif(m_collectSet.dirID, true);
        QTime tt;
        tt.start();
        getSynclessFiles(m_collectSet.rltvPath, m_collectSet.subdirFlag);
        emit startGif(m_collectSet.dirID, false);
        QSLOG_INFO(QString("[%1] finish to getNewFiles, cost time: %2.").arg(m_collectSet.dirName).arg(tt.elapsed()/1000.f));
    }
    else
    {
        m_nLineState = 1;
        //emit taskState(m_collectSet, m_userInfo.user.sendType, m_nLineState);
        emit taskState(m_collectSet, 0, m_nLineState);
    }
}

void SharedDirCollector::getNewDirs(QString strDir, QStringList &lstDir)
{
    if (!m_bRun)
    {
        return;
    }

    QDir qdir(strDir);
    QFileInfoList qflist = qdir.entryInfoList(QDir::Dirs);
    QFileInfo qfInfo;

    string strCurDir = strDir.toLocal8Bit().data();		//中文乱码
    //string strCurDirMdyTime("");
    uint strCurDirMdyTime = 0;

    for (int i=0; i<qflist.size(); ++i)
    {
        qfInfo = qflist.at(i);
        if (qfInfo.fileName() == ".")
        {
            //strCurDirMdyTime = qfInfo.lastModified().toString("yyyyMMddHHmmss").toStdString();
            strCurDirMdyTime = qfInfo.lastModified().toTime_t();
        }
        else if (qfInfo.fileName() == "..")
        {
            continue;
        }
        else if (qfInfo.isDir())
        {
            getNewDirs(qfInfo.filePath(), lstDir);
        }
    }

    if (strCurDirMdyTime > m_pTsctTime->mapDirTime[strCurDir])
    {
        lstDir.push_back(strDir);
    }
}

void SharedDirCollector::getNewFilesFromDir(const QStringList &lstDir, FileInfoList &fileList)
{
    QString strDir;
    string strCurDir("");
    //string strMdyTime("");
    uint strMdyTime = 0;
    for (int i=0; i<lstDir.size(); ++i)
    {
        strDir = lstDir.at(i);
        strCurDir = strDir.toLocal8Bit().data();
        //string strDirNewTime = m_pTsctTime->mapDirTime[strCurDir];
        uint strDirNewTime = m_pTsctTime->mapDirTime[strCurDir];
        QDir qdir(strDir);
        QFileInfoList qflist = qdir.entryInfoList(QDir::Files);

        for (int j=0; j<qflist.size(); ++j)
        {
            if (!m_bRun)
            {
                return;
            }

            const QFileInfo &qfInfo = qflist.at(j);

            FileInfo fileinfo;
            //strMdyTime = qfInfo.lastModified().toString("yyyyMMddHHmmss").toStdString();
            //QString strMdy = qfInfo.lastModified().toString("yyyyMMddHHmmss");	//测试
            strMdyTime = qfInfo.lastModified().toTime_t();
            if (strMdyTime > m_pTsctTime->mapDirTime[strCurDir] && filterFileName(qfInfo, qfInfo.filePath(), fileinfo))
            {
                fileinfo.nFileSize = qfInfo.size();
                fileList.push_back(fileinfo);
                if (strMdyTime > strDirNewTime)
                {
                    strDirNewTime = strMdyTime;
                }
            }
        }

        m_pTsctTime->mapDirTime[strCurDir] = strDirNewTime;
    }
}

void SharedDirCollector::getNewFiles(QString strDir, FileInfoList &fileList, bool bRoot)
{
    QDir qdir(strDir);
    QFileInfoList qflist = qdir.entryInfoList();
    QFileInfo qfInfo;
    string strCurDir = strDir.toLocal8Bit().data();		//中文乱码
    //string qtCurTime("");
    //string strMdyTime("");
    uint qtCurTime = 0;
    uint strMdyTime = 0;
    for (int i=0; i<qflist.size(); i++)
    {
        qfInfo = qflist.at(i);
        if (qfInfo.fileName() == ".")		//当前目录
        {
            //strCurDir = qfInfo.path().toStdString();
            //strMdyTime = qfInfo.lastModified().toString("yyyyMMddHHmmss").toStdString();
            strMdyTime = qfInfo.lastModified().toTime_t();
            if (bRoot)		//根目录
            {
                if (strMdyTime <= m_pTsctTime->mapDirTime[strCurDir])
                {
                    return;
                }
            }
            qtCurTime = strMdyTime;
        }
        else if (qfInfo.fileName() == "..")
        {
            continue;
        }
        else if (qfInfo.isFile())
        {
            FileInfo fileinfo;
            if (filterFileName(qfInfo, qfInfo.filePath(), fileinfo))
            {
                //strMdyTime = qfInfo.lastModified().toString("yyyyMMddHHmmss").toStdString();
                strMdyTime = qfInfo.lastModified().toTime_t();
                if (strMdyTime > m_pTsctTime->mapDirTime[strCurDir])
                {
                    fileList.push_back(fileinfo);
                }
            }
        }
        else if (qfInfo.isDir())
        {
            //strMdyTime = qfInfo.lastModified().toString("yyyyMMddHHmmss").toStdString();
            strMdyTime = qfInfo.lastModified().toTime_t();
            string strDirName = qfInfo.filePath().toLocal8Bit().data();
            if (strMdyTime > m_pTsctTime->mapDirTime[strDirName])
            {
                getNewFiles(qfInfo.filePath(), fileList);
            }
        }
    }

    m_pTsctTime->mapDirTime[strCurDir] = qtCurTime;
}

void SharedDirCollector::getAllFiles(FileInfoList &fileList, QString strPath)
{
    if (strPath.isEmpty() || !m_bRun)
    {
        return;
    }

    QDir qdir(strPath);
    QFileInfoList qsList = qdir.entryInfoList();
    for (int i=0; i<qsList.size(); i++)
    {
        if (!m_bRun)
        {
            return;
        }

        const QFileInfo & qfInfo = qsList.at(i);
        const QString &filename = qfInfo.fileName();
        if (filename == "." || filename == "..")
        {
            continue;
        }

        if (qfInfo.isFile())
        {
            FileInfo fileinfo;
            if (filterFileName(qfInfo, qfInfo.filePath(), fileinfo) && IsNewFile(fileinfo))
            {
                fileList.push_back(fileinfo);
            }
        }
        else if (qfInfo.isDir())
        {
            getAllFiles(fileList, qfInfo.filePath());
        }
    }
}

int SharedDirCollector::stop()
{
    m_bRun = false;

    // 停止调度
    if (m_pManager != NULL)
    {
        QObject *op = m_pCtkManager->getService("IDispatchTimer");
        if (op != NULL)
        {
            IDispatchTimer *iDt = qobject_cast<IDispatchTimer *>(op);
            iDt->stopDispatchTimer(m_pTimerObject);
        }
    }

    // 停止监控
    //QObject::disconnect(m_pWatcher.data());

    return 0;
}

void SharedDirCollector::CollectNewFiles()
{
    FileInfoList fileList;
    QStringList lstDirs;
    getNewDirs2(m_strUrl, lstDirs);
    getNewFilesFromDir2(lstDirs, fileList);

    QSLOG_DEBUG(QStringLiteral("完成收集，新文件数目为%1个。").arg(fileList.size()));
    emit print(QStringLiteral("[%1]: 完成收集，新文件数目为%2个。").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(fileList.size()));
    if (!m_bRun)
    {
        emit print(QStringLiteral("[%1]: 停止数据收集。").arg(QDateTime::currentDateTime().toString(Qt::ISODate)));
        return;
    }

    if (fileList.size() < 1)
    {
        return;
    }

    qSort(fileList.begin(), fileList.end(), _olderThen);

    //////////////////////////////////////////////////////////////////////////

    FileInfoList::iterator it = fileList.begin();
    for (; it != fileList.end(); it++)
    {
        if (!m_bRun)
        {
            QSLOG_INFO("run flag is false.");
            break;
        }

        if (m_pFileCrypt != NULL)
        {
            //----解密解压处理----
            //QByteArray fileContent;
            QString filePath = QString::fromLocal8Bit(it->strFilePath.c_str());
            QString tmpPath = filePath + ".jm";
            //bool ok = m_pFileCrypt->UCompCryptFile(filePath, fileContent);
            bool ok = m_pFileCrypt->UCompCryptFile(filePath, tmpPath);

            // 重新写入已经解密解缩的数据
            if (ok)
            {
//                 QFile file(filePath);
//                 if (file.open( QIODevice::WriteOnly | QIODevice::Truncate))
//                 {
//                     file.write(fileContent);
//                     file.close();
//                     QSLOG_DEBUG(QStringLiteral("%1:解密压缩成功。").arg(filePath));
//                 }

                bool bFlag = QFile::remove(filePath);
                if (bFlag)
                {
                    bFlag = QFile::rename(tmpPath, filePath);
                }
                if (bFlag)
                {
                    QSLOG_DEBUG(QStringLiteral("%1:解密压缩成功。").arg(filePath));
                }
                else
                {
                    QSLOG_DEBUG(QStringLiteral("%1:解密压缩失败。").arg(filePath));
                }

                // 删除密钥文件
                QFile::remove(filePath + ".key");
            }
            //--------------------
        }

        FileInfo &fi = *it;
        fi.strOrgFilePath = fi.strFilePath;
        if (fi.nFileType >= X_TYPE)
        {
            // 直接发送
            taskDone(true, fi);
            continue;
        }

        //----vtb标准化----
        while (m_threadFun.size() >= m_pManager->m_vtbMaxNum)
        {
            if (!m_bRun)
            {
                break;
            }

            // 移除任务完成的线程
            for (int i=m_threadFun.size()-1; i>=0; --i)
            {
                if (m_threadFun.at(i).isFinished())
                {
                    m_threadFun.removeAt(i);
                }
            }

            PublicThread::msleep(100);
        }

        PublicThread::msleep(300);
    }

    // 更新文件中的时间
    m_pManager->writeLastTime();

    // 正常运行记录时间
    if (m_bRun)
    {
        emit print(QStringLiteral("[%1]: 完成所有数据预处理。").arg(QDateTime::currentDateTime().toString(Qt::ISODate)));
    }
    else
    {
        emit print(QStringLiteral("[%1]: 停止数据收集。").arg(QDateTime::currentDateTime().toString(Qt::ISODate)));
    }
}

// bool SharedDirCollector::connectToWinRemote(QString strURL,const QString& strUser,const QString& strPwd)
// {
//     // 排除"@"
//     if (strURL.size() < 2)
//     {
//         return false;
//     }
//
//     QUrl qurl("file:///" + strURL);
//     if (qurl.isLocalFile())
//     {
//         QDir qdir(strURL);
//         if (qdir.exists())
//         {
//             return true;
//         }
//     }
//
// #ifdef Q_OS_WIN
//     strURL = strURL.replace("/", "\\");		// 注意!!
//     //strURL = "\\\\192.168.20.220\\1428-data";
//     char cURL[1024] = {0};
//     //strncpy(cURL, strURL.toStdString().c_str(), sizeof(cURL) - 1);
//     strncpy(cURL, strURL.toLocal8Bit().data(), sizeof(cURL) - 1);
//     NETRESOURCEA rsc = {0};
//     rsc.lpRemoteName = (LPSTR)cURL;
//     rsc.dwType = RESOURCETYPE_DISK;
//
//     DWORD ret = WNetAddConnection2A(&rsc, (LPCSTR)(strPwd.toStdString().c_str()), (LPCSTR)(strUser.toStdString().c_str()), 0);
//     if (NO_ERROR == ret || ret == 1219)
//     {
//         return true;
//     }
// #else
// #endif
//
//     QSLOG_ERROR("connectToWinRemote failed.");
//     return false;
// }

bool SharedDirCollector::testCollection()
{
    return connectToWinRemote(m_strUrl, m_strUser, m_strPassword);
}

void SharedDirCollector::taskDone(bool bFlag, const FileInfo &file)
{
//     //static unsigned int s_count = 0;	// 测试
//     if (bFlag)
//     {
//         //发送消息队列通知其他插件
//         MSGSTRUCT msg;
//         FileInfoToString(msg.body.m_msgdata, file);
//         foreach (string qName, m_strQueNames)
//         {
//             msg.head.m_queuename = qName;
//             m_pManager->m_msgLock.lock();
//             if (!m_pManager->m_pIClient->pushMsg(msg))	//阻塞式
//             {
//                 QSLOG_ERROR(QString("pushMsg error."));
//             }
//
//             //s_count++;
//         }
//
//         //QSLOG_DEBUG(QString("has send %1").arg(s_count));
//     }
//
//     // 若主动停止收集，记录当前处理文件的时间
//     if (m_watchtype == TIMER_SCAN)
//     {
//         // 更新内存中的时间
//         QString strDirPath;
//         QString strFullPath = QString::fromLocal8Bit(file.strOrgFilePath.c_str());
//         int nPos = strFullPath.lastIndexOf("/");
//         if (nPos != -1)
//         {
//             QMutexLocker autoLocker(&m_pManager->m_timeLock);
//             strDirPath = strFullPath.left(nPos);
//             m_pLastTime->dir_time[strDirPath.toStdString()] = file.strMdyTime;
//         }
//
//         // 更新文件中的时间
// //         if (!m_bRun)
// //         {
// //             m_pManager->writeLastTime();
// //         }
//     }
//
//     if (file.nFileType < X_TYPE)
//     {
//         QMutexLocker autoLocker(&m_threadNumMutex);
//         m_nVtbCount--;
//     }
}

void SharedDirCollector::getNewDirs2(QString strDir, QStringList &lstDir)
{
    if (!m_bRun)
    {
        return;
    }

    QDir qdir(strDir);
    QFileInfoList qflist = qdir.entryInfoList(QDir::Dirs);
    QFileInfo qfInfo;

    //string strCurDir = strDir.toLocal8Bit().data();		// 中文乱码（这样序列化乱码）
    string strCurDir = strDir.toStdString();				// 这样内存中乱码，但序列化不乱吗
    string strCurDirMdyTime("");

    for (int i=0; i<qflist.size(); ++i)
    {
        qfInfo = qflist.at(i);
        if (qfInfo.fileName() == ".")
        {
            strCurDirMdyTime = qfInfo.lastModified().toString("yyyyMMddHHmmss").toStdString();
        }
        else if (qfInfo.fileName() == "..")
        {
            continue;
        }
        else if (qfInfo.isDir())
        {
            getNewDirs2(qfInfo.filePath(), lstDir);
        }
    }

    if (strCurDirMdyTime > m_pLastTime->dir_time[strCurDir])
    {
        lstDir.push_back(strDir);
    }
}

void SharedDirCollector::getNewFilesFromDir2(const QStringList &lstDir, FileInfoList &fileList)
{
    QString strDir;
    string strCurDir("");
    string strMdyTime("");
    for (int i=0; i<lstDir.size(); ++i)
    {
        if (!m_bRun)
        {
            return;
        }

        strDir = lstDir.at(i);
        //strCurDir = strDir.toLocal8Bit();
        strCurDir = strDir.toStdString();
        string strDirLastTime = m_pLastTime->dir_time[strCurDir];	// 上次时间
        string strDirNewTime = strDirLastTime;						// 本次时间
        QDir qdir(strDir);
        QFileInfoList qflist = qdir.entryInfoList(QDir::Files);

        for (int j=0; j<qflist.size(); ++j)
        {
            const QFileInfo &qfInfo = qflist.at(j);

            FileInfo fileinfo;
            strMdyTime = qfInfo.lastModified().toString("yyyyMMddHHmmss").toStdString();
            if (strMdyTime > strDirLastTime && filterFileName(qfInfo, qfInfo.filePath(), fileinfo))
            {
                //fileinfo.nFileSize = qfInfo.size();
                fileList.push_back(fileinfo);
                if (strMdyTime > strDirNewTime)
                {
                    strDirNewTime = strMdyTime;
                }
            }
        }

        //m_pLastTime->dir_time[strCurDir] = strDirNewTime;
    }
}

bool SharedDirCollector::ProcessFile(const QString &strFileFullPath)
{
    if (m_pFileCrypt != NULL)
    {
        //----解密解压处理----
        QString filePath = strFileFullPath;
        QString tmpPath = filePath + ".jm";
        bool ok = m_pFileCrypt->UCompCryptFile(filePath, tmpPath);

        // 重新写入已经解密解缩的数据
        if (ok)
        {
            bool bFlag = QFile::remove(filePath);
            if (bFlag)
            {
                bFlag = QFile::rename(tmpPath, filePath);
            }
            if (bFlag)
            {
                QSLOG_DEBUG(QStringLiteral("%1:解密解压成功。").arg(filePath));
            }
            else
            {
                QSLOG_DEBUG(QStringLiteral("%1:解密解压失败。").arg(filePath));
                emit print(QStringLiteral("[%1]: [%2]解密解压失败").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(strFileFullPath));
                return false;
            }

            // 删除密钥文件
            QFile::remove(filePath + ".key");
        }
        //--------------------
    }
    else
    {
        return false;
    }

    return true;
}

void SharedDirCollector::syncTransfer()
{

}

void SharedDirCollector::getSynclessFiles(QString strDir, bool bSubdir)
{
    if (m_tUser.lstUser.empty()/* && !readSet()*/)
    {
        return;
    }

    //QTime tt;
    //tt.start();

    QDir qdir(strDir);
    QMap<QString, QStringList> mapFileUser;
    QStringList lstFile, lstSubDir;

    // 遍历当前目录下的文件和子目录
    QFileInfoList qfileList;
    if (bSubdir)
    {
        qfileList = qdir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    }
    else
    {
        qfileList = qdir.entryInfoList(QDir::Files | QDir::NoSymLinks);
    }

    //QSLOG_DEBUG(QString("list " + strDir + " ,time=%1").arg(tt.elapsed() / 1000.f));


    QString strDBPath = qApp->applicationDirPath() + "/work/record/" + m_collectSet.dirID + "/record.index";

    // 当前目录的最新时间列表
    //QString strFileListPath = qApp->applicationDirPath() + "/work/record/" + m_collectSet.dirID + "/latestFileList.xml";
    //list<string> t_oLatestFileList = initilizeLatestFileLists(strFileListPath);

    // 当前目录数据库中记录的最后处理时间
    int iLatestTime = -9999;

    QSLOG_DEBUG(QStringLiteral("开始处理目录:%1， 收集时间范围:%2, 目录最后处理时间记录标识:%3").arg(strDir).arg(m_collectSet.col_timerange).arg(m_collectSet.recordLatestTime));

    bool bHasFileUpdate = false;
    if (m_collectSet.recordLatestTime)
    {
        recordLatestTime(strDBPath, strDir, iLatestTime);

        time_t iMaxtime = QFileInfo(strDir).lastModified().toTime_t();

        QSLOG_DEBUG(QStringLiteral("当前处理目录最后修改时间:time_t[%1], datetime[%2],数据库记录最后修改时间:time_t[%3], datetime[%4]").arg(iMaxtime).
                    arg(QDateTime::fromTime_t(iMaxtime).toString("yyyyMMddhhmmss")).arg(iLatestTime).arg(QDateTime::fromTime_t(iMaxtime).toString("yyyyMMddhhmmss")));
        if (iMaxtime <= iLatestTime)
        {
            QSLOG_DEBUG(QStringLiteral("当前目录[%1]下没有文件更新").arg(strDir));

        }
        else
        {
            bHasFileUpdate = true;
        }
    }

    // 测试 wujun
    bHasFileUpdate = true;

    QDateTime qdtCollect = QDateTime::currentDateTime().addSecs(-m_collectSet.col_timerange*60);

    // modified by liubojun @20170604
    time_t iRecordMaxTime = iLatestTime;

    // 新的最新文件列表
    //list<string> t_oNewLatestFiles = t_oLatestFileList;
    //bool listUpdate = false;

    CurlFtp m_ftp;
    for (int i=0; i<qfileList.size(); ++i)
    {
        //tt.start();

        const QFileInfo &qf = qfileList.at(i);
        if (qf.isDir())
        {
            lstSubDir.append(qfileList.at(i).filePath());
        }
        else
        {
            if (m_bRun)
            {
                if (qf.fileName().toLower().endsWith(".tmp"))
                {
                    continue;
                }
                // 新增加判断条件（记录目录最后修改时间，此处需要判断当前文件时间是否大于等于当前目录的保存上次处理最后修改时间，如果小于则不对该文件进行处理）
                if (m_collectSet.recordLatestTime)
                {
                    /*                if (!bHasFileUpdate)
                    				{
                    				continue;
                    				}*/

                    time_t ifiletime_t = qf.lastModified().toTime_t();
                    if (iLatestTime != -9999 && ifiletime_t < iLatestTime)
                    {
                        continue;
                    }
                    //else if (ifiletime_t == iLatestTime)
                    //{
                    //    // 需要判断当前的文件名是否在记录的latestFileList.xml列表中
                    //    if (containsFile(t_oLatestFileList, qf.absoluteFilePath()))
                    //    {
                    //        continue;
                    //    }
                    //}

                    //if (ifiletime_t > iRecordMaxTime)
                    //{
                    //    iRecordMaxTime = ifiletime_t;
                    //    t_oNewLatestFiles.clear();
                    //    t_oNewLatestFiles.push_back(qf.absoluteFilePath().toLocal8Bit().toStdString());
                    //    listUpdate = true;
                    //}
                    //else if (ifiletime_t == iRecordMaxTime)
                    //{
                    //    t_oNewLatestFiles.push_back(qf.absoluteFilePath().toLocal8Bit().toStdString());
                    //    listUpdate = true;
                    //}
                }

                // 必须大于设置的时间范围才收集

                if (m_collectSet.col_timerange != -1)
                {
                    // 分钟转秒
                    //if (qf.lastModified()  < QDateTime::currentDateTime().addSecs(-m_collectSet.col_timerange*60))
                    if (qf.lastModified() < qdtCollect)
                    {
                        continue;
                    }
                }

                if (filterFileName(qf.fileName()))
                {
                    TransTask tTask;
                    if (!compareWithDest(m_ftp, qf, tTask))
                    {
                        tTask.collectSet = m_collectSet;
                        //tTask.userInfo = m_userInfo.user;
                        // 发送文件
                        DistributeFile sendFile(this, m_ftp);
                        sendFile.transfer(tTask);
                    }
                }

            }
        }
    }


    // 判断是否需要记录当前目录的最后处理时间
    // 记录当前目录最后修改时间
    if (m_collectSet.recordLatestTime)
    {
        //time_t iMaxtime = QFileInfo(strDir).lastModified().toTime_t();
        //if (iMaxtime > iLatestTime)
        if (iRecordMaxTime > iLatestTime)
        {
            updateLatestTime(strDBPath, strDir, (int)iRecordMaxTime);
        }

        //if (listUpdate)
        //{
        //    updateLatestFileLists(strFileListPath, t_oNewLatestFiles);
        //}
    }

    // 入库

    // 遍历子目录
    for (int i=0; i<lstSubDir.size(); ++i)
    {
        getSynclessFiles(lstSubDir.at(i), bSubdir);
    }
}

bool SharedDirCollector::compareWithDest(CurlFtp &oCurlFtp, const QFileInfo &fi, TransTask &tTask)
{
    // libcurl不能传输大小为0的文件
    if (fi.size() <= 0)
    {
        return true;
    }

    for (int i=0; i<m_tUser.lstUser.size(); ++i)
    {
        CollectUser &cUser = m_tUser.lstUser[i];
        // QString dstFileFullPath = getDestFilePath(fi.filePath(), fi.fileName(), cUser, fi.lastModified());

        // modified by liubojun @20171029
        int iTmBaseRule = cUser.user.timebaserule;

        QString dstFileFullPath;

        // 如果基于收集目录时间，需要准确判断当前的日期，因为收集有可能是配置的时间范围
        ///360Downloads/rcv/2017/201710/%T-[0,1]d%t%Y%t%m%t%d <===> /360Downloads/rcv/2017/201710/20171029/123
        QDateTime oDt;
        if (1 == iTmBaseRule) // 基于收集目录时间
        {
            oDt = CPathBuilder::getDateTimeFrom2Urls(m_tUser.colTaskInfo.rltvPath, fi.filePath().toLocal8Bit().toStdString().c_str());
        }
        else if (2 == iTmBaseRule) // 基于文件名时间
        {
            oDt = CPathBuilder::getDateTimeFromFileName(m_tUser.colTaskInfo.fileTemplate, fi.fileName().toLocal8Bit().toStdString().c_str());
        }
        else   // 基于系统时间 + 不基于任何时间
        {
            oDt = QDateTime::currentDateTime();
        }

        dstFileFullPath = getDestFilePath(fi.filePath(), fi.fileName(), cUser, oDt, iTmBaseRule);

        // QString dstFileFullPath = getDestFilePath(fi.filePath(), fi.fileName(), cUser, QDateTime::currentDateTime(), cUser.user.timebaserule);
        QString dstFilePath = dstFileFullPath;
        tTask.fileName = fi.fileName();
        tTask.srcFileFullPath = fi.filePath();
        dstFileFullPath += fi.fileName();

        // 分发到目录
        if (cUser.user.sendType == 0)
        {
            QFile file(dstFileFullPath);
            if (0 == cUser.user.compress && 0 == cUser.user.encrypt)
            {
                //qDebug() << fi.size() << file.size();
                if (file.exists() && fi.size() == file.size())
                {
                    continue;
                }
            }
            else
            {
                if (file.exists())
                {
                    continue;
                }
            }

        }
        // 分发到FTP
        else
        {
            char ftpUrl[200] = {0};
            char ftpPath[512] = { 0 };
            char usrPwd[100] = {0};
            string strIp = cUser.user.ip.toStdString();
            int nPort = cUser.user.port;
            string strPath = dstFileFullPath.toLocal8Bit().data();
            string strName = tTask.fileName.toLocal8Bit().data();
            string strUsr = cUser.user.lgUser.toLocal8Bit().data();
            string strPwd = cUser.user.lgPass.toLocal8Bit().data();
            sprintf(ftpUrl, "ftp://%s:%d%s", strIp.c_str(), nPort, strPath.c_str());
            sprintf(usrPwd, "%s:%s", strUsr.c_str(), strPwd.c_str());

            string strDestPath = dstFileFullPath.mid(0, dstFileFullPath.lastIndexOf("/")+1).toLocal8Bit().toStdString();
            sprintf(ftpPath, "ftp://%s:%d%s", strIp.c_str(), nPort, strDestPath.c_str());
            // CurlFtp m_ftp;
            double dSize = 0;
            if (oCurlFtp.getFileSize(ftpUrl, usrPwd, strName, dSize))
            {
                if (fi.size() != (long long)dSize)
                {
                    if (-1 == oCurlFtp.deleteFtpFile(ftpPath, usrPwd, strName))
                    {
                        continue;
                    }
                }
                else
                {
                    continue;
                }
                //
            }
        }

        tTask.userInfo.append(cUser.user);
        tTask.dstFilePath.append(dstFilePath);
    }

    return tTask.userInfo.empty();
}

void SharedDirCollector::onCheckDir(QSet<QString> lstPath)
{
    foreach (QString strPath, lstPath)
    {
        getSynclessFiles(strPath, false);
    }
}

//void SharedDirCollector::fileCreated(const QString &strFilePath)
//{
//    if (m_tUser.lstUser.isEmpty() && !readSet())
//    {
//        return;
//    }
//
//    QFileInfo qf(strFilePath);
//    if (qf.isFile() && filterFileName(qf.fileName()))
//    {
//        TransTask tTask;
//        if (!compareWithDest(qf, tTask))
//        {
//            tTask.collectSet = m_collectSet;
//            // 发送文件
//            DistributeFile sendFile(this);
//            sendFile.transfer(tTask);
//        }
//    }
//}
