#include "ftpCollector.h"
#include "pathbuilder.h"
#include "ctkLog.h"
#include "publicthread.h"
#include "DistributeFile.h"
#include "change_name.h"
#include "curlftp.h"
#include "record.h"

#include <QElapsedTimer>
#include <QCoreApplication>
#include <QProcess>

FtpCollector::FtpCollector(QWaitCondition &in_oCond, QMutex &in_oLocker, int &in_iLogsize)
    : CollectorBase(in_oCond, in_oLocker, in_iLogsize)
{
}

FtpCollector::~FtpCollector()
{

}

int FtpCollector::start()
{
    m_bRun = true;
    /*QObject *op = m_pCtkManager->getService("IDispatchTimer");
    QObject *op = in_pService;
    IDispatchTimer *iDt = qobject_cast<IDispatchTimer *>(op);
    if (iDt)
    {
        m_pCftp = QSharedPointer<CurlFtp>(new CurlFtp(this));
        QObject::connect(m_pCftp.data(), SIGNAL(done()), this, SLOT(ftpDone()));

        QSharedPointer<CltDispatch> cDispatch = QSharedPointer<CltDispatch>(new CltDispatch(this));
        m_oThread.start();
        cDispatch->moveToThread(&m_oThread);

        bool bFlag = iDt->SetDispatchTimer(m_collectSet.dispatch.toStdString(), cDispatch.data(), QSharedPointer<TimerCallBackParam>(new TimerCallBackParam()), m_pTimerObj);
        if (bFlag)
        {
            m_oProcess = cDispatch;
        }
        else
        {
            QSLOG_ERROR(QString("FtpCollector SetDispatchTimer failed: %1").arg(m_collectSet.dispatch));
        }

        m_bRun = true;
    }*/

    return 0;
}

int FtpCollector::stop()
{
    m_bRun = false;


    return 0;
}

void FtpCollector::stoprcv(int exitcode, QProcess::ExitStatus status)
{
    m_bChildProcessRunning = false;
}

void FtpCollector::getNewFiles()
{
    //if (!m_bFinish)
    //{
    //    QSLOG_DEBUG("Last ftp collect isn't finished.");
    //    return;
    //}
    QSLOG_DEBUG("get new files from ftp server.");
    if (!readSet())
    {
        QSLOG_ERROR("query database error.");
        return;
    }

    m_bRun = (bool)m_collectSet.enable;
    if (!m_bRun)
    {
        QSLOG_DEBUG("task is stopped.");
        return;
    }

    // 使用共享内存，查看当前的QProcess是否存在，如果存在的话，则无需再次启动针对该目录ID的收集任务
    if (!checkProcessFinished(m_collectSet.dirID))
    {
        QSLOG_DEBUG("Last ftp collect isn't finished.");
        return;
    }

    QString strLogInfo(QStringLiteral("[%1]: 开始收集任务 %2[%3]").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(m_collectSet.dirName)
                       .arg(m_collectSet.rltvPath));
    emit print(strLogInfo);
    QSLOG_DEBUG(strLogInfo);
    bool bConnect = true;
    // 先测试源路径是否正常
    bConnect = testFtpConnection(m_collectSet.ip, m_collectSet.port, m_collectSet.loginUser, m_collectSet.loginPass, m_collectSet.ftp_transferMode, m_collectSet.ftp_connectMode);

    if (!bConnect)
    {
        m_nLineState = 1;
        emit taskState(m_collectSet, 0, m_nLineState);
        QSLOG_DEBUG("test ftp error");
        return;
    }
    QSLOG_DEBUG("test ftp success.");

    // 如果连接是正常的话
    m_nLineState = 0;
    emit taskState(m_collectSet, 0, m_nLineState);

    emit startGif(m_collectSet.dirID, true);
    //m_oDataTransferPro = QSharedPointer<QProcess>(new QProcess());
    QProcess oProcess;
    //setProcess(&oProcess);
    //connect(&oProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
    oProcess.start("DataTransferPro", QStringList() << m_collectSet.dirID);

    // 初始化，设置子进程运行标识

    m_bChildProcessRunning = true;
    connect(&oProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(stoprcv(int, QProcess::ExitStatus)));
    QSLOG_DEBUG("start data transfer pro");
    //oProcess.waitForFinished();
    while (m_bChildProcessRunning)
    {
        QCoreApplication::processEvents();
        QThread::sleep(1);
    }
    //QProcess::execute("DataTransferPro", QStringList() << m_collectSet.dirID);
    emit startGif(m_collectSet.dirID, false);
    QSLOG_DEBUG("finish process.");
    return;

}

