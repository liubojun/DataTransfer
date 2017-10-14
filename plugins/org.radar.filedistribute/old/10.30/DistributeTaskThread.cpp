#include "DistributeTaskThread.h"
#include "ctkPublicFun.h"
#include "ctkLog.h"
#include "pathbuilder.h"
#pragma warning(disable:4996)

DistributeTaskThread::DistributeTaskThread(FileDistributeThread *pManager, const TransFileInfo &tInfo)
    : m_pManager(pManager)
    , m_fileInfo(tInfo)
    , m_pFileCrypt(NULL)
{
    m_pFtp = QSharedPointer<CurlFtp>(new CurlFtp());
    //qDebug() << "DistributeTaskThread run in thread: " << QThread::currentThreadId();
    if (m_pFtp == NULL)
    {
        QSLOG_ERROR("Create CurlFtp error.");
        exit(-1);
    }
    m_pFileCrypt = m_pManager->m_pFileCrypt;
}

DistributeTaskThread::~DistributeTaskThread()
{
    QMutexLocker autoLocker(&m_pManager->m_taskMutex);
    QSLOG_DEBUG("~DistributeTaskThread");
}

void DistributeTaskThread::run()
{
    QSLOG_DEBUG(QString("begin task: %1.").arg(m_fileInfo.m_strCollectFileName));

    if (!m_pManager->m_bRun)
    {
        m_pManager->onTaskFinished(getCurlAddr());
        return;
    }

    m_fileInfo.m_status = BeHandled;
    m_fileInfo.m_strStartDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

    CPathBuilder pb;
    QString tmpPath = pb.getCurlFilePath() + "/" + QString::number(getCurlAddr());
    QDir qdir(tmpPath);
    if (!qdir.exists())
    {
        qdir.mkpath(tmpPath);
    }
    QString tmpFilePath = tmpPath + "/" + m_fileInfo.m_strFileName;
    struct FileData fileData;
    // QString转char*
    QByteArray baTmp = tmpFilePath.toLocal8Bit();
    fileData.filename = baTmp.data();

    // 1. 获取文件数据
    if (1 == m_fileInfo.m_iCollectWay)	// ftp
    {
        char ftpUrl[200] = {0};
        char usrPwd[100] = {0};
        sprintf(ftpUrl, "ftp://%s:%d%s", m_fileInfo.m_strCollectIP.c_str(), m_fileInfo.m_iCollectPort,
                m_fileInfo.m_strCollectFileName.toStdString().c_str());
        sprintf(usrPwd, "%s:%s", m_fileInfo.m_strCollectUser.c_str(), m_fileInfo.m_strCollectPwd.c_str());

        //if (0 != m_pFtp->downloadFile(ftpUrl, usrPwd, &m_mData[0]))
        if (0 != m_pFtp->downloadFile(ftpUrl, usrPwd, &fileData))
        {
            m_pManager->dealError(m_fileInfo, QStringLiteral("从Ftp获取文件失败。"));
            m_pManager->onTaskFinished(getCurlAddr());
            return;
        }
    }
    else if (0 == m_fileInfo.m_iCollectWay)	// 共享目录
    {
        char url[200] = {0};
        char usrPwd[100] = {0};
        QString strTmp = m_fileInfo.m_strCollectFileName;
        if (strTmp.left(1) != "/")
        {
            strTmp = "/" + strTmp;
        }
        sprintf(url, "file://%s", strTmp.toLocal8Bit().data());
        sprintf(usrPwd, "%s:%s", m_fileInfo.m_strCollectUser.c_str(), m_fileInfo.m_strCollectPwd.c_str());
        //if (0 != m_pFtp->downloadFile(url, usrPwd, &m_mData[0]))
        if (0 != m_pFtp->downloadFile(url, usrPwd, &fileData))
        {
            for (int i=0; i<m_fileInfo.m_strUsers.size(); ++i)
            {
                m_fileInfo.m_strSendID = m_fileInfo.m_strUsers.at(i);
                m_pManager->dealError(m_fileInfo, QStringLiteral("从目录获取文件失败。"));
            }
            m_pManager->onTaskFinished(getCurlAddr());
            return;
        }
    }
    else
    {
        m_pManager->dealError(m_fileInfo, QStringLiteral("未知的收集方式。"));
        m_pManager->onTaskFinished(getCurlAddr());
        return;
    }

    QSLOG_DEBUG("Finish to collect file.");

    m_fileInfo.m_getDone = m_mData[0].size;
    for (int i=0; i<m_fileInfo.m_strUsers.size(); ++i)
    {
        // 获取分发用户信息
        m_fileInfo.m_strSendID = m_fileInfo.m_strUsers.at(i);
        m_fileInfo.m_status = BeHandled;
        SendAcctInfo userInfo;
        if (!m_pManager->getUserInfo(m_fileInfo.m_strSendID, userInfo))
        {
            m_pManager->dealError(m_fileInfo, QStringLiteral("获取用户信息失败。"));
            continue;
        }
        // 发送50%进度
        //m_pManager->sendTransProgress(m_fileInfo);

        // 2.加密压缩
        int nIndex = 0;
//         if (!CrypCurtFile(userInfo.m_bCompress, userInfo.m_bEncrypt, nIndex))
//         {
//             m_pManager->dealError(m_fileInfo, QStringLiteral("加密压缩文件失败。"));
//             continue;
//         }

        // 3.分发数据
        bool bRes = false;
        if (0 == userInfo.m_iSendWay)		//目录分发
        {
            //bRes = sendToDir(m_mData[nIndex], m_keyiv[nIndex], userInfo);
            bRes = sendToDir(fileData.filename, m_keyiv[nIndex], userInfo);
        }
        else if (1 == userInfo.m_iSendWay)	//ftp分发
        {
            //bRes = sendToFtp(m_mData[nIndex], m_keyiv[nIndex], userInfo);
            bRes = sendToFtp(fileData.filename, m_keyiv[nIndex], userInfo);
        }
        else
        {
            m_pManager->dealError(m_fileInfo, QStringLiteral("未知的发送方式。"));
            continue;
        }

        if (bRes)
        {
            //QMutexLocker autoLocker(&m_pManager->m_taskMutex);
            m_fileInfo.m_status = Finished;
            //m_fileInfo.m_strEndDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
            m_pManager->sendTransProgress(m_fileInfo);

            //QSLOG_DEBUG(QString("hahaha-- %1: %2").arg(m_fileInfo.m_strCollectFileName).arg(m_fileInfo.m_status));
        }
    }

    m_pManager->onTaskFinished(getCurlAddr());
    //emit taskFinished(getCurlAddr(), m_threadId);

    QSLOG_DEBUG(QString("finish task: %1.").arg(m_fileInfo.m_strCollectFileName));
}

