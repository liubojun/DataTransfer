﻿#include "sftpCollector.h"
#include "pathbuilder.h"
#include "ctkLog.h"
#include "publicthread.h"
#include "DistributeFile.h"
#include "change_name.h"
#include "curlftp.h"
#include "record.h"
#include "LibCurlSFtp.h"

#include <QElapsedTimer>
#include <QCoreApplication>
#include <QProcess>
#include <QLockFile>
#include <QTime>


SFtpCollector::SFtpCollector(QWaitCondition &in_oCond, QMutex &in_oLocker, int &in_iLogsize)
    : CollectorBase(in_oCond, in_oLocker, in_iLogsize)
{
    m_bBeingDeleted = false;
}

SFtpCollector::~SFtpCollector()
{

}

int SFtpCollector::start()
{
    m_bRun = true;
    return 0;
}

int SFtpCollector::stop()
{
    m_bRun = false;

    return 0;
}

void SFtpCollector::deleteSelf()
{
    m_bBeingDeleted = true;
}

void SFtpCollector::stoprcv(int exitcode, QProcess::ExitStatus status)
{
    m_bChildProcessRunning = false;
}

void SFtpCollector::getNewFiles()
{
    if (!m_oTaskLocker.tryLock())
    {
        return;
    }

    QSharedPointer<QMutex> autoUnlock(&m_oTaskLocker, &QMutex::unlock);
    if (!readSet())
    {
        QSLOG_ERROR("query database error.");
        m_bFinish = true;
        return;
    }

    m_bRun = (bool)m_collectSet.enable;
    if (!m_bRun)
    {
        QSLOG_DEBUG("task is stopped.");
        return;
    }

    ///////////////////////////////////////////////////////////////
    // 使用共享内存，查看当前的QProcess是否存在，如果存在的话，则无需再次启动针对该目录ID的收集任务
    if (!checkProcessFinished(m_collectSet.dirID))
    {
        QSLOG_DEBUG("Last ftp collect isn't finished.");
        return;
    }

    QString strLogInfo(QStringLiteral("开始收集任务 %1[%2]").arg(m_collectSet.dirName).arg(m_collectSet.rltvPath));
    emit print(strLogInfo);
    QSLOG_DEBUG(strLogInfo);
    bool bConnect = true;

	// 先测试源路径是否正常
	SFtp oSFtp;
	oSFtp.setTransferMode(m_collectSet.ftp_connectMode == 0 ? Passive : Active);
	oSFtp.connectToHost(m_collectSet.ip, m_collectSet.port);
	if (CURLcode::CURLE_OK != oSFtp.login(m_collectSet.loginUser, m_collectSet.loginPass))
	{
		bConnect = false;
	}
    // 先测试源路径是否正常
    //bConnect = testFtpConnection(m_collectSet.ip, m_collectSet.port, m_collectSet.loginUser, m_collectSet.loginPass, m_collectSet.ftp_transferMode, m_collectSet.ftp_connectMode);

    if (!bConnect)
    {
        m_nLineState = 1;
        emit taskState(m_collectSet, 0, m_nLineState);
        QSLOG_DEBUG("test sftp error");
        return;
    }

    // 如果连接是正常的话
    m_nLineState = 0;
    emit taskState(m_collectSet, 0, m_nLineState);

    emit startGif(m_collectSet.dirID, true);

    QProcess oProcess;

    oProcess.start("DataTransferPro", QStringList() << m_collectSet.dirID);

    // 初始化，设置子进程运行标识

    m_bChildProcessRunning = true;
    connect(&oProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(stoprcv(int, QProcess::ExitStatus)));
    QSLOG_DEBUG("start data transfer pro");

    while (m_bChildProcessRunning)
    {
        QCoreApplication::processEvents();
        QThread::sleep(1);
        // added by liubojun @20180620
        if (!m_bRun)
        {
            oProcess.kill();
            QSLOG_DEBUG("task is stopped.");
            break;
        }
    }

    emit startGif(m_collectSet.dirID, false);
    QSLOG_DEBUG("finish process.");
    if (m_bBeingDeleted)
    {
        if (m_bChildProcessRunning)
        {
            oProcess.kill();
            QSLOG_DEBUG("kill child process.");
        }
        deleteLater();
    }
    return;

}

