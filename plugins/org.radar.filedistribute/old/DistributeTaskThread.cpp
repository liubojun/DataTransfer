#include "DistributeTaskThread.h"
#include "ctkPublicFun.h"
#include "ctkLog.h"
#pragma warning(disable:4996)

DistributeTaskThread::DistributeTaskThread(FileDistributeThread *pManager, const TransFileInfo &tInfo)
    : m_pManager(pManager)
    , m_fileInfo(tInfo)
{
    m_pFtp = new CurlFtp();
    if (m_pFtp == NULL)
    {
        QSLOG_ERROR("Create CurlFtp error.");
    }
    //connect(m_pFtp, SIGNAL(progress(qint64, qint64, qint64, qint64)), this, SLOT(ftpTransProgress(qint64, qint64, qint64, qint64)));
}

void DistributeTaskThread::run()
{
    if (!m_pManager->m_bRun)
    {
        return;
    }

    m_fileInfo.m_status = BeHandled;
    m_fileInfo.m_strStartDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

    // 1. 获取文件数据
    if (1 == m_fileInfo.m_iCollectWay)
    {
        char ftpUrl[200] = {0};
        char usrPwd[100] = {0};
        sprintf(ftpUrl, "ftp://%s:%d%s", m_fileInfo.m_strCollectIP.c_str(), m_fileInfo.m_iCollectPort,
                m_fileInfo.m_strCollectFileName.toStdString().c_str());
        sprintf(usrPwd, "%s:%s", m_fileInfo.m_strCollectUser.c_str(), m_fileInfo.m_strCollectPwd.c_str());

        if (0 != m_pFtp->downloadFile(ftpUrl, usrPwd, &m_mData[0]))
        {
            m_pManager->dealError(m_fileInfo, QString("Fail to get file from ftp."));
            return;
        }
    }
    else if (0 == m_fileInfo.m_iCollectWay)
    {
        if (!collectFromDir(m_mData[0]))
        {
            m_pManager->dealError(m_fileInfo, QString("Fail to collectFromDir."));
            return;
        }
    }
    else
    {
        m_pManager->dealError(m_fileInfo, QString("Unknow collect way."));
        return;
    }

    m_fileInfo.m_getDone = m_mData[0].size;
    for (int i=0; i<m_fileInfo.m_strUsers.size(); ++i)
    {
        // 获取分发用户信息
        m_fileInfo.m_strSendID = m_fileInfo.m_strUsers.at(i);
        m_fileInfo.m_status = BeHandled;
        SendAcctInfo userInfo;
        if (!m_pManager->getUserInfo(m_fileInfo.m_strSendID, userInfo))
        {
            m_pManager->dealError(m_fileInfo, QString("Fail to get user info."));
            continue;
        }
        // 发送50%进度
        m_pManager->sendTransProgress(m_fileInfo);

        // 2.加密压缩
        int nIndex = 0;
        if (!CrypCurtFile(userInfo.m_bCompress, userInfo.m_bEncrypt, nIndex))
        {
            m_pManager->dealError(m_fileInfo, QString("Fail to cryptAndPutFile."));
            continue;
        }

        // 3.分发数据
        bool bRes = false;
        if (0 == userInfo.m_iSendWay)		//目录分发
        {
            bRes = sendToDir(m_mData[nIndex], m_keyiv[nIndex], userInfo);
        }
        else if (1 == userInfo.m_iSendWay)	//ftp分发
        {
            bRes = sendToFtp(m_mData[nIndex], m_keyiv[nIndex], userInfo);
        }
        else
        {
            m_pManager->dealError(m_fileInfo, QString("Unkonw send way."));
            continue;
        }

        if (bRes)
        {
            m_fileInfo.m_status = Finished;
            m_fileInfo.m_strEndDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
            m_pManager->sendTransProgress(m_fileInfo);
        }
    }

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
    memcpy(mData.memdata, t_in.readAll().data(), t_iFileSize);
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
    if (!loginToSendRemote(userInfo))
    {
        return false;
    }
    QString strSendPath = getCurrentPath(userInfo);
    QDir qDir;
    if (!qDir.exists(strSendPath))
    {
        qDir.mkpath(strSendPath);
    }
    QString strFullPath = strSendPath + m_fileInfo.m_strFileName;
    // 写数据
    bool bFlag = writeFileFromBuffer(strFullPath, mData, userInfo.m_bConSend);
    if (!bFlag)
    {
        m_pManager->dealError(m_fileInfo, QString("SendToDir error."));
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
    if (userInfo.m_bKeepDir)
    {
        strCollectSubPath = getCollectSubPath();
    }
    return strRootPath + strCollectSubPath;
}

QString DistributeTaskThread::getCollectSubPath()
{
    int nBasePathLen = m_fileInfo.m_strCollectPath.length();
    int nFileNameLen = m_fileInfo.m_strFileName.length();
    int nFileFullPathLen = m_fileInfo.m_strCollectFileName.length();
    QString res = m_fileInfo.m_strCollectFileName.mid(nBasePathLen, nFileFullPathLen-nBasePathLen-nFileNameLen);
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
    string strPath = getCurrentPath(userInfo).toLocal8Bit().data();
    char ftpUrl[200] = {0};
    char usrPwd[100] = {0};
    sprintf(ftpUrl, "ftp://%s:%d%s", strIp.c_str(), nPort, strPath.c_str());
    sprintf(usrPwd, "%s:%s", strUsr.c_str(), strPwd.c_str());

    int bRet = -1;
    if (userInfo.m_bConSend)	// 断点续传
    {
        bRet = m_pFtp->conputFile(ftpUrl, usrPwd, &mData, m_fileInfo.m_strFileName.toStdString());
    }
    else
    {
        bRet = m_pFtp->uploadFile(ftpUrl, usrPwd, &mData, m_fileInfo.m_strFileName.toStdString());
    }
    if (bRet != 0)
    {
        m_pManager->dealError(m_fileInfo, QString("Fail to uploadFile to ftp."));
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