bool DistributeTaskThread::collectFromDir(MemoryData &mData)
{
    QFile t_in(m_fileInfo.m_strCollectFileName);
    if (!t_in.open(QIODevice::ReadOnly))
    {
        return false;
    }

    long t_iFileSize = t_in.size();
    m_fileInfo.m_totalSize = t_iFileSize;

    mData.size = t_iFileSize;
    mData.memdata = (char *)malloc(t_iFileSize + 1);
    if (mData.memdata == NULL)
    {
        QSLOG_ERROR(QString("Fail to malloc %1").arg(t_iFileSize));
        t_in.close();
        return false;
    }
    QByteArray baTmp = t_in.readAll();
    memcpy(mData.memdata, baTmp.data(), t_iFileSize);
    mData.memdata[t_iFileSize] = 0;

    t_in.close();

    return true;
}

bool DistributeTaskThread::CrypCurtFile(bool bComp, bool bCrypt, int &nIndex)
{
    if (m_pFileCrypt == NULL)
    {
        QSLOG_DEBUG("m_pFileCrypt is nulll.");
        return false;
    }
    nIndex = (int)bCrypt + (int)bComp*2;
    if ((m_mData[nIndex].memdata != NULL) || (nIndex == 0))
    {
        return true;
    }

    QByteArray baOrg, baResult;
    KeyIv &keyiv = m_keyiv[nIndex];
    keyiv.bComp = bComp;
    keyiv.bCrypt = bCrypt;

    baOrg.append(m_mData[0].memdata, m_mData[0].size);
    if (!m_pFileCrypt->CompCryptFile(baOrg, baResult, keyiv, bComp, bCrypt))
    {
        return false;
    }

    m_mData[nIndex].size = baResult.size();
    m_mData[nIndex].memdata = (char *)malloc(m_mData[nIndex].size);
    memcpy(m_mData[nIndex].memdata, baResult.data(), baResult.size());

    return true;
}