void SFtpCollector::getNewFiles(const CollectTask &in_oTask)
{
    QSLOG_DEBUG("====");
    m_collectSet = in_oTask;

    //m_pCftp = QSharedPointer<CurlFtp>(new CurlFtp(this));
    //QObject::connect(m_pCftp.data(), SIGNAL(done()), this, SLOT(ftpDone()));

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
	CSubDirFilter m_oSubDirFilter;
	m_oSubDirFilter.init();

    // E:/workspace/DataTransfer/DataTransfer_code_20170831/vs2013/apps/DataTransfer/%T-1H%t%y/%t%m/%td
    // modified by liubojun. 支持按照特定时间获取数据
    QStringList rootPaths = CPathBuilder::getFinalPathFromUrl(m_collectSet.rltvPath);
    for (int i = 0; i < rootPaths.size(); ++i)
    {
        m_collectSet.rltvPath = rootPaths.at(i);
        //m_pCftp->setHostPort(m_collectSet.ip, m_collectSet.port);
        //m_pCftp->setUserPwd(m_collectSet.loginUser, m_collectSet.loginPass);
        //m_pCftp->setRootPath(m_collectSet.rltvPath);
        //m_pCftp->setFtpTransferMode(m_collectSet.ftp_transferMode);
        //m_pCftp->setFtpConnectMode(m_collectSet.ftp_connectMode);
        //m_pCftp->setSubDirFlag(m_collectSet.subdirFlag);
        //m_pCftp->setSubDirTemplateId(m_collectSet.subDirTemplate);

        emit print(QStringLiteral("[%1]: 开始收集任务 %2[%3]").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(m_collectSet.dirName)
                   .arg(m_collectSet.rltvPath));

        bool bConnect = true;
		// 先测试源路径是否正常
		SFtp oSFtp;
		oSFtp.setTransferMode(m_collectSet.ftp_connectMode == 0 ? Passive : Active);
		oSFtp.connectToHost(m_collectSet.ip, m_collectSet.port);
		oSFtp.login(m_collectSet.loginUser, m_collectSet.loginPass);
		if (CURLcode::CURLE_OK != oSFtp.cd(m_collectSet.rltvPath))
		{
			bConnect = false;
		}

        if (!bConnect)
        {
            m_nLineState = 1;
            emit taskState(m_collectSet, 0, m_nLineState);
            return;
        }

        if (bConnect)
        {
            m_nLineState = 0;
            emit taskState(m_collectSet, 0, m_nLineState);
            emit startGif(m_collectSet.dirID, true);
            m_bFinish = false;

			// 存储所有的文件和目录
			QList<CFileInfo> oFiles;

			// 存储所有的目录
			QList<QString> oDirs;
			oDirs.append(m_collectSet.rltvPath);
			int level = 1;
			while (!oDirs.isEmpty())
			{
				QString strCurDir = oDirs.takeFirst();
				if (strCurDir.length() >= 1 && strCurDir.right(1) != "/")
				{
					strCurDir += "/";
				}

				QList<CFileInfo> retFiles = oSFtp.list(strCurDir);
				foreach(const CFileInfo &fi, retFiles)
				{
					if (fi.type == FTP_DIR && m_collectSet.subdirFlag)
					{
						// 在此处加入过滤条件，支持针对子目录的模糊匹配
						if (!m_oSubDirFilter.match(m_collectSet.subDirTemplate, fi.name, level))
						{
							continue;
						}
						oDirs.append(fi.path);
					}
					else if (fi.type == FTP_FILE)
					{
						if (m_collectSet.recordLatestTime)
						{
							oRecordSet.updateLatestFileSize(strCurDir, fi.name, fi.time.toString("yyyyMMddhhmmss"), fi.size);

							if (!oRecordSet.checkIsNewFile(strCurDir, fi.name, fi.time.toString("yyyyMMddhhmmss"), fi.size))
							{
								continue;
							}
						}
						if (!filterFileName(fi.name))
						{
							continue;
						}
						oFiles.append(fi);

					}
				}
				level++;
			}
			//qSort(oFiles.begin(), oFiles.end(), compareCFileInfo);


			ftpDone(oFiles, oRecordSet);
            emit startGif(m_collectSet.dirID, false);
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

void SFtpCollector::ftpDone(const QList<CFileInfo> &files, CDirRecord &io_record)
{
	QSLOG_DEBUG(QString::fromLocal8Bit("本次收集共获取到新文件:%1").arg(files.size()));
	if (files.isEmpty() || !m_bRun)
	{
		return;
	}

    QTime oTimer;
    oTimer.start();

	SFtp oSFtpSource;
	SFtp oSFtpDestTemp;
	SFtp oSFtpDestSend;
	for (int i = 0; i<files.size(); ++i)
    {
        TransTask task;
		if (m_bRun && !compareWithDest(oSFtpDestTemp, files.at(i), task))
        {
            task.collectSet = m_collectSet;
            //task.userInfo = m_userInfo.user;
            // 发送文件
            DistributeFile sendFile(this);

			// 解决问题：当启用了记录收集时间时，当分发失败时，第二次无法重新分发
			if (!sendFile.transfer(task, oSFtpSource, oSFtpDestSend) && m_collectSet.recordLatestTime)
			{
				io_record.updateSendFailure(task.srcFileFullPath.mid(0, task.srcFileFullPath.lastIndexOf("/")+1), task.fileName);
			}
        }
    }
	m_bFinish = true;
    QSLOG_DEBUG(QString::fromLocal8Bit("本次分发共用时:%1秒").arg(oTimer.elapsed()/1000));
}

bool SFtpCollector::testCollection()
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

void SFtpCollector::taskDone(bool bFlag, const FileInfo &file)
{
    return;
}

bool SFtpCollector::compareWithDest(SFtp &oCurlFtp, const CFileInfo &fi, TransTask &tTask)
{
    // libcurl不能传输大小为0的文件
    if (fi.size <= 0)
    {
        return true;
    }

    for (int i=0; i<m_tUser.lstUser.size(); ++i)
    {
        CollectUser &cUser = m_tUser.lstUser[i];
		QString strFileFullPath = fi.path;
		QString strFileName = fi.name;
        // QString dstFileFullPath = getDestFilePath(strFileFullPath, strFileName, cUser, QDateTime::fromString(fi.strMdyTime.c_str(), "yyyyMMddhhmmss"));

        // modified by liubojun @20171029
        int iTmBaseRule = cUser.user.timebaserule;

        QString dstFileFullPath;

        // 如果基于收集目录时间，需要准确判断当前的日期，因为收集有可能是配置的时间范围
        ///360Downloads/rcv/2017/201710/%T-[0,1]d%t%Y%t%m%t%d <===> /360Downloads/rcv/2017/201710/20171029/123
        QDateTime oDt;
        if (1 == iTmBaseRule) // 基于收集目录时间
        {
			oDt = CPathBuilder::getDateTimeFrom2Urls(m_tUser.colTaskInfo.rltvPath, fi.path.toLocal8Bit().toStdString().c_str());
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
        tTask.strDestFileName = CChangeName::change_name_by_id(fi.name.toLocal8Bit().toStdString().c_str(), cUser.rename_rule.toLocal8Bit().toStdString().c_str()).c_str();

        dstFileFullPath += tTask.strDestFileName;
        // 分发到目录
        if (cUser.user.sendType == 0)
        {
            QFile file(dstFileFullPath);
            if (file.exists() && fi.size == file.size())
            {
                continue;
            }
        }
        // 分发到FTP
		else if (cUser.user.sendType == 1)
        {
          
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

            //QString ftpPath = QString("ftp://%1:%2%3").arg(strIp.c_str()).arg(nPort).arg(strDestPath.c_str());
            //sprintf(ftpPath, "ftp://%s:%d%s", strIp.c_str(), nPort, strDestPath.c_str());
            // CurlFtp m_ftp;
            double dSize = 0;
			if (dSize = oCurlFtp.getFileSize(fi.path))
            {
                if (fi.size != (long long)dSize)
                {
					if (-1 == oCurlFtp.remove(fi.path))
                    {
                        continue;
                    }
                }
                else
                {
                    continue;
                }
            }
        }
		else
		{
			oCurlFtp.connectToHost(cUser.user.ip, cUser.user.port, cUser.user.lgUser, cUser.user.lgPass);
			oCurlFtp.setTransferMode(cUser.user.ftpTransferMode == 0 ? Passive : Active);
			//if (CURLE_OK != oCurlFtp.login(cUser.user.lgUser, cUser.user.lgPass))
			//{
			//	continue;
			//}
			//else
			{
				long long dSize = (long long)oCurlFtp.getFileSize(dstFileFullPath);
				if (-1 != dSize)
				{
					if (fi.size != dSize)
					{
						if (-1 == oCurlFtp.remove(fi.path))
						{
							continue;
						}
					}
					else
					{
						continue;
					}

				}

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

int SFtpCollector::reStart()
{
    return start();
}

void SFtpCollector::setProcess(QProcess *in_pro)
{
    m_pro = in_pro;
}

void SFtpCollector::readOutput()
{
    QByteArray content = m_pro->readAllStandardOutput();
    emit print(content);
}