void FtpCollector::getNewFiles(const CollectTask &in_oTask)
{
    QSLOG_DEBUG("====");
    m_collectSet = in_oTask;

    m_pCftp = QSharedPointer<CurlFtp>(new CurlFtp(this));
    QObject::connect(m_pCftp.data(), SIGNAL(done()), this, SLOT(ftpDone()));

    m_bRun = (bool)m_collectSet.enable;
    if (!m_bRun)
    {
        return;
    }

    if (!readSet())
    {
        return;
    }

    // modified by liubojun @20171225
    CDirRecord oRecordSet(m_collectSet.dirName);

    if (m_collectSet.recordLatestTime)
    {
        oRecordSet.loadLatestFileSize();
    }

    // E:/workspace/DataTransfer/DataTransfer_code_20170831/vs2013/apps/DataTransfer/%T-1H%t%y/%t%m/%td
    // modified by liubojun. 支持按照特定时间获取数据
    QStringList rootPaths = CPathBuilder::getFinalPathFromUrl(m_collectSet.rltvPath);
    for (int i = 0; i < rootPaths.size(); ++i)
    {
        m_collectSet.rltvPath = rootPaths.at(i);
        m_pCftp->setHostPort(m_collectSet.ip, m_collectSet.port);
        m_pCftp->setUserPwd(m_collectSet.loginUser, m_collectSet.loginPass);
        m_pCftp->setRootPath(m_collectSet.rltvPath);
        m_pCftp->setFtpTransferMode(m_collectSet.ftp_transferMode);
        m_pCftp->setFtpConnectMode(m_collectSet.ftp_connectMode);
        m_pCftp->setSubDirFlag(m_collectSet.subdirFlag);
        m_pCftp->setSubDirTemplateId(m_collectSet.subDirTemplate);

        emit print(QStringLiteral("[%1]: 开始收集任务 %2[%3]").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(m_collectSet.dirName)
                   .arg(m_collectSet.rltvPath));

        bool bConnect = true;
        // 先测试源路径是否正常
        bConnect = testFtpConnection(m_collectSet.ip, m_collectSet.port, m_collectSet.loginUser, m_collectSet.loginPass, m_collectSet.ftp_transferMode, m_collectSet.ftp_connectMode);

        if (!bConnect)
        {
            m_nLineState = 1;
            //emit taskState(m_collectSet, 0, m_nLineState);
            return;
        }

        if (bConnect)
        {
            m_nLineState = 0;
            //emit taskState(m_collectSet, 0, m_nLineState);
            //emit startGif(m_collectSet.dirID, true);
            m_bFinish = false;
            m_fileList.clear();	//清空新文件列表

            m_pCftp->getNewFiles(m_fileList, oRecordSet);
            //emit startGif(m_collectSet.dirID, false);
        }
        else
        {
            m_nLineState = 1;
            emit taskState(m_collectSet, 0, m_nLineState);
        }
    }

    if (m_collectSet.recordLatestTime)
    {
        oRecordSet.reflush();
    }
}

void FtpCollector::ftpDone()
{
    QSLOG_DEBUG(QString("Finished ftp collect, %1 files").arg(m_fileList.size()));
    if (m_fileList.isEmpty() || !m_bRun)
    {
        m_bFinish = true;
        return;
    }

    CurlFtp m_ftp;
    for (int i=0; i<m_fileList.size(); ++i)
    {
        TransTask task;
        if (m_bRun && !compareWithDest(m_ftp, m_fileList.at(i), task))
        {
            task.collectSet = m_collectSet;
            //task.userInfo = m_userInfo.user;
            // 发送文件
            DistributeFile sendFile(this, m_ftp);
            sendFile.transfer(task);
        }
    }

    m_bFinish = true;
}

bool FtpCollector::testCollection()
{
//     QSharedPointer<CurlFtp> pFtp = QSharedPointer<CurlFtp>(new CurlFtp(this));
//     pFtp->setHostPort(m_strIP, m_iPort);
//     pFtp->setUserPwd(m_strUser, m_strPassword);
// 	char url[100] = {0};
// 	char usrpwd[100] = {0};
// 	sprintf(ftpurl, "ftp://%s:%s@%s:%d/%s", m_strUser, m_strPwd, m_strIP, m_nPort, m_strRoot);
// 	sprintf(url, "")
// 		sprintf(usrpwd, "%s:%s", strUsr.c_str(), strPwd.c_str());
//    return pFtp->connectToHost();
    return true;
}

void FtpCollector::taskDone(bool bFlag, const FileInfo &file)
{
    return;
}