bool DistributeTaskThread::loginToSendRemote(SendAcctInfo &userInfo)
{
    bool &bConnect = userInfo.m_bDirConnected;
    if (!bConnect)
    {
        if (0 == userInfo.m_iSendTryTimes)
        {
            //userInfo.m_strSendHost = getHostFromFileName(userInfo.m_strRltvPath);
            userInfo.m_strSendHost = userInfo.m_strRltvPath;
            userInfo.m_iSendTryTimes++;
            bConnect = connectToWinRemote(userInfo.m_strSendHost, userInfo.m_strSendUserName, userInfo.m_strSendPasswd);
        }
    }

    return bConnect;
}

QString DistributeTaskThread::getHostFromFileName(const QString& r_strFullPathFileName)
{
    string t_file = r_strFullPathFileName.toStdString();
    size_t t_index = t_file.find('/',2);
    if (t_index == string::npos)
        return "";
    QString ret="\\\\";
    ret += t_file.substr(2, t_index - 2).c_str();
    return ret;
}

bool DistributeTaskThread::sendToDir(MemoryData &mData, KeyIv &keyiv, SendAcctInfo &userInfo)
{
    if (mData.memdata == NULL)
    {
        return false;
    }
//     if (!loginToSendRemote(userInfo))
//     {
//         return false;
//     }
    QString strSendPath = getCurrentPath(userInfo);
    QDir qDir;
    if (!qDir.exists(strSendPath))
    {
        qDir.mkpath(strSendPath);
    }
    if (strSendPath.right(1) != "/")
    {
        strSendPath += "/";
    }

    QString strFullPath = strSendPath + m_fileInfo.m_strFileName;
    QString strTmpName = strFullPath + ".~tmp";

    //////////////////////////////////////////////////////////////////////////
    string strUsr = userInfo.m_strSendUserName.toLocal8Bit().data();
    string strPwd = userInfo.m_strSendPasswd.toStdString();
    if (strSendPath.left(1) != "/")
    {
        strSendPath = "/" + strSendPath;
    }
    string strPath = strSendPath.toLocal8Bit().data();
    char url[200] = {0};
    char usrPwd[100] = {0};
    sprintf(url, "file://%s", strPath.c_str());
    sprintf(usrPwd, "%s:%s", strUsr.c_str(), strPwd.c_str());

    int bRet = -1;
    if (userInfo.m_bConSend)	// 断点续传
    {
        bRet = m_pFtp->conputFileToDir(url, usrPwd, &mData, m_fileInfo.m_strFileName.toLocal8Bit().data());
    }
    else
    {
        bRet = m_pFtp->uploadFileToDir(url, usrPwd, &mData, m_fileInfo.m_strFileName.toLocal8Bit().data());
    }
    if (bRet != 0)
    {
        m_pManager->dealError(m_fileInfo, QStringLiteral("上传到目录失败。"));
        return false;
    }

    bool res = false;
    // 删除原来文件
    if (QFile::exists(strFullPath))
    {
        res = QFile::remove(strFullPath);
    }
    // 重命名
    QFile w_file(strTmpName);
    if (!w_file.open(QIODevice::ReadOnly))
    {
        QSLOG_ERROR("Fail to open device.");
        return false;
    }
    res = w_file.rename(strFullPath);
    if (!res)
    {
        return false;
    }

    // 发送key和iv
    if (userInfo.m_bCompress || userInfo.m_bEncrypt)
    {
        strFullPath += ".key";
        writeKeyIv(strFullPath, keyiv);
    }

    // 为了显示100%，暂时不设置为完成
    m_fileInfo.m_putDone = m_fileInfo.m_getDone;
    m_fileInfo.m_errorInfo = "";
    m_pManager->sendTransProgress(m_fileInfo);
    //if (m_fileInfo.m_status == Finished)
    //{
    //    int a = 0;
    //}

    return true;
}

