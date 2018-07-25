#include <QCoreApplication>
#include <QUrl>
#include <QUuid>
#include "DistributeFile.h"
#include "ctkPublicFun.h"
#include "ctkLog.h"
#include "pathbuilder.h"
#include "LibCurlFtp.h"
#pragma warning(disable:4996)

const QString g_suffix[4] = { "", ".jm1", ".jm2", ".jm3" };

//DistributeFile::DistributeFile(CollectorBase *pBase, CurlFtp &oCurlFtp)
DistributeFile::DistributeFile(CollectorBase *pBase)
    : m_pBase(pBase)
      //, m_oCurlFtp(oCurlFtp)
{
    if (pBase != NULL)
    {
        QObject::connect(this, SIGNAL(emitLog(const QString &, int)), pBase, SLOT(emitLog(const QString &, int)));

        QObject::connect(this, SIGNAL(emitBroadCast(const QString &, const QString &)), pBase, SLOT(emitBroadCast(const QString &, const QString &)));

    }
    //m_pFtp = QSharedPointer<CurlFtp>(new CurlFtp());
    //if (m_pFtp == NULL)
    //{
    //    QSLOG_ERROR("Create CurlFtp error.");
    //    exit(-1);
    //}
    //m_pFileCrypt = pBase->m_pFileCrypt;

    // m_downloadPath = qApp->applicationDirPath() + "/work/download/" + QString::number(getCurlAddr());
    //m_downloadPath = qApp->applicationDirPath() + "/work/download/" + QUuid::createUuid().toString();
    //m_downloadPath = qApp->applicationDirPath() + "/work/download/" + pBase->m_collectSet.dirName;
    // m_downloadPath = qApp->applicationDirPath() + "/work/download/";
    //taskFinish();
}

DistributeFile::~DistributeFile()
{
    taskFinish();
    //QMutexLocker autoLocker(&m_pManager->m_taskMutex);
    //QSLOG_DEBUG("~DistributeTaskThread");
}

