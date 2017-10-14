#include "CollectorBase.h"
#include "IRadarBaseDataParse.h"
#include "IRadarPathAdapter.h"
#include "pathbuilder.h"
#include "ctkPublicFun.h"
#include "curlftp.h"
#include <QUrl>
#include <QRegExp>
#include <QFileInfoList>
#include <QStringList>
#include <QMutexLocker>
#include <qthread.h>
#include <lastCollectTime.h>
#include <algorithm>

// 初始化静态成员（自定义类对象，该类必须有默认构造函数）
TransCollectTimeList CollectorBase::m_lstTCtime;
bool CollectorBase::m_bReadTime = false;
QMutex CollectorBase::m_mutexTime;
//QString CollectorBase::m_strTctimePath;
QString CollectorBase::m_strSttimePath;

CollectorBase::CollectorBase(CollectManager *pManager, QWaitCondition &in_oCond, QMutex &in_oLocker, int &in_iLogsize, QObject *pParent)
    : m_pManager(pManager),
      m_oCond(in_oCond),
      m_oLocker(in_oLocker),
      m_iLogsize(in_iLogsize)
    , QObject(pParent)		//从QObject继承的类需要写pParent!!
{
    m_bFinish = true;

    m_pLastTime = NULL;
    m_pCtkManager = m_pManager->m_pCtkManager;

    QObject *obj = NULL;
    //获取基数据解析器
    obj = m_pCtkManager->getService("IDataParse");
    m_pDataParseServ = (qobject_cast<IDataParse *>(obj));

    obj = m_pCtkManager->getService("IRadarPathAdapter");
    if (NULL != obj)
    {
        m_pMakePath = (qobject_cast<IRadarPathAdapter *>(obj))->createRadarProductPath();
    }

    obj = m_pCtkManager->getService("ICrypt");
    if (NULL != obj)
    {
        m_pFileCrypt = qobject_cast<IFileCrypt *>(obj);
    }

    m_bRun = false;
    m_nLineState = 0;

    m_oThread.start();
    this->moveToThread(&m_oThread);
    connect(this, SIGNAL(begin()), this, SLOT(onBegined()));
}

CollectorBase::~CollectorBase()
{
    m_oThread.requestInterruption();
    //m_oThread.quit();
    //m_oThread.wait();
    m_oThread.terminate();
}