bool DistributeTaskThread::sendToDir(const char *fullPath, KeyIv &keyiv, SendAcctInfo &userInfo)
{
    if (fullPath == NULL)
    {
        return false;
    }

    QString strSendPath = getCurrentPath(userInfo);
    QDir qDir;
    if (!qDir.exists(strSendPath))
    {
        qDir.mkpath(strSendPath);
    }
    if (strSendPath.right(1) != "/")
    {
        strSendPath += "/";
    }

    QString strFullPath = strSendPath + m_fileInfo.m_strFileName;
    QString strTmpName = strFullPath + ".~tmp";

    //////////////////////////////////////////////////////////////////////////
    string strUsr = userInfo.m_strSendUserName.toLocal8Bit().data();
    string strPwd = userInfo.m_strSendPasswd.toStdString();
    if (strSendPath.left(1) != "/")
    {
        strSendPath = "/" + strSendPath;
    }
    string strPath = strSendPath.toLocal8Bit().data();
    char url[200] = {0};
    char usrPwd[100] = {0};
    sprintf(url, "file://%s", strPath.c_str());
    sprintf(usrPwd, "%s:%s", strUsr.c_str(), strPwd.c_str());

    int bRet = -1;
    if (userInfo.m_bConSend)	// 断点续传
    {
        bRet = m_pFtp->conputFileToDir(url, usrPwd, m_fileInfo.m_strFileName.toLocal8Bit().data(), fullPath);
    }
    else
    {
        bRet = m_pFtp->uploadFileToDir(url, usrPwd, m_fileInfo.m_strFileName.toLocal8Bit().data(), fullPath);
    }
    if (bRet != 0)
    {
        m_pManager->dealError(m_fileInfo, QStringLiteral("上传到目录失败。"));
        return false;
    }

    bool res = false;
    // 删除原来文件
    if (QFile::exists(strFullPath))
    {
        res = QFile::remove(strFullPath);
    }
    // 重命名
    QFile w_file(strTmpName);
    if (!w_file.open(QIODevice::ReadOnly))
    {
        QSLOG_ERROR("Fail to open device.");
        return false;
    }
    res = w_file.rename(strFullPath);
    if (!res)
    {
        return false;
    }

    // 发送key和iv
//     if (userInfo.m_bCompress || userInfo.m_bEncrypt)
//     {
//         strFullPath += ".key";
//         writeKeyIv(strFullPath, keyiv);
//     }

    // 为了显示100%，暂时不设置为完成
    m_fileInfo.m_putDone = m_fileInfo.m_getDone;
    m_fileInfo.m_errorInfo = "";
    m_pManager->sendTransProgress(m_fileInfo);

    return true;
}