//bool DistributeFile::transfer(TransTask &task, CurlFtp &oCurlFtp)
//{
//    //QSLOG_DEBUG(QString("begin task: %1.").arg(task.srcFileFullPath));
//    m_pCurlFtp = &oCurlFtp;
//
//    QDir qdir(m_downloadPath);
//    if (!qdir.exists())
//    {
//        qdir.mkpath(m_downloadPath);
//    }
//    QString tmpFilePath = m_downloadPath + "/" + task.fileName;
//    struct FileData fileData;
//    // QString转char*
//    QByteArray baTmp = tmpFilePath.toLocal8Bit();
//    fileData.filename = baTmp.data();
//
//    QString strBroadMsgFile;
//    // 1. 获取文件数据
//    if (1 == task.collectSet.collectType)	// ftp收集
//    {
//        string strIP = task.collectSet.ip.toStdString();
//        //char ftpUrl[512] = {0};
//        //char usrPwd[100] = {0};
//        // modified by liubojun@20170928,与收集统一方式
//        QString ftpUrl = QString("ftp://%1:%2%3").arg(strIP.c_str()).arg(task.collectSet.port).arg(task.srcFileFullPath);
//        strBroadMsgFile = ftpUrl;
//        //sprintf(ftpUrl, "ftp://%s:%d%s", strIP.c_str(), task.collectSet.port, task.srcFileFullPath.toLocal8Bit().data());
//        QString usrPwd = QString("%1:%2").arg(task.collectSet.loginUser).arg(task.collectSet.loginPass);
//        //sprintf(usrPwd, "%s:%s", task.collectSet.loginUser.toLocal8Bit().data(), task.collectSet.loginPass.toLocal8Bit().data());
//        m_pCurlFtp->setFtpConnectMode(task.collectSet.ftp_connectMode);
//        m_pCurlFtp->setFtpTransferMode(task.collectSet.ftp_transferMode);
//        if (0 != m_pCurlFtp->downloadFile(ftpUrl.toLocal8Bit().toStdString().c_str(), usrPwd.toLocal8Bit().toStdString().c_str(), &fileData))
//        {
//            QString logInfo = QStringLiteral("从FTP下载文件[%1]失败。").arg(task.fileName);
//            //m_pBase->emitLog(task.collectSet.dirName, logInfo);
//            //m_pBase->emitLog(logInfo, BAD);
//            emit emitLog(logInfo, BAD);
//            return false;
//        }
//    }
//    else	// 共享目录收集
//    {
//        //char url[512] = {0};
//        QString strTmp = task.srcFileFullPath;
//        // !!
//        if (strTmp.length() >= 1 && strTmp.left(1) != "/")
//        {
//            strTmp = "/" + strTmp;
//        }
//        else
//        {
//            QSLOG_ERROR("strTmp.length() is incorrect.");
//        }
//        QString url = QString("file://%1").arg(strTmp);
//        //sprintf(url, "file://%s", strTmp.toLocal8Bit().data());
//        // 直接使用文件拷贝
//        // QUrl ourl(QString::fromLocal8Bit(url));
//        QUrl ourl(url);
//        QString strFile = ourl.toLocalFile();
//        strBroadMsgFile = strFile;
//        QFile f(strFile);
//        qint64 size = f.size();
//        QSLOG_DEBUG(QString("%1").arg(f.size()));
//
//        //if (0 != m_pFtp->downloadFile(url, NULL, &fileData))
//        if (!f.copy(QString::fromLocal8Bit(fileData.filename)))
//        {
//            QString logInfo = QStringLiteral("从源目录下载[%1]失败。").arg(task.srcFileFullPath);
//            //m_pBase->emitLog(task.collectSet.dirName, logInfo);
//            //m_pBase->emitLog(logInfo, BAD);
//            emit emitLog(logInfo, BAD);
//            return false;
//        }
//    }
//
//    bool bRes = false;
//    // modified by liubojun @20180205
//    //if (0 == task.userInfo.size())
//    //{
//    //    QSLOG_DEBUG(QStringLiteral("当前收集任务未配置分发用户"));
//    //}
//    // 遍历分发用户
//    //QSLOG_INFO(QString("user num is %1").arg(QString::number(task.userInfo.size())));
//    //for (int i=0; i<task.userInfo.size(); ++i)
//    {
//        //const UserInfo &user = task.userInfo.at(i);
//        const UserInfo &user = task.userInfo;
//        // 2.加密压缩
//        //int nIndex = 0;
//        //QString strFileFullPath("");
//        //if (!CrypCurtFile(tmpFilePath, user.compress, user.encrypt, nIndex, strFileFullPath))
//        //{
//        //    QString logInfo = QStringLiteral("加密压缩文件[%1]失败。").arg(task.fileName);
//        //    QSLOG_ERROR(logInfo);
//        //    continue;
//        //}
//        //QByteArray barray = strFileFullPath.toLocal8Bit();
//        //if (nIndex != 0)
//        //{
//        //    fileData.filename = barray.data();
//        //}
//        QSLOG_DEBUG(QString::fromLocal8Bit("文件:%1分发到[%2]").arg(fileData.filename).arg(user.userName));
//        // 3.分发数据
//
//        QString strDstFile;
//        if (0 == user.sendType)		//目录分发
//        {
//            // bRes = sendToDir(fileData.filename,  task, i);
//            bRes = sendToDir(fileData.filename, task);
//            strDstFile = task.dstFilePath + task.strDestFileName;
//        }
//        else if (1 == user.sendType)	//ftp分发
//        {
//            //bRes = sendToFtp(fileData.filename, task, i);
//            bRes = sendToFtp(fileData.filename, task);
//            strDstFile = QString("ftp://%1:%2%3%4").
//                         arg(task.userInfo.ip.toStdString().c_str()).
//                         arg(task.userInfo.port).
//                         arg(task.dstFilePath.toLocal8Bit().toStdString().c_str()).
//                         arg(task.strDestFileName);
//        }
//        else
//        {
//            QSLOG_ERROR("Unknow send way.");
//        }
//
//        if (bRes)
//        {
//            QString strInfo = QStringLiteral("文件[%1]发送完成。").arg(task.fileName);
//            //m_pBase->emitLog(task.collectSet.dirName, strInfo);
//            //m_pBase->emitLog(strInfo, GOOD);
//            emit emitLog(strInfo, GOOD);
//            emit emitBroadCast(strBroadMsgFile, strDstFile);
//
//        }
//    }
//
//
//    QSLOG_DEBUG(QString("finish task: %1.").arg(task.srcFileFullPath));
//    return bRes;
//}

