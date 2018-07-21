#include "shareddirCollector.h"
#include "ctkPublicFun.h"
#include "pathbuilder.h"
#include "ctkLog.h"
#include "pathbuilder.h"
#include "record.h"
#include <time.h>
//#ifdef _WIN32
//#include <WinNetWk.h>
//#endif
#include <QTimer>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QUrl>
//#include <QDesktopServices>
#include "publicthread.h"
#include "DistributeFile.h"
#include "change_name.h"
#include "LibCurlFtp.h"
#include "LibCurlSFtp.h"


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

SharedDirCollector::SharedDirCollector(QWaitCondition &in_oCond, QMutex &in_oLocker, int &in_iLogsize)
    : CollectorBase(in_oCond, in_oLocker, in_iLogsize/*, NULL*/)
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
    //QObject *op = m_pCtkManager->getService("IDispatchTimer");
    //IDispatchTimer *iDt = qobject_cast<IDispatchTimer *>(op);
    //if (iDt)
    //{
    //    QSharedPointer<CltDispatch> cDispatch = QSharedPointer<CltDispatch>(new CltDispatch(this));
    //    m_oThread.start();
    //    cDispatch->moveToThread(&m_oThread);

    //    bool bFlag = iDt->SetDispatchTimer(m_collectSet.dispatch.toStdString(), cDispatch.data(), QSharedPointer<TimerCallBackParam>(new TimerCallBackParam()), m_pTimerObject);
    //    if (bFlag)
    //    {
    //        m_oProcessor = cDispatch;
    //    }
    //    else
    //    {
    //        QSLOG_ERROR(QString("SharedDirCollector SetDispatchTimer failed: %1").arg(m_collectSet.dispatch));
    //    }
    //}

    //// 实时监控
    ///* op = m_pCtkManager->getService("INotify");
    //INotify *iNy = qobject_cast<INotify *>(op);
    //if (iNy != NULL)
    //{
    //m_pWatcher = iNy->createWatcher();
    //m_pWatcher->addDirWatch(m_collectSet.rltvPath);
    //QObject::connect(m_pWatcher.data(), SIGNAL(fileCreated(const QString &)), this, SLOT(fileCreated(const QString &)));
    //QObject::connect(m_pWatcher.data(), SIGNAL(fileModified(const QString &)), this, SLOT(fileCreated(const QString &)));
    //QObject::connect(m_pWatcher.data(), SIGNAL(filerename(const QString&, const QString&)), this, SLOT(fileCreated(const QString&)));
    //}*/

    m_bRun = true;

    return 0;
}

int SharedDirCollector::reStart()
{
    //if (m_pWatcher.isNull())
    //{
    //    return start();
    //}

    //QObject *op = m_pCtkManager->getService("IDispatchTimer");
    //IDispatchTimer *iDt = qobject_cast<IDispatchTimer *>(op);
    //if (iDt)
    //{
    //    QSharedPointer<CltDispatch> cDispatch = QSharedPointer<CltDispatch>(new CltDispatch(this));
    //    cDispatch->moveToThread(&m_oThread);

    //    bool bFlag = iDt->SetDispatchTimer(m_collectSet.dispatch.toStdString(), cDispatch.data(), QSharedPointer<TimerCallBackParam>(new TimerCallBackParam()), m_pTimerObject);
    //    if (bFlag)
    //    {
    //        m_oProcessor = cDispatch;
    //    }
    //    else
    //    {
    //        QSLOG_ERROR(QString("SharedDirCollector SetDispatchTimer failed: %1").arg(m_collectSet.dispatch));
    //    }
    //}

    //// 实时监控
    ////QObject::connect(m_pWatcher.data(), SIGNAL(fileCreated(const QString &)), this, SLOT(fileCreated(const QString &)));
    ////QObject::connect(m_pWatcher.data(), SIGNAL(fileModified(const QString &)), this, SLOT(fileCreated(const QString &)));
    ////QObject::connect(m_pWatcher.data(), SIGNAL(filerename(const QString&, const QString&)), this, SLOT(fileCreated(const QString&)));

    //m_bRun = true;

    return 0;
}