QString DistributeTaskThread::getCurrentPath(const SendAcctInfo &userInfo)
{
    QString strRootPath, strCollectSubPath("");
    strRootPath = userInfo.m_strRltvPath;
    if (strRootPath.right(1) != "/")
    {
        strRootPath += "/";
    }
    if (userInfo.m_bKeepDir && !m_fileInfo.m_strCollectPath.isEmpty())
    {
        strCollectSubPath = getCollectSubPath();
    }
    return strRootPath + strCollectSubPath;
}

QString DistributeTaskThread::getCollectSubPath()
{
    int nBasePathLen = m_fileInfo.m_strCollectPath.length();
    if (m_fileInfo.m_strCollectPath.right(1) != "/")
    {
        nBasePathLen += 1;
    }
    int nFileNameLen = m_fileInfo.m_strFileName.length();
    int nFileFullPathLen = m_fileInfo.m_strCollectFileName.length();
    QString res("");
    int nLen = nFileFullPathLen-nBasePathLen-nFileNameLen-1;
    if (nLen > 0)
    {
        res = m_fileInfo.m_strCollectFileName.mid(nBasePathLen, nLen);
    }
    return res;
}

bool DistributeTaskThread::writeFileFromBuffer(const QString &fileName, const MemoryData &mData, bool bConput)
{
    QString strTmpName = fileName + ".~tmp";
    QFile w_file(strTmpName);
    char *pStart = mData.memdata;
    size_t tSize = mData.size;
    //////////////////////////////////////////////////////////////////////////
    // 共享目录断点续传
    if (bConput)
    {
        if (!w_file.open(QIODevice::Append))
        {
            return false;
        }
        qint64 nSend = w_file.size();
        pStart += nSend;
        tSize -= nSend;
    }
    else
    {
        if (!w_file.open(QIODevice::WriteOnly))
        {
            QSLOG_ERROR("Fail to open device.");
            return false;
        }
    }

    w_file.write(pStart, tSize);
    w_file.close();

    // 删除原来文件
    if (QFile::exists(fileName))
    {
        QFile::remove(fileName);
    }
    w_file.rename(fileName);

    return true;
}