bool DistributeFile::transfer(TransTask &task, QSharedPointer<FtpBase> &pFtpSource, QSharedPointer<FtpBase> &pFtpDest)
{
    //m_pCurlFtp = &oCurlFtp;
    QSLOG_DEBUG(QString::fromLocal8Bit("===开始传输文件:%1===").arg(task.srcFileFullPath));
    m_downloadPath = qApp->applicationDirPath() + "/work/download/" + m_pBase->m_collectSet.dirName;
    QDir qdir(m_downloadPath);
    if (!qdir.exists())
    {
        qdir.mkpath(m_downloadPath);
    }
    QString tmpFilePath = m_downloadPath + "/" + task.fileName;
    struct FileData fileData;
    // QString转char*
    QByteArray baTmp = tmpFilePath.toLocal8Bit();
    fileData.filename = baTmp.data();

    QString strBroadMsgFile;
    // 1. 获取文件数据
    if (0 == task.collectSet.collectType)
    {
        QString strTmp = task.srcFileFullPath;
        // !!
        if (strTmp.length() >= 1 && strTmp.left(1) != "/")
        {
            strTmp = "/" + strTmp;
        }
        else
        {
            QSLOG_ERROR("strTmp.length() is incorrect.");
        }
        QString url = QString("file://%1").arg(strTmp);
        //sprintf(url, "file://%s", strTmp.toLocal8Bit().data());
        // 直接使用文件拷贝
        // QUrl ourl(QString::fromLocal8Bit(url));
        QUrl ourl(url);
        QString strFile = ourl.toLocalFile();
        strBroadMsgFile = strFile;
        QFile f(strFile);
        qint64 size = f.size();
        //QSLOG_DEBUG(QString("%1").arg(f.size()));

        if (!f.copy(QString::fromLocal8Bit(fileData.filename)))
        {
            QString logInfo = QStringLiteral("从源目录下载[%1]失败。").arg(task.srcFileFullPath);
            emit emitLog(logInfo, BAD);
            return false;
        }
    }
    else
    {
        if (1 == task.collectSet.collectType)	// ftp收集
        {
            if (pFtpSource.isNull())
            {
                pFtpSource = QSharedPointer<FtpBase>(new CFtp());
            }

        }
        else // sftp 收集
        {
            if (pFtpSource.isNull())
            {
                pFtpSource = QSharedPointer<FtpBase>(new SFtp());
            }
        }
        pFtpSource->setTransferMode(task.collectSet.ftp_connectMode == 0 ? Passive : Active);
        pFtpSource->connectToHost(task.collectSet.ip, task.collectSet.port, task.collectSet.loginUser, task.collectSet.loginPass);

        if (CURLE_OK != pFtpSource->get(task.srcFileFullPath, tmpFilePath, task.collectSet.ftp_transferMode == 0 ? Binary : Ascii))
        {
            QString logInfo = QStringLiteral("从FTP下载文件[%1]失败。").arg(task.fileName);
            emit emitLog(logInfo, BAD);
            return false;
        }

		strBroadMsgFile = pFtpSource->makeUrl2(task.srcFileFullPath);
    }

    bool bRes = false;
    const UserInfo &user = task.userInfo;

    QSLOG_DEBUG(QString::fromLocal8Bit("文件:%1分发到[%2]").arg(QString::fromLocal8Bit(fileData.filename)).arg(user.userName));
    // 3.分发数据

    QString strDstFile;
    if (0 == user.sendType)		//目录分发
    {
        //bRes = sendToDir(fileData.filename, task);
        //strDstFile = task.dstFilePath + task.strDestFileName;

        QString strDestFileFullPath(task.dstFilePath + task.strDestFileName);
		strDstFile = strDestFileFullPath;
        QDir oDir(task.dstFilePath);
        if (!oDir.exists())
        {
            if (!oDir.mkpath(task.dstFilePath))
            {
                QString logInfo = QStringLiteral("创建分发目录%1失败。").arg(task.dstFilePath);
                emit emitLog(logInfo, BAD);
                return false;
            }
        }
        QFile sourcefile(tmpFilePath);
        QFile destfile(strDestFileFullPath + task.userInfo.sendSuffix);
        bool bNeedRename = false;
        bool bNeedCopy = false;
        if (destfile.exists())
        {
            if (destfile.size() != sourcefile.size())
            {
                destfile.remove();
                bNeedCopy = true;
            }
            else
            {
                bNeedRename = true;
            }

        }
        else
        {
            bNeedRename = true;
            bNeedCopy = true;
        }

        if (bNeedCopy && !sourcefile.copy(strDestFileFullPath + task.userInfo.sendSuffix))
        {
            QString logInfo = QStringLiteral("文件[%1]上传到%2失败").arg(task.fileName).arg(task.dstFilePath);

            QSLOG_ERROR(logInfo + ",reason:" + sourcefile.errorString());
            emit emitLog(logInfo, BAD);
            return false;
        }

        if (bNeedRename && !task.userInfo.sendSuffix.trimmed().isEmpty())
        {
			QFile destFileFinal(strDestFileFullPath);
			if (destFileFinal.exists())
			{
				destFileFinal.remove();
			}
            QFile destFile(strDestFileFullPath + task.userInfo.sendSuffix);
            if (!destFile.rename(strDestFileFullPath))
            {
                QString logInfo = QStringLiteral("文件[%1]重命名失败。").arg(task.fileName);
                emit emitLog(logInfo, BAD);
                QSLOG_ERROR(logInfo + ",reason:" + sourcefile.errorString());
                return false;
            }
        }


    }
    else
    {
        if (1 == user.sendType) // ftp
        {
            if (pFtpDest.isNull())
            {
                pFtpDest = QSharedPointer<FtpBase>(new CFtp());
            }
        }
        else
        {
            if (pFtpDest.isNull()) // sftp
            {
                pFtpDest = QSharedPointer<FtpBase>(new SFtp());
            }
        }

        pFtpDest->setTransferMode(task.userInfo.ftpTransferMode == 0 ? Passive : Active);
        pFtpDest->connectToHost(task.userInfo.ip, task.userInfo.port, task.userInfo.lgUser, task.userInfo.lgPass);

        if (CURLE_OK != pFtpDest->put(tmpFilePath, task.dstFilePath + task.strDestFileName, task.userInfo.sendSuffix, task.userInfo.ftpTransferType == 0 ? Binary : Ascii))
        {
            QString logInfo = QStringLiteral("文件[%1]上传到ftp失败。").arg(task.fileName);
            emit emitLog(logInfo, BAD);
            return false;
        }

		strDstFile = pFtpDest->makeUrl2(task.dstFilePath + task.strDestFileName);
    }

    QString strInfo = QStringLiteral("文件[%1]发送完成。").arg(task.fileName);
    emit emitLog(strInfo, GOOD);
    emit emitBroadCast(strBroadMsgFile, strDstFile);

    QSLOG_DEBUG(QString("finish task: %1.").arg(task.srcFileFullPath));
    return true;
}

