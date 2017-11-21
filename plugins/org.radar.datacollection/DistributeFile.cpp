#include <QCoreApplication>
#include <QUrl>
#include "DistributeFile.h"
#include "ctkPublicFun.h"
#include "ctkLog.h"
#include "pathbuilder.h"
#pragma warning(disable:4996)

const QString g_suffix[4] = { "", ".jm1", ".jm2", ".jm3" };

DistributeFile::DistributeFile(CollectorBase *pBase, CurlFtp &oCurlFtp)
    : m_pFileCrypt(NULL)
    , m_pBase(pBase)
    , m_oCurlFtp(oCurlFtp)
{
    //m_pFtp = QSharedPointer<CurlFtp>(new CurlFtp());
    //if (m_pFtp == NULL)
    //{
    //    QSLOG_ERROR("Create CurlFtp error.");
    //    exit(-1);
    //}
    m_pFileCrypt = pBase->m_pFileCrypt;

    // m_downloadPath = qApp->applicationDirPath() + "/work/download/" + QString::number(getCurlAddr());
    m_downloadPath = qApp->applicationDirPath() + "/work/download/" + QUuid::createUuid().toString();
    // m_downloadPath = qApp->applicationDirPath() + "/work/download/";
    taskFinish();
}

DistributeFile::~DistributeFile()
{
    taskFinish();
    //QMutexLocker autoLocker(&m_pManager->m_taskMutex);
    //QSLOG_DEBUG("~DistributeTaskThread");
}