void SharedDirCollector::getNewFiles()
{
    if (!m_oTaskLocker.tryLock())
    {
        return;
    }

    QSharedPointer<QMutex> autoUnlock(&m_oTaskLocker, &QMutex::unlock);

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

    m_oSubDirFilter.init();
    foreach (QString strDir, finalDirs)
    {

        bool bConnect = true;
        // 先测试源路径是否正常

        bConnect = testFileConnection(strDir);

        if (!bConnect)
        {
            m_nLineState = 1;
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
            CDirRecord oRecord(m_collectSet.dirName);
            if (m_collectSet.recordLatestTime)
            {
                oRecord.loadLatestFileSize();
            }

            DIRLEVEL oDirLevel;
            oDirLevel.dir = strDir;
            oDirLevel.level = 0;	// 根目录层级为0
            m_strSubDirTemplate = m_collectSet.subDirTemplate;


            getSynclessFiles(oDirLevel, m_collectSet.subdirFlag, oRecord);
            if (m_collectSet.recordLatestTime)
            {
                oRecord.reflush();
            }
            emit startGif(m_collectSet.dirID, false);
            QSLOG_INFO(QString("[%1] finish to getNewFiles, cost time: %2 seconds").arg(m_collectSet.dirName).arg(tt.elapsed()/1000.f));
        }
        else
        {
            m_nLineState = 1;
            emit taskState(m_collectSet, 0, m_nLineState);
        }

    }

    emit finished();
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

void SharedDirCollector::getNewFiles(const CollectTask &in_oTask)
{

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

    //// 停止调度
    //if (m_pManager != NULL)
    //{
    //    QObject *op = m_pCtkManager->getService("IDispatchTimer");
    //    if (op != NULL)
    //    {
    //        IDispatchTimer *iDt = qobject_cast<IDispatchTimer *>(op);
    //        iDt->stopDispatchTimer(m_pTimerObject);
    //    }
    //}

    //// 停止监控
    ////QObject::disconnect(m_pWatcher.data());

    return 0;
}

void SharedDirCollector::deleteSelf()
{
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    // deleteLater();
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
    //if (m_pFileCrypt != NULL)
    //{
    //    //----解密解压处理----
    //    QString filePath = strFileFullPath;
    //    QString tmpPath = filePath + ".jm";
    //    bool ok = m_pFileCrypt->UCompCryptFile(filePath, tmpPath);

    //    // 重新写入已经解密解缩的数据
    //    if (ok)
    //    {
    //        bool bFlag = QFile::remove(filePath);
    //        if (bFlag)
    //        {
    //            bFlag = QFile::rename(tmpPath, filePath);
    //        }
    //        if (bFlag)
    //        {
    //            QSLOG_DEBUG(QStringLiteral("%1:解密解压成功。").arg(filePath));
    //        }
    //        else
    //        {
    //            QSLOG_DEBUG(QStringLiteral("%1:解密解压失败。").arg(filePath));
    //            emit print(QStringLiteral("[%1]: [%2]解密解压失败").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(strFileFullPath));
    //            return false;
    //        }

    //        // 删除密钥文件
    //        QFile::remove(filePath + ".key");
    //    }
    //    //--------------------
    //}
    //else
    //{
    //    return false;
    //}

    return true;
}

void SharedDirCollector::syncTransfer()
{

}

void SharedDirCollector::getSynclessFiles(DIRLEVEL in_processDir, bool bSubdir, CDirRecord &oRecord)
{
    if (!m_bRun)
    {
        return;
    }
    //if (m_tUser.lstUser.empty()/* && !readSet()*/)
    //{
    //    return;
    //}

    //QTime tt;
    //tt.start();
    QString strDir = in_processDir.dir;
    QDir qdir(strDir);
    QMap<QString, QStringList> mapFileUser;
    // QStringList lstFile, lstSubDir;
    QStringList lstFile;
    QList<DIRLEVEL> lstSubDir;
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


    bool bHasFileUpdate = false;


    // 测试 wujun
    bHasFileUpdate = true;

    QDateTime qdtCollect = QDateTime::currentDateTime().addSecs(-m_collectSet.col_timerange*60);

    // CurlFtp m_ftp;
    QSharedPointer<FtpBase> pFtpSource;
    QSharedPointer<FtpBase> pFtpDest;
    //QSharedPointer<FtpBase> pFtpTemp;
    for (int i=0; i<qfileList.size(); ++i)
    {
        if (!m_bRun)
        {
            return;
        }
        //tt.start();

        const QFileInfo &qf = qfileList.at(i);

        if (qf.isDir())
        {
            // 如果需要遍历子目录
            QString strPath = qf.absoluteFilePath() + "/";
            DIRLEVEL oDirLevel;
            oDirLevel.dir = strPath;
            oDirLevel.level = in_processDir.level + 1;

            // modified by liubojun @20180222,针对北京民航张睿之现场反馈问题提出的新需求
            // 在此处加入过滤条件，支持针对子目录的模糊匹配
            QString strDirName = qf.fileName();
            if (m_oSubDirFilter.match(m_strSubDirTemplate, strDirName, oDirLevel.level))
            {
                lstSubDir.append(oDirLevel);
            }
            else
            {
                QSLOG_DEBUG(QString("subdir %1 is not match the rule").arg(oDirLevel.dir));
            }

            //lstSubDir.append(qfileList.at(i).filePath());
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

                    QString ifiletime_t = qf.lastModified().toString("yyyyMMddhhmmss");
                    oRecord.updateLatestFileSize(qf.absolutePath(), qf.fileName(), ifiletime_t, qf.size());

                    if (!oRecord.checkIsNewFile(qf.absolutePath(), qf.fileName(), ifiletime_t, qf.size()))
                    {
                        continue;
                    }

                }

                // 必须大于设置的时间范围才收集

                if (m_collectSet.col_timerange != -1)
                {
                    // 分钟转秒
                    if (qf.lastModified() < qdtCollect)
                    {
                        continue;
                    }
                }

                if (filterFileName(qf.fileName()))
                {
                    TransTask tTask;
                    if (!compareWithDest(pFtpDest, qf, tTask) && m_bRun)
                    {
                        tTask.collectSet = m_collectSet;
                        //tTask.userInfo = m_userInfo.user;
                        // 发送文件
                        DistributeFile sendFile(this);
                        if (!sendFile.transfer(tTask, pFtpSource, pFtpDest) && m_collectSet.recordLatestTime)
                        {
                            oRecord.updateSendFailure(qf.absolutePath(), qf.fileName());
                        }
                    }
                }

            }
        }
    }

    // 入库

    // 遍历子目录
    // for (int i=0; i<lstSubDir.size(); ++i)
    for (int i = 0; i<lstSubDir.size(); ++i)
    {
        getSynclessFiles(lstSubDir.at(i), bSubdir, oRecord);
    }
}