// 加密压缩二进制流
//bool DistributeTaskThread::CrypCurtFile(bool bComp, bool bCrypt, int &nIndex)
//{
//    if (m_pFileCrypt == NULL)
//    {
//        QSLOG_DEBUG("m_pFileCrypt is nulll.");
//        return false;
//    }
//    nIndex = (int)bCrypt + (int)bComp*2;
//    if ((m_mData[nIndex].memdata != NULL) || (nIndex == 0))
//    {
//        return true;
//    }
//
//    QByteArray baOrg, baResult;
//    KeyIv &keyiv = m_keyiv[nIndex];
//    keyiv.bComp = bComp;
//    keyiv.bCrypt = bCrypt;
//
//    baOrg.append(m_mData[0].memdata, m_mData[0].size);
//    if (!m_pFileCrypt->CompCryptFile(baOrg, baResult, keyiv, bComp, bCrypt))
//    {
//        return false;
//    }
//
//    m_mData[nIndex].size = baResult.size();
//    m_mData[nIndex].memdata = (char *)malloc(m_mData[nIndex].size);
//    memcpy(m_mData[nIndex].memdata, baResult.data(), baResult.size());
//
//    return true;
//}

// 加密压缩落盘文件
bool DistributeFile::CrypCurtFile(const QString &srcFilePath, bool bComp, bool bCrypt, int &nIndex, QString &dstFilePath)
{
    //nIndex = (int)bCrypt + (int)bComp*2;
    //dstFilePath = srcFilePath + g_suffix[nIndex];
    //if (nIndex == 0)
    //{
    //    return true;
    //}

    //QFile qfile(dstFilePath);
    //if (qfile.exists())
    //{
    //    return true;
    //}

    //KeyIv &keyiv = m_keyiv[nIndex];
    //keyiv.bComp = bComp;
    //keyiv.bCrypt = bCrypt;

    //if (m_pFileCrypt == NULL)
    //{
    //    QSLOG_ERROR(QStringLiteral("获取加密实例失败."));
    //    return false;
    //}

    ////if (!m_pFileCrypt->CompCryptFile(srcFilePath, dstFilePath, keyiv))
    ////{
    ////    return false;
    ////}

    return true;
}