void DistributeFile::transfer(TransTask &task)
{
    //QSLOG_DEBUG(QString("begin task: %1.").arg(task.srcFileFullPath));

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

    // 1. 获取文件数据
    if (1 == task.collectSet.collectType)	// ftp收集
    {
        string strIP = task.collectSet.ip.toStdString();
        //char ftpUrl[512] = {0};
        //char usrPwd[100] = {0};
        // modified by liubojun@20170928,与收集统一方式
        QString ftpUrl = QString("ftp://%1:%2%3").arg(strIP.c_str()).arg(task.collectSet.port).arg(task.srcFileFullPath);
        //sprintf(ftpUrl, "ftp://%s:%d%s", strIP.c_str(), task.collectSet.port, task.srcFileFullPath.toLocal8Bit().data());
        QString usrPwd = QString("%1:%2").arg(task.collectSet.loginUser).arg(task.collectSet.loginPass);
        //sprintf(usrPwd, "%s:%s", task.collectSet.loginUser.toLocal8Bit().data(), task.collectSet.loginPass.toLocal8Bit().data());
        m_oCurlFtp.setFtpConnectMode(task.collectSet.ftp_connectMode);
        m_oCurlFtp.setFtpTransferMode(task.collectSet.ftp_transferMode);
        if (0 != m_oCurlFtp.downloadFile(ftpUrl.toLocal8Bit().toStdString().c_str(), usrPwd.toLocal8Bit().toStdString().c_str(), &fileData))
        {
            QString logInfo = QStringLiteral("从FTP下载文件[%1]失败。").arg(task.fileName);
            //m_pBase->emitLog(task.collectSet.dirName, logInfo);
            m_pBase->emitLog(logInfo, FAIL);
            return;
        }
    }
    else	// 共享目录收集
    {
        //char url[512] = {0};
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
        QFile f(strFile);
        qint64 size = f.size();
        QSLOG_DEBUG(QString("%1").arg(f.size()));

        //if (0 != m_pFtp->downloadFile(url, NULL, &fileData))
        if (!f.copy(QString::fromLocal8Bit(fileData.filename)))
        {
            QString logInfo = QStringLiteral("从源目录下载[%1]失败。").arg(task.srcFileFullPath);
            //m_pBase->emitLog(task.collectSet.dirName, logInfo);
            m_pBase->emitLog(logInfo, FAIL);
            return;
        }
    }

    if (0 == task.userInfo.size())
    {
        QSLOG_DEBUG(QStringLiteral("当前收集任务未配置分发用户"));
    }
    // 遍历分发用户
    //QSLOG_INFO(QString("user num is %1").arg(QString::number(task.userInfo.size())));
    for (int i=0; i<task.userInfo.size(); ++i)
    {
        const UserInfo &user = task.userInfo.at(i);

        // 2.加密压缩
        int nIndex = 0;
        QString strFileFullPath("");
        //if (!CrypCurtFile(tmpFilePath, user.compress, user.encrypt, nIndex, strFileFullPath))
        //{
        //    QString logInfo = QStringLiteral("加密压缩文件[%1]失败。").arg(task.fileName);
        //    QSLOG_ERROR(logInfo);
        //    continue;
        //}
        QByteArray barray = strFileFullPath.toLocal8Bit();
        if (nIndex != 0)
        {
            fileData.filename = barray.data();
        }
        QSLOG_DEBUG(QString::fromLocal8Bit("文件:%1分发到[%2]").arg(fileData.filename).arg(user.userName));
        // 3.分发数据
        bool bRes = false;
        if (0 == user.sendType)		//目录分发
        {
            bRes = sendToDir(fileData.filename, m_keyiv[nIndex], task, i);
        }
        else if (1 == user.sendType)	//ftp分发
        {
            bRes = sendToFtp(fileData.filename, m_keyiv[nIndex], task, i);
        }
        else
        {
            QSLOG_ERROR("Unknow send way.");
        }

        if (bRes)
        {
            QString strInfo = QStringLiteral("文件[%1]发送完成。").arg(task.fileName);
            //m_pBase->emitLog(task.collectSet.dirName, strInfo);
            m_pBase->emitLog(strInfo, SUCCESS);
        }
    }


    QSLOG_DEBUG(QString("finish task: %1.").arg(task.srcFileFullPath));
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
    nIndex = (int)bCrypt + (int)bComp*2;
    dstFilePath = srcFilePath + g_suffix[nIndex];
    if (nIndex == 0)
    {
        return true;
    }

    QFile qfile(dstFilePath);
    if (qfile.exists())
    {
        return true;
    }

    KeyIv &keyiv = m_keyiv[nIndex];
    keyiv.bComp = bComp;
    keyiv.bCrypt = bCrypt;

    if (m_pFileCrypt == NULL)
    {
        QSLOG_ERROR(QStringLiteral("获取加密实例失败."));
        return false;
    }

    //if (!m_pFileCrypt->CompCryptFile(srcFilePath, dstFilePath, keyiv))
    //{
    //    return false;
    //}

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

bool DistributeFile::sendToDir(const char *fullPath, KeyIv &keyiv, TransTask &taskInfo, int userIndex)
{

    if (fullPath == NULL)
    {
        return false;
    }
    //QSLOG_INFO(QString::fromLocal8Bit("send to dir : %1").arg(QString::fromLocal8Bit(fullPath)));

    QString strSendPath = taskInfo.dstFilePath.at(userIndex);
    QDir qDir;
    if (!qDir.exists(strSendPath))
    {
        qDir.mkpath(strSendPath);
    }
    if (strSendPath.length() >= 1 && strSendPath.right(1) != "/")
    {
        strSendPath += "/";
    }

    // 目标文件路径
    // QString strFullPath = strSendPath + taskInfo.fileName;
    QString strFullPath = strSendPath + taskInfo.strDestFileName;
    // 目标临时文件路径
    QString strTmpName = strFullPath + taskInfo.userInfo.at(userIndex).sendSuffix;

    //////////////////////////////////////////////////////////////////////////
    //string strUsr = userInfo.m_strSendUserName.toLocal8Bit().data();
    //string strPwd = userInfo.m_strSendPasswd.toStdString();
    if (strSendPath.length() >= 1 && strSendPath.left(1) != "/")
    {
        strSendPath = "/" + strSendPath;
    }
    string strPath = strSendPath.toLocal8Bit().data();
    //char url[512] = {0};
    //char usrPwd[100] = {0};
    //sprintf(url, "file://%s", strPath.c_str());
    QString url = QString("file://%1").arg(strPath.c_str());
    //sprintf(usrPwd, "%s:%s", strUsr.c_str(), strPwd.c_str());

    int bRet = -1;
    if (taskInfo.userInfo.at(userIndex).conput)	// 断点续传
    {
        bRet = m_oCurlFtp.conputFileToDir(url.toLocal8Bit().toStdString().c_str(), NULL,
                                          taskInfo.strDestFileName.toLocal8Bit().data(), fullPath,
                                          taskInfo.userInfo.at(userIndex).sendSuffix.toAscii().data());
    }
    else
    {
        bRet = m_oCurlFtp.uploadFileToDir(url.toLocal8Bit().toStdString().c_str(), NULL,
                                          taskInfo.strDestFileName.toLocal8Bit().data(), fullPath,
                                          taskInfo.userInfo.at(userIndex).sendSuffix.toAscii().data());
    }
    if (bRet != 0)
    {
        QSLOG_ERROR(QStringLiteral("上传到目录失败。"));
        //m_pManager->dealError(m_fileInfo, QStringLiteral("上传到目录失败。"));
        return false;
    }

    bool res = false;
    // 删除原来文件
    //QSLOG_INFO("begin remove");
    if (QFile::exists(strFullPath))
    {
        res = QFile::remove(strFullPath);
    }
    //QSLOG_INFO("remove success");
    //QSLOG_INFO("begin open");
    // 重命名
    QFile w_file(strTmpName);
    if (!w_file.open(QIODevice::ReadOnly))
    {
        QSLOG_ERROR("Fail to open device.");
        return false;
    }
    //QSLOG_INFO("begin rename");
    res = w_file.rename(strFullPath);
    if (!res)
    {
        QSLOG_ERROR("rename fail");
        return false;
    }
    //QSLOG_INFO("rename success");

    // 发送key和iv
    //if (taskInfo.userInfo.at(userIndex).compress || taskInfo.userInfo.at(userIndex).encrypt)
    //{
    //    strFullPath += ".key";
    //    writeKeyIv(strFullPath, keyiv);
    //}

    //QSLOG_INFO("bool DistributeFile::sendToDir finish");
    return true;
}


bool DistributeFile::writeKeyIv(const QString &fileName, KeyIv &keyiv)
{
    QString strTmpName = fileName + ".tmp";
    QFile wFile(strTmpName);
    if (!wFile.open(QIODevice::WriteOnly))
    {
        QSLOG_ERROR("Fail to writeKeyIv.");
        return false;
    }

    QDataStream out(&wFile);
    out << keyiv.bComp;
    out << keyiv.bCrypt;
    out << keyiv.key;
    out << keyiv.iv;
    wFile.close();

    // 删除原来key文件
    if (QFile::exists(fileName))
    {
        QFile::remove(fileName);
    }

    return wFile.rename(fileName);
}

bool DistributeFile::sendToFtp(const char *fullPath, KeyIv &keyiv, TransTask &task, int userIndex)
{
    string strIp =  task.userInfo.at(userIndex).ip.toStdString();
    int nPort = task.userInfo.at(userIndex).port;
    string strUsr = task.userInfo.at(userIndex).lgUser.toLocal8Bit().data();
    string strPwd = task.userInfo.at(userIndex).lgPass.toLocal8Bit().data();
    // 相对路径
    string strPath = task.dstFilePath.at(userIndex).toLocal8Bit().data();
    //char ftpUrl[512] = {0};
    //char usrPwd[100] = {0};
    QString ftpUrl = QString("ftp://%1:%2%3").arg(strIp.c_str()).arg(nPort).arg(strPath.c_str());
    //sprintf(ftpUrl, "ftp://%s:%d%s", strIp.c_str(), nPort, strPath.c_str());
    QString usrPwd = QString("%1:%2").arg(strUsr.c_str()).arg(strPwd.c_str());
    //sprintf(usrPwd, "%s:%s", strUsr.c_str(), strPwd.c_str());

    int bRet = -1;
    if (task.userInfo.at(userIndex).conput)	// 断点续传
    {
        bRet = m_oCurlFtp.conputFileToFtp(ftpUrl.toLocal8Bit().toStdString().c_str(),
                                          usrPwd.toLocal8Bit().toStdString().c_str()
                                          , task.strDestFileName.toLocal8Bit().data(),
                                          fullPath, task.userInfo.at(userIndex).sendSuffix.toAscii().data());
    }
    else
    {
        bRet = m_oCurlFtp.uploadFileToFtp(ftpUrl.toLocal8Bit().toStdString().c_str(),
                                          usrPwd.toLocal8Bit().toStdString().c_str(),
                                          task.strDestFileName.toLocal8Bit().data(),
                                          fullPath, task.userInfo.at(userIndex).sendSuffix.toAscii().data());
    }
    if (bRet != 0)
    {
        QString logInfo = QStringLiteral("文件[%1]上传到ftp失败。").arg(task.fileName);
        //m_pBase->emitLog(task.collectSet.dirName, logInfo);
        m_pBase->emitLog(logInfo, FAIL);
        return false;
    }

    // 发送key和iv
    //if (task.userInfo.at(userIndex).compress || task.userInfo.at(userIndex).encrypt)
    //{
    //    string strKeyIv = task.fileName.toStdString() + ".key";
    //    QByteArray baKey;
    //    QDataStream out(&baKey, QIODevice::WriteOnly);
    //    out << keyiv.bComp;
    //    out << keyiv.bCrypt;
    //    out << keyiv.key;
    //    out << keyiv.iv;

    //    // 从内存上传到FTP
    //    MemoryData mkey;
    //    mkey.size = baKey.size();
    //    mkey.memdata = (char *)malloc(mkey.size + 1);
    //    memcpy(mkey.memdata, baKey.data(), mkey.size);
    //    mkey.memdata[mkey.size] = 0;
    //    if (m_pFtp->uploadFile(ftpUrl, usrPwd, &mkey, strKeyIv, task.userInfo.at(userIndex).sendSuffix.toAscii().data()) != 0)
    //    {
    //        QSLOG_ERROR(QString("Fail to upload %1.").arg(QString::fromStdString(strKeyIv)));
    //        //return false;
    //    }
    //}

    return true;
}

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