bool SharedDirCollector::compareWithDest(QSharedPointer<FtpBase> &pCurlFtp, const QFileInfo &fi, TransTask &tTask)
{
    // libcurl不能传输大小为0的文件
    if (fi.size() <= 0)
    {
        return true;
    }
    //QSLOG_DEBUG("compare with dest");

    //for (int i=0; i<m_tUser.lstUser.size(); ++i)
    {
        CollectUser &cUser = m_tUser.sendUser;
        // QString dstFileFullPath = getDestFilePath(fi.filePath(), fi.fileName(), cUser, fi.lastModified());

        // modified by liubojun @20171029
        int iTmBaseRule = cUser.iTimeRule;

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
        //QSLOG_DEBUG("get dest path");
        // QString dstFileFullPath = getDestFilePath(fi.filePath(), fi.fileName(), cUser, QDateTime::currentDateTime(), cUser.user.timebaserule);
        QString dstFilePath = dstFileFullPath;
        tTask.fileName = fi.fileName();
        tTask.srcFileFullPath = fi.filePath();
        // dstFileFullPath += fi.fileName();
        //QSLOG_DEBUG(QString("name = %1, rule = %2").arg(fi.fileName()).arg(cUser.rename_rule));
        tTask.strDestFileName = QString::fromLocal8Bit(CChangeName::change_name_by_id(fi.fileName().toLocal8Bit().toStdString().c_str(), cUser.rename_rule.toLocal8Bit().toStdString().c_str()).c_str());
        dstFileFullPath += tTask.strDestFileName;
        //QSLOG_DEBUG("get dest name");

        // 分发到目录
        if (cUser.user.sendType == 0)
        {
            QFileInfo file(dstFileFullPath);

            if (file.exists())
            {
                if (fi.size() == file.size())
                {
                    QSLOG_DEBUG(QString("dest file exist and size is the same with the source ,file content is not the same"
                                        "source file: %1, use content comare : %2").arg(fi.fileName()).arg(m_collectSet.compareContent));
                    if (m_collectSet.compareContent)
                    {
                        QFile file_source(fi.absoluteFilePath());
                        QFile file_dest(file.absoluteFilePath());

                        if (!file_source.open(QIODevice::ReadOnly) || !file_dest.open(QIODevice::ReadOnly))
                        {
                            QSLOG_ERROR(QString("open file failure"));
                        }

                        if (file_source.readAll() == file_dest.readAll())
                        {
                            //continue;
                        }

                    }
                    else
                    {
                        //continue;
                    }
                }
                else
                {
                    QFile oFile(dstFileFullPath);
                    oFile.remove();
                }

            }


        }
        // 分发到FTP
        else
        {
            if (cUser.user.sendType == 1)
            {
                if (pCurlFtp.isNull())
                {
                    pCurlFtp = QSharedPointer<FtpBase>(new CFtp());
                }
            }
            else
            {
                if (pCurlFtp.isNull())
                {
                    pCurlFtp = QSharedPointer<FtpBase>(new SFtp());
                }
            }

            pCurlFtp->connectToHost(cUser.user.ip, cUser.user.port, cUser.user.lgUser, cUser.user.lgPass);
            pCurlFtp->setTransferMode(cUser.user.ftpTransferMode == 0 ? Passive : Active);

            long long dSize = (long long)pCurlFtp->getFileSize(dstFileFullPath);
            if (-1 != dSize)
            {
                if (fi.size() != dSize)
                {
                    if (-1 == pCurlFtp->remove(dstFileFullPath))
                    {
                        //continue;
                    }
                }
                else
                {
                    //continue;
                }

            }

            //string strIp = cUser.user.ip.toStdString();
            //int nPort = cUser.user.port;
            ////string strPath = dstFileFullPath.toLocal8Bit().data();
            //string strName = tTask.strDestFileName.toLocal8Bit().data();
            //string strUsr = cUser.user.lgUser.toLocal8Bit().data();
            //string strPwd = cUser.user.lgPass.toLocal8Bit().data();
            //QString ftpUrl = QString("ftp://%1:%2%3").arg(strIp.c_str()).arg(nPort).arg(dstFileFullPath);
            ////sprintf(ftpUrl, "ftp://%s:%d%s", strIp.c_str(), nPort, strPath.c_str());
            //QString usrPwd = QString("%1:%2").arg(strUsr.c_str()).arg(strPwd.c_str());
            ////sprintf(usrPwd, "%s:%s", strUsr.c_str(), strPwd.c_str());

            //int iIndex = dstFileFullPath.lastIndexOf("/");
            //string strDestPath;
            //if (dstFileFullPath.length() > iIndex)
            //{
            //    strDestPath = dstFileFullPath.mid(0, iIndex + 1).toLocal8Bit().toStdString();
            //}
            //else
            //{
            //    QSLOG_ERROR("dstFileFullPath.length() is incorrect.");
            //}
            ////string strDestPath = dstFileFullPath.mid(0, iIndex + 1).toLocal8Bit().toStdString();
            //QString ftpPath = QString("ftp://%1:%2%3").arg(strIp.c_str()).arg(nPort).arg(strDestPath.c_str());
            ////sprintf(ftpPath, "ftp://%s:%d%s", strIp.c_str(), nPort, strDestPath.c_str());
            //// CurlFtp m_ftp;
            //double dSize = 0;
            //if (oCurlFtp.getFileSize(ftpUrl.toLocal8Bit().toStdString().c_str(),
            //                         usrPwd.toLocal8Bit().toStdString().c_str(), strName, dSize))
            //{
            //    if (fi.size() != (long long)dSize)
            //    {
            //        if (-1 == oCurlFtp.deleteFtpFile(ftpPath.toLocal8Bit().toStdString().c_str(),
            //                                         usrPwd.toLocal8Bit().toStdString().c_str(), strName))
            //        {
            //            continue;
            //        }
            //    }
            //    else
            //    {
            //        continue;
            //    }
            //    //
            //}
        }

        //tTask.userInfo.append(cUser.user);
        //tTask.dstFilePath.append(dstFilePath);
        tTask.userInfo = (cUser.user);
        tTask.dstFilePath = (dstFilePath);
    }

    // return tTask.userInfo.empty();
    return tTask.userInfo.userID.isEmpty();
}

//void SharedDirCollector::onCheckDir(QSet<QString> lstPath)
//{
//    foreach (QString strPath, lstPath)
//    {
//        getSynclessFiles(strPath, false);
//    }
//}

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