bool DistributeTaskThread::writeKeyIv(const QString &fileName, KeyIv &keyiv)
{
    QString strTmpName = fileName + ".~tmp";
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

bool DistributeTaskThread::sendToFtp(MemoryData &mData, KeyIv &keyiv, SendAcctInfo &userInfo)
{
    string strIp = userInfo.m_strSendHost.toStdString();
    int nPort = userInfo.m_iSendPort;
    string strUsr = userInfo.m_strSendUserName.toStdString();
    string strPwd = userInfo.m_strSendPasswd.toStdString();
    QString qstrPath = getCurrentPath(userInfo);
    if (qstrPath.right(1) != "/")
    {
        qstrPath += "/";
    }
    string strPath = qstrPath.toLocal8Bit().data();
    char ftpUrl[200] = {0};
    char usrPwd[100] = {0};
    sprintf(ftpUrl, "ftp://%s:%d%s", strIp.c_str(), nPort, strPath.c_str());
    sprintf(usrPwd, "%s:%s", strUsr.c_str(), strPwd.c_str());

    int bRet = -1;
    if (userInfo.m_bConSend)	// 断点续传
    {
        bRet = m_pFtp->conputFile(ftpUrl, usrPwd, &mData, m_fileInfo.m_strFileName.toLocal8Bit().data());
    }
    else
    {
        bRet = m_pFtp->uploadFile(ftpUrl, usrPwd, &mData, m_fileInfo.m_strFileName.toLocal8Bit().data());
    }
    if (bRet != 0)
    {
        m_pManager->dealError(m_fileInfo, QStringLiteral("上传到Ftp失败。"));
        return false;
    }

    // 为了显示100%，暂时不设置为完成
    m_fileInfo.m_putDone = m_fileInfo.m_getDone;
    m_pManager->sendTransProgress(m_fileInfo);

    // 发送key和iv
    if (userInfo.m_bCompress || userInfo.m_bEncrypt)
    {
        string strKeyIv = m_fileInfo.m_strFileName.toStdString() + ".key";
        QByteArray baKey;
        QDataStream out(&baKey, QIODevice::WriteOnly);
        out << keyiv.bComp;
        out << keyiv.bCrypt;
        out << keyiv.key;
        out << keyiv.iv;

        MemoryData mkey;
        mkey.size = baKey.size();
        mkey.memdata = (char *)malloc(mkey.size + 1);
        memcpy(mkey.memdata, baKey.data(), mkey.size);
        mkey.memdata[mkey.size] = 0;
        if (m_pFtp->uploadFile(ftpUrl, usrPwd, &mkey, strKeyIv) != 0)
        {
            QSLOG_ERROR(QString("Fail to upload %1.").arg(QString::fromStdString(strKeyIv)));
            //return false;
        }
    }

    return true;
}

bool DistributeTaskThread::sendToFtp(const char *fullPath, KeyIv &keyiv, SendAcctInfo &userInfo)
{
    string strIp = userInfo.m_strSendHost.toStdString();
    int nPort = userInfo.m_iSendPort;
    string strUsr = userInfo.m_strSendUserName.toStdString();
    string strPwd = userInfo.m_strSendPasswd.toStdString();
    QString qstrPath = getCurrentPath(userInfo);
    if (qstrPath.right(1) != "/")
    {
        qstrPath += "/";
    }
    string strPath = qstrPath.toLocal8Bit().data();
    char ftpUrl[200] = {0};
    char usrPwd[100] = {0};
    sprintf(ftpUrl, "ftp://%s:%d%s", strIp.c_str(), nPort, strPath.c_str());
    sprintf(usrPwd, "%s:%s", strUsr.c_str(), strPwd.c_str());

    int bRet = -1;
    if (userInfo.m_bConSend)	// 断点续传
    {
        bRet = m_pFtp->conputFileToFtp(ftpUrl, usrPwd, m_fileInfo.m_strFileName.toLocal8Bit().data(), fullPath);
    }
    else
    {
        bRet = m_pFtp->uploadFileToFtp(ftpUrl, usrPwd, m_fileInfo.m_strFileName.toLocal8Bit().data(), fullPath);
    }
    if (bRet != 0)
    {
        m_pManager->dealError(m_fileInfo, QStringLiteral("上传到Ftp失败。"));
        return false;
    }

    // 为了显示100%，暂时不设置为完成
    m_fileInfo.m_putDone = m_fileInfo.m_getDone;
    m_pManager->sendTransProgress(m_fileInfo);

    // 发送key和iv
//     if (userInfo.m_bCompress || userInfo.m_bEncrypt)
//     {
//         string strKeyIv = m_fileInfo.m_strFileName.toStdString() + ".key";
//         QByteArray baKey;
//         QDataStream out(&baKey, QIODevice::WriteOnly);
//         out << keyiv.bComp;
//         out << keyiv.bCrypt;
//         out << keyiv.key;
//         out << keyiv.iv;
//
//         MemoryData mkey;
//         mkey.size = baKey.size();
//         mkey.memdata = (char *)malloc(mkey.size + 1);
//         memcpy(mkey.memdata, baKey.data(), mkey.size);
//         mkey.memdata[mkey.size] = 0;
//         if (m_pFtp->uploadFile(ftpUrl, usrPwd, &mkey, strKeyIv) != 0)
//         {
//             QSLOG_ERROR(QString("Fail to upload %1.").arg(QString::fromStdString(strKeyIv)));
//             //return false;
//         }
//     }

    return true;
}

int DistributeTaskThread::getCurlAddr()
{
    return (int)m_pFtp->m_pCurl;
}

// void DistributeTaskThread::startWork()
// {
//     QTimer::singleShot(10, this, SLOT(run()));
// }

void SignalSender::emittaskFinished(int id)
{
    emit taskFinished(id);
}