bool CollectorBase::filterFileName(const QFileInfo &fi, const QString &strFilePath, FileInfo &fileInfo)
{
    return true;

//     bool bFlag = false;
//     QRegExp reg;
//     reg.setCaseSensitivity(Qt::CaseInsensitive);
//     QString strMatchRegular;
//
//     FILENAME_TABLE::const_iterator it = m_mapNameTypeRules.begin();
//     for (; it != m_mapNameTypeRules.end(); ++it)
//     {
//         const FileTypeInfo &typeInfo = it.value();
//         reg.setPattern(typeInfo.strNameReg.c_str());
//
//         if (reg.exactMatch(fi.fileName()))
//         {
//             bFlag = true;
//
//             fileInfo.strFilePath = fi.filePath().toLocal8Bit().data();
//             fileInfo.strFileName = fi.fileName().toLocal8Bit().data();		//QString转string，防止中文乱码
//             fileInfo.nFileType = it.key();
//             fileInfo.strMdyTime = fi.lastModified().toString("yyyyMMddHHmmss").toStdString();
//             fileInfo.nFileSize = fi.size();
//
//             if (!m_strStation.isEmpty())
//             {
//                 fileInfo.strStation = m_strStation.toStdString();
//             }
//
//             getFileStringType(fileInfo.strFileType, it.key());
//
//             //////////////////////////////////////////////////////////////////////////
//             // 其他类型 [10/10/2016 wujun]
//             if (it.key() == X_TYPE)
//             {
//                 QString strFileName = QString::fromStdString(fileInfo.strFileName);
//                 if (strFileName.endsWith("MAXR.bin", Qt::CaseInsensitive))
//                 {
//                     fileInfo.nFileType = MAXR;
//                     fileInfo.strFileType = "MAXR";
//                 }
//             }
//             //////////////////////////////////////////////////////////////////////////
//
//             // 根据类型获取站号、属性等
//             int nStationIndex = it.value().nStationIndex;
//             int nDateTimeIndex = it.value().nDateTimeIndex;
//
//             if (nStationIndex != CollectSet::INVALID)
//             {
//                 int pos = reg.indexIn(fi.fileName());
//                 if (pos > -1)
//                 {
//                     fileInfo.strStation = reg.cap(nStationIndex).toStdString();
//                     if (fileInfo.strStation.length() == 3)
//                     {
//                         fileInfo.strStation = "Z9" + fileInfo.strStation;
//                     }
//                 }
//             }
//
//             if (nDateTimeIndex != CollectSet::INVALID)
//             {
//                 int pos = reg.indexIn(fi.fileName());
//                 if (pos > -1)
//                 {
//                     fileInfo.strFileTime = reg.cap(nDateTimeIndex).toStdString();
//                     std::string strTempTime("");
//                     for (int index = 0; index < (int)fileInfo.strFileTime.length(); ++index)
//                     {
//                         char c = fileInfo.strFileTime.at(index);
//                         if (c >= '0' && c <= '9')
//                         {
//                             strTempTime += c;
//                         }
//                     }
//                     fileInfo.strFileTime = strTempTime;
//                 }
//             }
//
//             break;
//         }
//     }
//
//     // 决定该文件的发送队列
//     if (bFlag)
//     {
//         std::map<string, std::vector<CDataInfo> >::iterator iter = m_queueInfo.find(fileInfo.strFileType);
//         if (iter != m_queueInfo.end())
//         {
//             // 查看文件到底匹配的哪个正则表达式
//             for (size_t i = 0; i < iter->second.size(); ++i)
//             {
//                 const CDataInfo &di = iter->second.at(i);
//                 if (di.typeReg == strMatchRegular.toStdString())
//                 {
//                     // 匹配上
//                     fileInfo.strDestQueues = di.queNames;
//
//                     break;
//                 }
//             }
//         }
//     }
//
//
//     return bFlag;
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
    //////string strLast;
    //////std::map<std::string, StationTypeTime>::iterator iter = m_lstSTtime.lsInfos.find(fileInfo.strStation);
    //////if (iter != m_lstSTtime.lsInfos.end())
    //////{
    //////    strLast = iter->second.mapTypeTime[fileInfo.strOldType];
    //////    if (fileInfo.strMdyTime <= strLast)
    //////    {
    //////        return false;
    //////    }
    //////    else
    //////    {
    //////        QSLOG_DEBUG(QString("[%1]: %2 > %3").arg(QString(fileInfo.strFileName.c_str()), QString(fileInfo.strMdyTime.c_str()), QString(strLast.c_str())));
    //////        return true;
    //////    }
    //////}

    return true;

    /*foreach (const StationTypeTime &time, m_lstSTtime.lsInfos)
    {
    if (fileInfo.strStation == time.strStation)
    {
    strLast = time.mapTypeTime[fileInfo.strOldType];
    if (fileInfo.strMdyTime <= strLast)
    {
    return false;
    }
    else
    {
    QSLOG_DEBUG(QString("[%1]: %2 > %3").arg(QString(fileInfo.strFileName.c_str()), QString(fileInfo.strMdyTime.c_str()), QString(strLast.c_str())));
    return true;
    }
    }
    }

    return true;*/
}

// bool CollectorBase::IsHistyFile(FileInfo &fileInfo)
// {
// 	if (fileInfo.strMdyTime < m_strStartTime ||
// 		fileInfo.strMdyTime > m_strEndTime)
// 	{
// 		return false;
// 	}
//
// 	return true;
// }

bool CollectorBase::ReadLastTime()
{
    return true;
}

void CollectorBase::DeleteTcTime()
{
    QMutexLocker locker(&m_mutexTime);
    if (m_pTsctTime != NULL)
    {
        m_lstTCtime.lsts.remove(*m_pTsctTime);
        TransCollectTimeToXml(m_strTctimePath.toLocal8Bit().data(), m_lstTCtime);
    }
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
    emit showLog(m_collectSet, info, infoType);
    // QThread::msleep(20);
}