QString DistributeFile::getHostFromFileName(const QString& r_strFullPathFileName)
{
    string t_file = r_strFullPathFileName.toStdString();
    size_t t_index = t_file.find('/',2);
    if (t_index == string::npos)
        return "";
    QString ret="\\\\";
    ret += t_file.substr(2, t_index - 2).c_str();
    return ret;
}


//bool DistributeFile::sendToDir(const char *fullPath, TransTask &taskInfo)
//{
//
//    if (fullPath == NULL)
//    {
//        return false;
//    }
//
//    QString strSendPath = taskInfo.dstFilePath;//.at(userIndex);
//    QDir qDir;
//    if (!qDir.exists(strSendPath))
//    {
//        qDir.mkpath(strSendPath);
//    }
//    if (strSendPath.length() >= 1 && strSendPath.right(1) != "/")
//    {
//        strSendPath += "/";
//    }
//
//    // 目标文件路径
//    QString strFullPath = strSendPath + taskInfo.strDestFileName;
//    // 目标临时文件路径
//    QString strTmpName = strFullPath + taskInfo.userInfo.sendSuffix;//taskInfo.userInfo.at(userIndex).sendSuffix;
//
//    if (strSendPath.length() >= 1 && strSendPath.left(1) != "/")
//    {
//        strSendPath = "/" + strSendPath;
//    }
//
//    QString url = QString::fromLocal8Bit("file://%1").arg(strSendPath);
//
//    int bRet = -1;
//
//    if (taskInfo.userInfo.conput)
//    {
//        bRet = m_pCurlFtp->conputFileToDir(url.toLocal8Bit().toStdString().c_str(), NULL,
//                                           taskInfo.strDestFileName.toLocal8Bit().data(), fullPath,
//                                           // taskInfo.userInfo.at(userIndex).sendSuffix.toAscii().data());
//                                           taskInfo.userInfo.sendSuffix.toAscii().data());
//    }
//    else
//    {
//        bRet = m_pCurlFtp->uploadFileToDir(url.toLocal8Bit().toStdString().c_str(), NULL,
//                                           taskInfo.strDestFileName.toLocal8Bit().data(), fullPath,
//                                           // taskInfo.userInfo.at(userIndex).sendSuffix.toAscii().data());
//                                           taskInfo.userInfo.sendSuffix.toAscii().data());
//    }
//    if (bRet != 0)
//    {
//        QSLOG_ERROR(QStringLiteral("上传到目录失败。"));
//        //m_pManager->dealError(m_fileInfo, QStringLiteral("上传到目录失败。"));
//        return false;
//    }
//
//    bool res = false;
//    // 删除原来文件
//    if (QFile::exists(strFullPath))
//    {
//        res = QFile::remove(strFullPath);
//    }
//    // 重命名
//    QFile w_file(strTmpName);
//    if (!w_file.open(QIODevice::ReadOnly))
//    {
//        QSLOG_ERROR("Fail to open device.");
//        return false;
//    }
//
//    res = w_file.rename(strFullPath);
//    if (!res)
//    {
//        QSLOG_ERROR("rename fail");
//        return false;
//    }
//
//    // 发送key和iv
//    //if (taskInfo.userInfo.at(userIndex).compress || taskInfo.userInfo.at(userIndex).encrypt)
//    //{
//    //    strFullPath += ".key";
//    //    writeKeyIv(strFullPath, keyiv);
//    //}
//
//    //QSLOG_INFO("bool DistributeFile::sendToDir finish");
//    return true;
//}


//bool DistributeFile::writeKeyIv(const QString &fileName, KeyIv &keyiv)
//{
//    QString strTmpName = fileName + ".tmp";
//    QFile wFile(strTmpName);
//    if (!wFile.open(QIODevice::WriteOnly))
//    {
//        QSLOG_ERROR("Fail to writeKeyIv.");
//        return false;
//    }
//
//    QDataStream out(&wFile);
//    out << keyiv.bComp;
//    out << keyiv.bCrypt;
//    out << keyiv.key;
//    out << keyiv.iv;
//    wFile.close();
//
//    // 删除原来key文件
//    if (QFile::exists(fileName))
//    {
//        QFile::remove(fileName);
//    }
//
//    return wFile.rename(fileName);
//}