bool FtpCollector::compareWithDest(CurlFtp &oCurlFtp, const FileInfo &fi, TransTask &tTask)
{
    // libcurl不能传输大小为0的文件
    if (fi.nFileSize <= 0)
    {
        return true;
    }

    for (int i=0; i<m_tUser.lstUser.size(); ++i)
    {
        CollectUser &cUser = m_tUser.lstUser[i];
        QString strFileFullPath = QString::fromLocal8Bit(fi.strFilePath.c_str());
        QString strFileName = QString::fromLocal8Bit(fi.strFileName.c_str());
        // QString dstFileFullPath = getDestFilePath(strFileFullPath, strFileName, cUser, QDateTime::fromString(fi.strMdyTime.c_str(), "yyyyMMddhhmmss"));

        // modified by liubojun @20171029
        int iTmBaseRule = cUser.user.timebaserule;

        QString dstFileFullPath;

        // 如果基于收集目录时间，需要准确判断当前的日期，因为收集有可能是配置的时间范围
        ///360Downloads/rcv/2017/201710/%T-[0,1]d%t%Y%t%m%t%d <===> /360Downloads/rcv/2017/201710/20171029/123
        QDateTime oDt;
        if (1 == iTmBaseRule) // 基于收集目录时间
        {
            oDt = CPathBuilder::getDateTimeFrom2Urls(m_tUser.colTaskInfo.rltvPath, fi.strFilePath.c_str());
        }
        else if (2 == iTmBaseRule) // 基于文件名时间
        {
            oDt = CPathBuilder::getDateTimeFromFileName(m_tUser.colTaskInfo.fileTemplate, strFileName);
        }
        else   // 基于系统时间 + 不基于任何时间
        {
            oDt = QDateTime::currentDateTime();
        }

        dstFileFullPath = getDestFilePath(strFileFullPath, strFileName, cUser, oDt, iTmBaseRule);

        QString dstFilePath = dstFileFullPath;
        tTask.fileName = strFileName;
        tTask.srcFileFullPath = strFileFullPath;
        tTask.strDestFileName = CChangeName::change_name_by_id(fi.strFileName.c_str(), cUser.rename_rule.toLocal8Bit().toStdString().c_str()).c_str();

        dstFileFullPath += tTask.strDestFileName;
        // 分发到目录
        if (cUser.user.sendType == 0)
        {
            QFile file(dstFileFullPath);
            if (file.exists() && fi.nFileSize == file.size())
            {
                continue;
            }
        }
        // 分发到FTP
        else
        {
            //char ftpUrl[512] = {0};
            //char ftpPath[512] = { 0 };
            //char usrPwd[100] = {0};
            string strIp = cUser.user.ip.toStdString();
            int nPort = cUser.user.port;
            string strPath = dstFileFullPath.toLocal8Bit().data();
            // string strName = tTask.fileName.toLocal8Bit().data();
            string strName = tTask.strDestFileName.toLocal8Bit().data();

            string strUsr = cUser.user.lgUser.toLocal8Bit().data();
            string strPwd = cUser.user.lgPass.toLocal8Bit().data();
            QString ftpUrl = QString("ftp://%1:%2%3").arg(strIp.c_str()).arg(nPort).arg(strPath.c_str());
            //sprintf(ftpUrl, "ftp://%s:%d%s", strIp.c_str(), nPort, strPath.c_str());
            QString usrPwd = QString("%1:%2").arg(strUsr.c_str()).arg(strPwd.c_str());
            //sprintf(usrPwd, "%s:%s", strUsr.c_str(), strPwd.c_str());

            string strDestPath("");
            if (string::npos != dstFileFullPath.lastIndexOf("/"))
            {
                strDestPath = dstFileFullPath.mid(0, dstFileFullPath.lastIndexOf("/") + 1).toLocal8Bit().toStdString();
            }
            else
            {
                QSLOG_ERROR("dstFileFullPath.lastIndexOf ERR");
            }

            QString ftpPath = QString("ftp://%1:%2%3").arg(strIp.c_str()).arg(nPort).arg(strDestPath.c_str());
            //sprintf(ftpPath, "ftp://%s:%d%s", strIp.c_str(), nPort, strDestPath.c_str());
            // CurlFtp m_ftp;
            double dSize = 0;
            if (oCurlFtp.getFileSize(ftpUrl.toLocal8Bit().toStdString().c_str(), usrPwd.toLocal8Bit().toStdString().c_str(), strName, dSize))
            {
                if (fi.nFileSize != (long long)dSize)
                {
                    if (-1 == oCurlFtp.deleteFtpFile(ftpPath.toLocal8Bit().toStdString().c_str(), usrPwd.toLocal8Bit().toStdString().c_str(), strName))
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

        tTask.userInfo = cUser.user;
        //tTask.userInfo.append(cUser.user);
        //tTask.dstFilePath.append(dstFilePath);
        tTask.dstFilePath = dstFilePath;
    }

    // return tTask.userInfo.empty();
    return tTask.userInfo.userID.isEmpty();
}

int FtpCollector::reStart()
{
    return start();
}

void FtpCollector::setProcess(QProcess *in_pro)
{
    m_pro = in_pro;
}

void FtpCollector::readOutput()
{
    QByteArray content = m_pro->readAllStandardOutput();
    emit print(content);
}