QString CollectorBase::getDestFilePath(const QString &srcFileFullPath, const QString &fileName, const CollectUser &cUser)
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
    getNewFiles();
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

bool CollectorBase::testFtpConnection(QString ip, int port, QString lgUser, QString lgPass, int transfermode, int connectmode)
{
    CurlFtp cFtp(this);
    char url[100] = {0};
    char usrPwd[100] = {0};
    sprintf(url, "ftp://%s:%d", ip.toStdString().c_str(), port);
    // sprintf(url, "sftp://%s", ip.toStdString().c_str(), port);
    sprintf(usrPwd, "%s:%s", lgUser.toStdString().c_str(), lgPass.toStdString().c_str());
    cFtp.setFtpTransferMode(m_collectSet.ftp_transferMode);
    cFtp.setFtpConnectMode(m_collectSet.ftp_connectMode);

    return cFtp.connectToHost(url, usrPwd);
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
    // 读取分发用户设置
    if (!DataBase::getInstance()->QueryUserInfo(m_tUser))
    {
        QSLOG_ERROR("Fail to QueryUserInfo.");
        return false;
    }

    return true;
}

void CollectorBase::recordLatestTime(const QString &dburl, const QString &dir, int &latesttime)
{
    DataBase::getInstance()->queryDirLatestTime(dburl, dir, latesttime);
}

void CollectorBase::updateLatestTime(const QString &dburl, const QString &dir, int latesttime)
{
    DataBase::getInstance()->updateDirLatestTime(dburl, dir, latesttime);
}