//
//bool DistributeFile::sendToFtp(const char *fullPath, TransTask &task)
//{
//    //string strIp =  task.userInfo.at(userIndex).ip.toStdString();
//    //int nPort = task.userInfo.at(userIndex).port;
//    //string strUsr = task.userInfo.at(userIndex).lgUser.toLocal8Bit().toStdString();
//    //string strPwd = task.userInfo.at(userIndex).lgPass.toLocal8Bit().toStdString();
//    //// 相对路径
//    //string strPath = task.dstFilePath.at(userIndex).toLocal8Bit().toStdString();
//    string strIp = task.userInfo.ip.toStdString();
//    int nPort = task.userInfo.port;
//    string strUsr = task.userInfo.lgUser.toLocal8Bit().toStdString();
//    string strPwd = task.userInfo.lgPass.toLocal8Bit().toStdString();
//    // 相对路径
//    string strPath = task.dstFilePath.toLocal8Bit().toStdString();
//    //char ftpUrl[512] = {0};
//    //char usrPwd[100] = {0};
//    QString ftpUrl = QString("ftp://%1:%2%3").arg(strIp.c_str()).arg(nPort).arg(strPath.c_str());
//    //sprintf(ftpUrl, "ftp://%s:%d%s", strIp.c_str(), nPort, strPath.c_str());
//    QString usrPwd = QString("%1:%2").arg(strUsr.c_str()).arg(strPwd.c_str());
//    //sprintf(usrPwd, "%s:%s", strUsr.c_str(), strPwd.c_str());
//
//    int bRet = -1;
//    // if (task.userInfo.at(userIndex).conput)	// 断点续传
//    if (task.userInfo.conput)	// 断点续传
//    {
//        bRet = m_pCurlFtp->conputFileToFtp(ftpUrl.toLocal8Bit().toStdString().c_str(),
//                                           usrPwd.toLocal8Bit().toStdString().c_str()
//                                           , task.strDestFileName.toLocal8Bit().toStdString().c_str(),
//                                           // fullPath, task.userInfo.at(userIndex).sendSuffix.toStdString().c_str());
//                                           fullPath, task.userInfo.sendSuffix.toStdString().c_str());
//    }
//    else
//    {
//        bRet = m_pCurlFtp->uploadFileToFtp(ftpUrl.toLocal8Bit().toStdString().c_str(),
//                                           usrPwd.toLocal8Bit().toStdString().c_str(),
//                                           task.strDestFileName.toLocal8Bit().toStdString().c_str(),
//                                           // fullPath, task.userInfo.at(userIndex).sendSuffix.toStdString().c_str());
//                                           fullPath, task.userInfo.sendSuffix.toStdString().c_str(), task.userInfo.ftpTransferMode);
//    }
//    if (bRet != 0)
//    {
//        QString logInfo = QStringLiteral("文件[%1]上传到ftp失败。").arg(task.fileName);
//        //m_pBase->emitLog(task.collectSet.dirName, logInfo);
//        //m_pBase->emitLog(logInfo, BAD);
//        emit emitLog(logInfo, BAD);
//        return false;
//    }
//
//    // 发送key和iv
//    //if (task.userInfo.at(userIndex).compress || task.userInfo.at(userIndex).encrypt)
//    //{
//    //    string strKeyIv = task.fileName.toStdString() + ".key";
//    //    QByteArray baKey;
//    //    QDataStream out(&baKey, QIODevice::WriteOnly);
//    //    out << keyiv.bComp;
//    //    out << keyiv.bCrypt;
//    //    out << keyiv.key;
//    //    out << keyiv.iv;
//
//    //    // 从内存上传到FTP
//    //    MemoryData mkey;
//    //    mkey.size = baKey.size();
//    //    mkey.memdata = (char *)malloc(mkey.size + 1);
//    //    memcpy(mkey.memdata, baKey.data(), mkey.size);
//    //    mkey.memdata[mkey.size] = 0;
//    //    if (m_pFtp->uploadFile(ftpUrl, usrPwd, &mkey, strKeyIv, task.userInfo.at(userIndex).sendSuffix.toAscii().data()) != 0)
//    //    {
//    //        QSLOG_ERROR(QString("Fail to upload %1.").arg(QString::fromStdString(strKeyIv)));
//    //        //return false;
//    //    }
//    //}
//
//    return true;
//}

int DistributeFile::getCurlAddr()
{
    // return (int)m_pFtp->m_pCurl;
    // 暂时注释
    return -1;
}

void DistributeFile::taskFinish()
{
    // 删除本地临时文件
    QDir qdir(m_downloadPath);
    qdir.removeRecursively();
}