list<string> CollectorBase::initilizeLatestFileLists(const QString &dir)
{
    LatestFileList t_oFileLists;
    if (!XmlToLatestFileList(dir.toLocal8Bit().toStdString(), t_oFileLists))
    {
        QSLOG_ERROR(dir + " XmlToLatestFileList failure");
    }
    return t_oFileLists.m_files;
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

void CollectorBase::updateLatestFileLists(const QString &dir, const list<string> &files)
{
    LatestFileList t_oFileLists;
    t_oFileLists.m_files = files;
    if (!LatestFileListToXml(dir.toLocal8Bit().toStdString(), t_oFileLists))
    {
        QSLOG_ERROR(dir + " LatestFileListToXml failure");
    }
}

QString CollectorBase::getFinalPathFromUrl(const QString &path)
{
    QString strNewUrl;

    // %T-1H %T表示当前的系统时间 -1H表示基于当前系统时间的增减量，还支持天d、分钟M、秒s
    // %T-1H%t%y/%t%m/%t%d/%t%H/%t%M/%t%s, %t后接时间，%y%m%d%H%M%s分别表示年月日时分秒
    //QUrl oUrl(path);
    QString strUrl = path;//oUrl.path();

    QString strFinalUrl;
    QStringList urlList = strUrl.split("/");

    //
    for (int i = 0; i != path.length(); ++i)
    {
        if ('/' != path.at(i))
        {
            break;
        }
        strFinalUrl.append(path.at(i));
    }
    foreach(QString str, urlList)
    {
        if (str.isEmpty())
        {
            continue;
        }
        strFinalUrl.append(parseFromUrl(str));
    }

    QSLOG_DEBUG("final url = " + strFinalUrl);
    return strFinalUrl;
}

QString CollectorBase::parseFromUrl(const QString &url)
{
    // 最终的时间
    QDateTime finalDateTime = QDateTime::currentDateTime();
    QString strNewUrl("");
    QString strFinalUrl;
    QString strUrl = url;

    int index0 = 0;
    int index1 = 0;

    if (-1 != (index1 = strUrl.indexOf("%T")))
    {
        QSLOG_DEBUG(QString("find add or sub character"));
        // 找到%T与%t之间的内容

        int index2 = strUrl.indexOf("%t", index1);

        if (-1 == index2)
        {
            QSLOG_ERROR("check url format error, should be with t after T");
            return strNewUrl;
        }
        // 截断之间的内容
        QString valueRange = strUrl.mid(index1 + 2, index2 - index1 - 2);

        // 分析第一个是+号还是-号
        QChar c = valueRange.at(0);
        if ('-' == c || '+' == c)
        {
            // 计算当前时间的增减量
            int addorsub = 1;
            if ('-' == c)
            {
                addorsub = -1;
            }

            // 再计算后面的时间内容
            // 确定valueRange的最后一个字符
            QChar lastc = valueRange.at(valueRange.length() - 1);
            int iTime = valueRange.mid(1, valueRange.length() - 2).toInt() * addorsub;

            int step = 0;
            if ('H' == lastc)
            {
                finalDateTime = finalDateTime.addSecs(iTime * 3600);
                //QSLOG_DEBUG(QString("%1 H").arg(iTime));
            }
            else if ('d' == lastc)
            {
                finalDateTime = finalDateTime.addDays(iTime);
                //QSLOG_DEBUG(QString("%1 d").arg(iTime));
            }
            else if ('M' == lastc)
            {
                finalDateTime = finalDateTime.addSecs(iTime * 60);
                //QSLOG_DEBUG(QString("%1 M").arg(iTime));
            }
            else if ('s' == lastc)
            {
                finalDateTime = finalDateTime.addSecs(iTime);
                //QSLOG_DEBUG(QString("%1 s").arg(iTime));
            }
            else
            {
                QSLOG_ERROR("check unsupport character after %T should be H / d / M / s");
                return strNewUrl;
            }
        }
        else
        {
            QSLOG_ERROR("The character after %T is incorrect, should be + or -");
            return strNewUrl;
        }

        QStringList strTimeBody = strUrl.mid(index2).split("%t");
        QString strDateTimeFormat;
        foreach(QString strBody, strTimeBody)
        {
            if (strBody.isEmpty())
            {
                continue;
            }
            // %y%m%d%H%M%s
            if ("%Y" == strBody)
            {
                strDateTimeFormat.append("yyyy");
            }
            else if ("%m" == strBody)
            {
                strDateTimeFormat.append("MM");
            }
            else if ("%d" == strBody)
            {
                strDateTimeFormat.append("dd");
            }
            else if ("%H" == strBody)
            {
                strDateTimeFormat.append("hh");
            }
            else if ("%M" == strBody)
            {
                strDateTimeFormat.append("mm");
            }
            else if ("%S" == strBody)
            {
                strDateTimeFormat.append("ss");
            }
            else
            {
                QSLOG_ERROR("The character after %t is incorrect, should be ymdHMs ");
                return strNewUrl;
            }
        }

        strFinalUrl.append(finalDateTime.toString(strDateTimeFormat));

    }
    else if (-1 != (index1 = strUrl.indexOf("%t")))
    {
        QSLOG_DEBUG(QString("find time character"));
        QStringList strTimeBody = strUrl.split("%t");
        QString strDateTimeFormat;
        foreach(QString strBody, strTimeBody)
        {
            if (strBody.isEmpty())
            {
                continue;
            }
            // %y%m%d%H%M%s
            if ("%Y" == strBody)
            {
                strDateTimeFormat.append("yyyy");
            }
            else if ("%m" == strBody)
            {
                strDateTimeFormat.append("MM");
            }
            else if ("%d" == strBody)
            {
                strDateTimeFormat.append("dd");
            }
            else if ("%H" == strBody)
            {
                strDateTimeFormat.append("hh");
            }
            else if ("%M" == strBody)
            {
                strDateTimeFormat.append("mm");
            }
            else if ("%S" == strBody)
            {
                strDateTimeFormat.append("ss");
            }
            else
            {
                QSLOG_ERROR("The character after %t is incorrect, should be ymdHMs");
                return strNewUrl;
            }
        }

        strFinalUrl.append(finalDateTime.toString(strDateTimeFormat));
    }
    else
    {
        strFinalUrl.append(url);
    }

    return strFinalUrl + "/";
}

// void CollectorBase::setEnable(bool bFlag)
// {
//
// }
