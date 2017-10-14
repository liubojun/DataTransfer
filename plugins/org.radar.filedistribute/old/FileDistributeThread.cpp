#include "FileDistributeThread.h"
#include "ctkLog.h"
#include "curlftp.h"
#include "DistributeTaskThread.h"
#include <vector>
#include <QMessageBox>
#include <QDateTime>
#include <QFile>
#include "transFileInfo.h"
#include <windows.h>
#include <Winnetwk.h>
#include <QDir>
using namespace std;

#pragma warning(disable:4996)

extern SignalObject g_SignalObject;


FileDistributeThread::FileDistributeThread(void *parent)
{
    m_mutex = QSharedPointer<QMutex>(new QMutex);
    m_mutexData = QSharedPointer<QMutex>(new QMutex);

    m_pFtp = new CurlFtp();
    if (m_pFtp == NULL)
    {
        QSLOG_ERROR("Create CurlFtp error.");
        exit(-1);
    }

    m_parent = parent;

    m_stoped = false;
    m_currentIndex = -1;
    m_hasTask = false;
    m_strLastSendID = "";
    m_strLastCdPath = "";
    m_bDirConnected = false;
    m_iCollectTryTimes = 0;
    m_bSendSignalToMainWindow = false;

    m_pFileCrypt = NULL;

//     int nThreadNum = QThreadPool::globalInstance()->maxThreadCount();
//     if (nThreadNum < 5)
//     {
//         QThreadPool::globalInstance()->setMaxThreadCount(5);
//     }

    this->moveToThread(&m_oThread);
    m_oThread.start();
    m_bRun = true;
}

FileDistributeThread::~FileDistributeThread()
{
    m_bRun = false;

    QMutexLocker locker(m_mutexData.data());
    if (m_pFtp != NULL)
    {
        delete m_pFtp;
        m_pFtp = NULL;
    }

    QThreadPool::globalInstance()->waitForDone(2000);
}

FileDistributeThread::FileDistributeThread(const FileDistributeThread &fd)
{
    this->m_mutex = QSharedPointer<QMutex>(new QMutex);
    this->m_mutexData = QSharedPointer<QMutex>(new QMutex);

    this->m_pFtp = new CurlFtp();

    this->m_parent     = fd.m_parent;
    this->m_stoped	   = fd.m_stoped;
    m_currentIndex = -1;
    m_hasTask = false;
    m_strLastSendID = "";
    m_strLastCdPath = "";

    m_strCollectID = fd.m_strCollectID;
    m_strCollectHost = fd.m_strCollectHost;

    m_strCollectUserName=fd.m_strCollectUserName;
    m_strCollectPasswd = fd.m_strCollectPasswd;
    m_iCollectPort = fd.m_iCollectPort;
    m_sendUserAcct = fd.m_sendUserAcct;
    m_transFileList = fd.m_transFileList;
    m_bDirConnected = false;
    m_iCollectTryTimes = 0;
    m_bSendSignalToMainWindow = fd.m_bSendSignalToMainWindow;
    m_strRltvPath = fd.m_strRltvPath;
    m_pFileCrypt = fd.m_pFileCrypt;

    // 使用事件队列方式进行信号槽绑定
    //connect(this, SIGNAL(doTask()), this, SLOT(processData()), Qt::QueuedConnection);
    if (NULL != m_parent && m_bSendSignalToMainWindow)
    {
        connect(this, SIGNAL(transProgress(const TransFileInfo)),(QObject*)m_parent,SIGNAL(transProgress(const TransFileInfo)));
    }
    connect(m_pFtp, SIGNAL(progress(qint64, qint64, qint64, qint64)), this, SLOT(ftpTransProgress(qint64, qint64, qint64, qint64)));

    // QTimer::singleShot(1000, this, SLOT(run()));
}

FileDistributeThread& FileDistributeThread::operator=(const FileDistributeThread &fd)
{
    if ( this == &fd)
    {
        return *this;
    }

    this->m_mutex = QSharedPointer<QMutex>(new QMutex);
    this->m_mutexData = QSharedPointer<QMutex>(new QMutex);

    if (this->m_pFtp != NULL)
    {
        delete this->m_pFtp;
    }
    m_pFtp = new CurlFtp();

    this->m_parent     = fd.m_parent;
    this->m_stoped	   = fd.m_stoped;

    m_currentIndex = -1;
    m_hasTask = false;
    m_strLastSendID = "";
    m_strLastCdPath = "";

    m_strCollectID = fd.m_strCollectID;
    m_strCollectHost = fd.m_strCollectHost;

    m_strCollectUserName=fd.m_strCollectUserName;
    m_strCollectPasswd = fd.m_strCollectPasswd;
    m_iCollectPort = fd.m_iCollectPort;
    m_sendUserAcct = fd.m_sendUserAcct;
    m_transFileList = fd.m_transFileList;

    m_bDirConnected = false;

    m_iCollectTryTimes = 0;
    m_bSendSignalToMainWindow = fd.m_bSendSignalToMainWindow;
    m_strRltvPath = fd.m_strRltvPath;
    m_pFileCrypt = fd.m_pFileCrypt;

    //connect(this, SIGNAL(doTask()), this, SLOT(processData()), Qt::QueuedConnection);
    if (NULL != m_parent && m_bSendSignalToMainWindow)
    {
        connect(this        ,SIGNAL(transProgress(const TransFileInfo)),(QObject*)m_parent,SIGNAL(transProgress(const TransFileInfo)));
    }

    return *this;
}

void FileDistributeThread::setSendAcctInfo(const vector<SendAcctInfo>& r_vSendAcctInfo)
{
    QMutexLocker locker(m_mutexData.data());
    for (unsigned int i = 0; i < r_vSendAcctInfo.size(); i++)
    {
        //if ( !m_sendUserAcct.contains(r_vSendAcctInfo[i].m_strSendID) )
        //{
        m_sendUserAcct.insert(r_vSendAcctInfo[i].m_strSendID,r_vSendAcctInfo[i]);
        //}
    }
}

void FileDistributeThread::addTransFileInfo(const vector<TransFileInfo>& r_fileList)
{
    QMutexLocker locker(m_mutexData.data());
    if (m_transFileList.size() == 0 && !r_fileList.empty())
    {
        m_currentIndex = 0;
    }
    for (unsigned int i = 0; i < r_fileList.size(); i++)
    {
        m_transFileList.push_back(r_fileList[i]);
    }
    m_hasTask = true;
    //emit doTask();
}

void FileDistributeThread::getTransFileInfo(vector<TransFileInfo>& r_fileList)
{
    QMutexLocker locker(m_mutexData.data());
    for (int i = 0; i<m_transFileList.size(); i++)
    {
        r_fileList.push_back(m_transFileList.at(i));
    }
}

void FileDistributeThread::stop()
{
    m_mutex->lock();
    m_stoped = true;
    m_mutex->unlock();
}

// 处理分发任务
// 递归太多层次，造成堆栈溢出，在同一个线程中，通过信号槽方式调用，emit 信号，相当于直接调用其绑定的槽函数
void FileDistributeThread::processData()
{
    //qDebug()<<"FileDistributeThread::processData() run in thread: "<<QThread::currentThread();
    QMutexLocker locker(m_mutexData.data());
    if (!m_hasTask || m_currentIndex == -1 || m_transFileList.empty() || m_currentIndex >= m_transFileList.size())
    {
        return;
    }

    QSLOG_DEBUG("Beging to processData.");

    m_testDirCmdIdMap.clear();
    m_lstNeedCreate.clear();

    m_transFileList[m_currentIndex].m_status = BeHandled;
    m_transFileList[m_currentIndex].m_strStartDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

    MemoryData mData;	// 用于存放文件内容
    // 1. 获取文件数据
    if (1 == m_transFileList[m_currentIndex].m_iCollectWay)		//ftp收集
    {
        char ftpUrl[200] = {0};
        char usrPwd[100] = {0};
        sprintf(ftpUrl, "ftp://%s:%d%s", m_strCollectHost.toStdString().c_str(), m_iCollectPort,
                m_transFileList[m_currentIndex].m_strCollectFileName.toLocal8Bit().toStdString().c_str());
        sprintf(usrPwd, "%s:%s", m_strCollectUserName.toLocal8Bit().toStdString().c_str(), m_strCollectPasswd.toLocal8Bit().toStdString().c_str());

        if (m_pFtp->downloadFile(ftpUrl, usrPwd, &mData) != 0)
        {
            dealSendError("getFromFtp failed");

            //emit doTask();
            return;
        }
    }
    else if (0 == m_transFileList[m_currentIndex].m_iCollectWay) //目录收集
    {
        if (!collectFromDir(mData))
        {
            dealSendError("Fail to collectFromDir");
            //emit doTask();
            return;
        }
    }
    else
    {
        QSLOG_DEBUG("dealUnsupportSendWay.");
        dealUnsupportCollectWay();
        //emit doTask();
        return;
    }

    // 发送50%进度
    m_transFileList[m_currentIndex].m_getDone = mData.size;
    m_transFileList[m_currentIndex].m_putDone = 0;
    sendTransProgress(m_transFileList[m_currentIndex]);

    // 2.加密压缩
    if (!CryptCurFile(mData))
    {
        dealSendError("Fail to CryptCurFile.");

        //emit doTask();
        return;
    }

    // 3. 分发数据
    if (0 == m_transFileList[m_currentIndex].m_iSendWay)	//目录分发
    {
        loginToSendRemote();	//登录分发路径
        sendToDir(mData);
    }
    else if (1 == m_transFileList[m_currentIndex].m_iSendWay)	//ftp 分发
    {
        sendToFtp(mData);
    }
    else
    {
        QSLOG_DEBUG("dealUnsupportSendWay.");
        dealUnsupportSendWay();
    }

    //emit doTask();

    QSLOG_DEBUG("Finish to processData.");

    return;
}

QString FileDistributeThread::getFtpCdPath()
{
    QString t_strSendID = m_transFileList[m_currentIndex].m_strSendID;
    // 保持目录结构
    if (m_sendUserAcct[t_strSendID].m_bKeepDir && !m_strRltvPath.isEmpty())
    {
        return getCurrentPath();
    }
    QString t_path = m_sendUserAcct[t_strSendID].m_strRltvPath;
    if ('/' != t_path[t_path.length() - 1])
    {
        t_path += "/";
    }
    return t_path;
}

QString FileDistributeThread::getCurrentPath()
{
    QString t_sendId = m_transFileList[m_currentIndex].m_strSendID;
    QString t_strCollectSubPath = getCollectSubPath();//like   dir1/dir2/dir3
    QString t_strSendRltvPath   = m_sendUserAcct[t_sendId].m_strRltvPath;
    if ('/' != t_strSendRltvPath[t_strSendRltvPath.length() - 1])
    {
        t_strSendRltvPath += "/";
    }
    QString t_strCurrentPath = t_strSendRltvPath + t_strCollectSubPath;
    return t_strCurrentPath;
}

QString FileDistributeThread::getCollectSubPath()//like:     subdir/subdir/
{
    QString t_strCollectRltvPath = m_strRltvPath;
    QString t_strFileName     = m_transFileList[m_currentIndex].m_strFileName;
    QString t_strFullPath     = m_transFileList[m_currentIndex].m_strCollectFileName;
    QString t_strSubPath;

    if (t_strCollectRltvPath[t_strCollectRltvPath.length() - 1] != '/')
        t_strCollectRltvPath += '/';
    int t_iBasePathLen = t_strCollectRltvPath.size();
    int t_iFileNameLen = t_strFileName.size();
    t_strSubPath = t_strFullPath.mid(t_iBasePathLen,t_strFullPath.size() - t_iBasePathLen - t_iFileNameLen);

    return t_strSubPath; //like:     subdir/subdir/
}

void FileDistributeThread::cryptAndPutFile()
{
//     QString t_strSendID = m_transFileList[m_currentIndex].m_strSendID;
//
//     QString t_strFileName = m_transFileList[m_currentIndex].m_strFileName;
//     QString t_strTmpName = m_transFileList[m_currentIndex].m_strFileName + ".~tmp";		//临时文件名
//
//     CryptCurFile();			//加密文件内容
//
//     int t_cmdId = 0;
//
//     QMap<QString, QMap<QString,QUrlInfo> >& t_mPathFileList = m_sendUserAcct[m_strLastSendID].m_mPathFileList;
//     QMap<QString,QUrlInfo>& t_nameUrl = t_mPathFileList[m_strLastCdPath];
//
//     if (m_sendUserAcct[t_strSendID].m_bConSend && t_nameUrl.contains(t_strTmpName))
//     {
//         //在此添加断点续传代码
//         //文件大小
//         long t_size = t_nameUrl[t_strTmpName].size();
//         //t_cmdId = m_ftpSend->put(m_transFileList[m_currentIndex].getBuffer(true,t_size).data(), _ToSpecialEncoding(t_strTmpName));
//         // t_cmdId = m_ftpSend->conPut(m_transFileList[m_currentIndex].getBuffer(true,t_size).data(), _ToSpecialEncoding(t_strTmpName));
//     }
//     else
//     {
//         // t_cmdId = m_ftpSend->put(m_transFileList[m_currentIndex].getBuffer().data(), _ToSpecialEncoding(t_strTmpName));
//     }
//     m_sendCmdIdMap[t_cmdId] = "put";
}

// void FileDistributeThread::dealCollectError(const QString& r_errorInfo)
// {
//     m_mutexData->lock();
//     m_transFileList[m_currentIndex].m_status = Failed;///<处理失败
//     m_transFileList[m_currentIndex].m_errorInfo = r_errorInfo;
//     m_transFileList[m_currentIndex].m_strEndDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
//     //	m_transFileList[m_currentIndex].deleteBuffer();
//     delete m_transFileList[m_currentIndex].getBuffer().data();
//
//     if (m_currentIndex < m_transFileList.size())
//         m_currentIndex++;
//     if (m_currentIndex == m_transFileList.size())
//     {
//         m_hasTask = false;
//     }
//     m_mutexData->unlock();
// }

void FileDistributeThread::dealSendError(const QString& r_errorInfo)
{
    m_transFileList[m_currentIndex].m_status = Failed;
    m_transFileList[m_currentIndex].m_errorInfo = r_errorInfo;
    m_transFileList[m_currentIndex].m_strEndDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    sendTransProgress(m_transFileList[m_currentIndex]);

    if (m_currentIndex < m_transFileList.size())
    {
        m_currentIndex++;
    }
    else if (m_currentIndex == m_transFileList.size())
    {
        m_transFileList.clear();
        m_hasTask = false;
    }
}

void FileDistributeThread::sendDone(bool hasError, QString errInfo)
{
    if (!hasError)
    {
        m_transFileList[m_currentIndex].m_status = Finished;	// 成功
        m_transFileList[m_currentIndex].m_errorInfo = "";
    }
    else
    {
        m_transFileList[m_currentIndex].m_status = Failed;		// 失败
        m_transFileList[m_currentIndex].m_errorInfo = errInfo;
    }
    m_transFileList[m_currentIndex].m_strEndDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    sendTransProgress(m_transFileList[m_currentIndex]);

    if (m_currentIndex < m_transFileList.size())
    {
        m_currentIndex++;
    }
    if (m_currentIndex == m_transFileList.size())
    {
        m_transFileList.clear();		// [7/20/2016 wujun]
        m_hasTask = false;
    }
}

void FileDistributeThread::dealUnsupportSendWay()
{
    if (m_currentIndex < m_transFileList.size())
        m_currentIndex++;
    if (m_currentIndex == m_transFileList.size())
    {
        m_transFileList.clear();
        m_hasTask = false;
    }
}

void FileDistributeThread::dealUnsupportCollectWay()
{
    if (m_currentIndex < m_transFileList.size())
        m_currentIndex++;
    if (m_currentIndex == m_transFileList.size())
    {
        m_transFileList.clear();
        m_hasTask = false;
    }
}

bool FileDistributeThread::loginToCollectRemote()
{
    if ("MANUALSUPPLY" == m_strCollectID)//人工补传
    {
        return true;
    }
    if (!m_bDirConnected && m_strCollectUserName.size() > 0)
    {
        if (0 == m_iCollectTryTimes)
        {
            m_strCollectHost = getHostFromFileName(m_transFileList[0].m_strCollectFileName);
            m_iCollectTryTimes++;
            m_bDirConnected = connectToWinRemote(m_strCollectHost,m_strCollectUserName,m_strCollectPasswd);
            if (!m_bDirConnected)
            {
                QMessageBox msgBox;
                string str1 = "connect to "+m_strCollectHost.toStdString()+" error:\nuser="+m_strCollectUserName.toStdString()+",password="+m_strCollectPasswd.toStdString();
                str1+="\nusage \\\\nnn.nnn.nnn.nnn";
                msgBox.setText(str1.c_str());
                msgBox.exec();
                return false;
            }

            return true;
        }
    }
    return m_bDirConnected;
}
bool FileDistributeThread::loginToSendRemote()
{
    QString t_strSendID = m_transFileList[m_currentIndex].m_strSendID;
    bool& t_bDirConnected = m_sendUserAcct[t_strSendID].m_bDirConnected;

    SendAcctInfo& t_ftpInfo = m_sendUserAcct[t_strSendID];
    if (!t_bDirConnected && t_ftpInfo.m_strSendUserName.size() > 0)
    {
        if (0 == t_ftpInfo.m_iSendTryTimes)
        {
            t_ftpInfo.m_strSendHost = getHostFromFileName(m_sendUserAcct[t_strSendID].m_strRltvPath);
            t_ftpInfo.m_iSendTryTimes++;
            t_bDirConnected = connectToWinRemote(t_ftpInfo.m_strSendHost,t_ftpInfo.m_strSendUserName,t_ftpInfo.m_strSendPasswd);
            if (!t_bDirConnected)
            {
                QMessageBox msgBox;
                string str1 = "connect to "+t_ftpInfo.m_strSendHost.toStdString()+" error:\nuser="+t_ftpInfo.m_strSendUserName.toStdString()+",password="+t_ftpInfo.m_strSendPasswd.toStdString();
                str1+="\nusage \\\\nnn.nnn.nnn.nnn";
                msgBox.setText(str1.c_str());
                msgBox.exec();
                return false;
            }
            return true;
        }
    }
    return t_bDirConnected;
}

// bool FileDistributeThread::collectFromDir()
// {
//     QString s = m_transFileList[m_currentIndex].m_strCollectFileName;
//     bool t_bLoad = loadFileToBuffer(m_transFileList[m_currentIndex].m_strCollectFileName
//                                     ,m_transFileList[m_currentIndex].getBuffer(false)
//                                     ,m_transFileList[m_currentIndex].m_errorInfo);
//     if (!t_bLoad)//处理失败
//     {
//         QSLOG_DEBUG("collectFromDir failed");
//         m_transFileList[m_currentIndex].m_status = 2;///<处理失败
//         m_transFileList[m_currentIndex].m_errorInfo = m_strCollectID+":get error:"+m_transFileList[m_currentIndex].m_errorInfo;
//         m_transFileList[m_currentIndex].m_strEndDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
// //		m_transFileList[m_currentIndex].deleteBuffer();
//         delete m_transFileList[m_currentIndex].getBuffer().data();
//
//         if (m_currentIndex < m_transFileList.size())
//             m_currentIndex++;
//         if (m_currentIndex == m_transFileList.size())
//         {
//             m_hasTask = false;
//         }
//         return false;
//     }
//     return true;
// }

bool FileDistributeThread::collectFromDir(MemoryData &mData)
{
    QFile t_in(m_transFileList[m_currentIndex].m_strCollectFileName);
    if (!t_in.open(QIODevice::ReadOnly))
    {
        return false;
    }

    long t_iFileSize = t_in.size();
    m_transFileList[m_currentIndex].m_getDone = 0;
    m_transFileList[m_currentIndex].m_putDone = 0;
    m_transFileList[m_currentIndex].m_totalSize = t_iFileSize;
    sendTransProgress(m_transFileList[m_currentIndex]);

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

bool FileDistributeThread::sendToDir(MemoryData &mData)
{
    QString strKeyIvPath("");
    QString t_strSendID = m_transFileList[m_currentIndex].m_strSendID;
    QString t_fileName("");

    if (m_sendUserAcct[t_strSendID].m_bKeepDir && !m_strRltvPath.isEmpty())
    {
        QString t_strCurrentPath = getCurrentPath();
        QDir t_dir;
        if (!t_dir.exists(t_strCurrentPath))
        {
            t_dir.mkpath(t_strCurrentPath);
        }
        t_fileName = t_strCurrentPath + m_transFileList[m_currentIndex].m_strFileName;

        strKeyIvPath = t_strCurrentPath;
    }
    else
    {
        QString t_subPath = m_sendUserAcct[t_strSendID].m_strRltvPath;
        //////////////////////////////////////////////////////////////////////////
        QDir t_dir;
        if (!t_dir.exists(t_subPath))
        {
            t_dir.mkpath(t_subPath);
        }
        //////////////////////////////////////////////////////////////////////////
        if ('/' != t_subPath[t_subPath.length() - 1])
        {
            t_subPath += "/";
        }
        t_fileName = t_subPath + m_transFileList[m_currentIndex].m_strFileName;

        strKeyIvPath = t_subPath;
    }

    // 写数据
    bool t_bSend = writeFileFromBuffer(t_fileName, mData);
    if (!t_bSend)
    {
        dealSendError("sendToDir error.");
    }
    else
    {
        //----发送key和iv----
        if (m_sendUserAcct[t_strSendID].m_bCompress || m_sendUserAcct[t_strSendID].m_bEncrypt)
        {
            strKeyIvPath += m_transFileList[m_currentIndex].m_strFileName + ".key";
            writeKeyIv(strKeyIvPath, m_sendUserAcct[t_strSendID].m_keyiv);
        }
        //-------------------

        //m_transFileList[m_currentIndex].m_status = Finished;///<处理完成
        // 为了显示100%，暂时不设置为完成
        m_transFileList[m_currentIndex].m_putDone = m_transFileList[m_currentIndex].m_getDone;
        m_transFileList[m_currentIndex].m_errorInfo = "";
        //m_transFileList[m_currentIndex].m_strEndDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        sendTransProgress(m_transFileList[m_currentIndex]);

        // 处理完成
        QThread::msleep(30);
        m_transFileList[m_currentIndex].m_status = Finished;
        m_transFileList[m_currentIndex].m_errorInfo = "";
        m_transFileList[m_currentIndex].m_strEndDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        sendTransProgress(m_transFileList[m_currentIndex]);

        if (m_currentIndex < m_transFileList.size())
        {
            m_currentIndex++;
        }

        if (m_currentIndex == m_transFileList.size())
        {
            m_transFileList.clear();
            m_hasTask = false;
        }
    }
    return t_bSend;
}

void FileDistributeThread::SetParent(void *parent,bool sendMsg)
{
    m_parent = parent;
    m_bSendSignalToMainWindow = sendMsg;
    if (NULL != m_parent && m_bSendSignalToMainWindow)
    {
        connect(this, SIGNAL(transProgress(const TransFileInfo)), (QObject*)m_parent, SIGNAL(transProgress(const TransFileInfo)));
    }
}
void FileDistributeThread::setSendMsg(bool r_bSend)
{
    m_bSendSignalToMainWindow = r_bSend;
    if (NULL != m_parent && m_bSendSignalToMainWindow)
    {
        connect(this        ,SIGNAL(transProgress(const TransFileInfo)),(QObject*)m_parent,SIGNAL(transProgress(const TransFileInfo)));
    }
    else
    {
        disconnect(this        ,SIGNAL(transProgress(const TransFileInfo)),(QObject*)m_parent,SIGNAL(transProgress(const TransFileInfo)));
    }
}

void FileDistributeThread::sendTransProgress(TransFileInfo &tsInfo)
{
    if (NULL != m_parent && m_bSendSignalToMainWindow)
    {
        if (tsInfo.m_totalSize < tsInfo.m_getDone)
        {
            tsInfo.m_totalSize = tsInfo.m_getDone;
        }
        tsInfo.m_strEndDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        emit transProgress(tsInfo);
    }
}

void FileDistributeThread::run()
{
//     qDebug() << "FileDistributeThread::run() run in thread: " << QThread::currentThread();
//     while (1)
//     {
//         m_mutex->lock();
//         if (m_stoped)
//         {
//             m_stoped = false;
//             m_hasTask = false;
//             m_mutex->unlock();
//             break;
//         }
//         else
//         {
//             processData();
//         }
//         m_mutex->unlock();
//         msleep(500);
//     }
}

bool FileDistributeThread::writeFileFromBuffer(const QString &fileName, const MemoryData &mData)
{
    QString strTmpName = fileName + ".~tmp";
    QFile w_file(strTmpName);
    char *pStart = mData.memdata;
    size_t tSize = mData.size;
    //////////////////////////////////////////////////////////////////////////
    // 共享目录断点续传
    QString t_sendId = m_transFileList[m_currentIndex].m_strSendID;
    if (m_sendUserAcct[t_sendId].m_bConSend)
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
    //////////////////////////////////////////////////////////////////////////
    qint64 wSize = w_file.write(pStart, tSize);
//     if (wSize != tSize)
//     {
//
//     }
    w_file.close();

    // 删除原来文件
    if (QFile::exists(fileName))
    {
        QFile::remove(fileName);
    }
    w_file.rename(fileName);

    return true;
}

// 写密钥
bool FileDistributeThread::writeKeyIv(const QString &fileName, KeyIv &keyiv)
{
    QString strTmpName = fileName + ".~tmp";
    QFile t_out(strTmpName);
    if (!t_out.open(QIODevice::WriteOnly))
    {
        QSLOG_ERROR("writeKeyIv open error.");
        return false;
    }

    QDataStream out(&t_out);
    out << keyiv.bComp;
    out << keyiv.bCrypt;
    out << keyiv.key;
    out << keyiv.iv;
    t_out.close();

    // 删除原来key文件
    if (QFile::exists(fileName))
    {
        QFile::remove(fileName);
    }

    return t_out.rename(fileName);
}

bool FileDistributeThread::connectToWinRemote(const QString& r_resource,const QString& r_user,const QString& r_passwd)
{
    // 解决本地文件夹，返回错误码67的问题
    QDir qdir(r_resource);
    if (qdir.exists())
    {
        return true;
    }
    //////////////////////////////////////////////////////////////////////////

    char t_cResource[1024];
    memset((void *)t_cResource,0,sizeof(t_cResource));
    strcpy(t_cResource,r_resource.toStdString().c_str());

    NETRESOURCEA t_resource;
    t_resource.lpRemoteName = (LPSTR)t_cResource;
    t_resource.dwType=RESOURCETYPE_DISK;
    t_resource.dwDisplayType=0;
    t_resource.dwScope=0;
    t_resource.dwUsage=0;
    t_resource.lpComment="";
    t_resource.lpLocalName="";
    t_resource.lpProvider="";

    DWORD ret = WNetAddConnection2A(&t_resource,(LPCSTR)(r_passwd.toStdString().c_str()),(LPCSTR)(r_user.toStdString().c_str()),0);

    if (NO_ERROR == ret || 1219 == ret)
        return true;

    return false;
}

QString FileDistributeThread::getHostFromFileName(const QString& r_strFullPathFileName)
{
    string t_file = r_strFullPathFileName.toStdString();
    size_t t_index = t_file.find('/',2);
    if (t_index == string::npos)
        return "";
    QString ret="\\\\";
    ret += t_file.substr(2, t_index - 2).c_str();
    return ret;
}

QString FileDistributeThread::getRightCutPath(const QString& r_path)
{
    int icnt = r_path.size();
    int iendPos = icnt - 1;
    for (int i = icnt - 2; i>=0; i--)
    {
        if ('/' == r_path[i])
        {
            iendPos = i;
            break;
        }
    }
    return r_path.mid(0, iendPos + 1);
}

// void FileDistributeThread::ftpGetDataProgress(qint64 done, qint64 total)
// {
//     m_transFileList[m_currentIndex].m_getDone = done;
//     m_transFileList[m_currentIndex].m_totalSize = total;
//     sendTransProgress(m_transFileList[m_currentIndex]);
// }
// void FileDistributeThread::ftpPutDataProgress(qint64 done, qint64 total)
// {
//     m_transFileList[m_currentIndex].m_putDone = done;
//     m_transFileList[m_currentIndex].m_totalSize = total;
//     sendTransProgress(m_transFileList[m_currentIndex]);
// }

void FileDistributeThread::ftpTransProgress(qint64 dltotal, qint64 dlnow, qint64 ultotal, qint64 ulnow)
{
    // added by liubojun @20160804,下标越界
    if (m_currentIndex >= m_transFileList.size())
    {
        return;
    }
    m_transFileList[m_currentIndex].m_nSpeed = (m_transFileList[m_currentIndex].m_getDone + m_transFileList[m_currentIndex].m_putDone) / m_pFtp->m_nPgtime;
    // 下载
    if (dltotal != 0)
    {
        m_transFileList[m_currentIndex].m_nSpeed = (dlnow - m_transFileList[m_currentIndex].m_getDone) / m_pFtp->m_nPgtime;
        m_transFileList[m_currentIndex].m_getDone = dlnow;
        m_transFileList[m_currentIndex].m_totalSize = dltotal;
        sendTransProgress(m_transFileList[m_currentIndex]);
    }
    // 上传
    else if (ultotal != 0)
    {
        m_transFileList[m_currentIndex].m_nSpeed = (ulnow - m_transFileList[m_currentIndex].m_putDone) / m_pFtp->m_nPgtime;
        m_transFileList[m_currentIndex].m_putDone = ulnow;
        m_transFileList[m_currentIndex].m_totalSize = ultotal;
        sendTransProgress(m_transFileList[m_currentIndex]);
    }
}

void FileDistributeThread::setFileCrypt(IFileCrypt *pFileCrypt)
{
    m_pFileCrypt = pFileCrypt;
}

bool FileDistributeThread::CryptCurFile(MemoryData &mData)
{
    if (m_pFileCrypt == NULL)
    {
        return false;
    }

    QString t_strSendID = m_transFileList[m_currentIndex].m_strSendID;
    bool bComp = m_sendUserAcct[t_strSendID].m_bCompress;
    bool bCrypt = m_sendUserAcct[t_strSendID].m_bEncrypt;

    QByteArray baOrg, baResult;
    KeyIv &keyiv = m_sendUserAcct[t_strSendID].m_keyiv;
    keyiv.bComp = bComp;
    keyiv.bCrypt = bCrypt;

    //baOrg.setRawData(mData.memdata, mData.size);	//包含指针，不copy
    baOrg.append(mData.memdata, mData.size);
    if (m_pFileCrypt->CompCryptFile(baOrg, baResult, keyiv, bComp, bCrypt))
    {
        free(mData.memdata);
        mData.size = baResult.size();
        mData.memdata = (char *)malloc(mData.size);
        memcpy(mData.memdata, baResult.data(), mData.size);
    }
    else
    {
        QSLOG_ERROR("CryptCurFile failed.");
        return false;
    }

    return true;
}

bool FileDistributeThread::sendToFtp(MemoryData &mData)
{
    QString t_sendId = m_transFileList[m_currentIndex].m_strSendID;
    string strIp = m_sendUserAcct[t_sendId].m_strSendHost.toStdString();
    int nPort = m_sendUserAcct[t_sendId].m_iSendPort;
    string strUsr = m_sendUserAcct[t_sendId].m_strSendUserName.toStdString();
    string strPwd = m_sendUserAcct[t_sendId].m_strSendPasswd.toStdString();
    string strPath = getFtpCdPath().toStdString();
    char ftpUrl[200] = {0};
    char usrPwd[100] = {0};
    sprintf(ftpUrl, "ftp://%s:%d%s", strIp.c_str(), nPort, strPath.c_str());
    sprintf(usrPwd, "%s:%s", strUsr.c_str(), strPwd.c_str());
    //////////////////////////////////////////////////////////////////////////
    //----断点续传功能----
    int bRet = -1;
    if (m_sendUserAcct[t_sendId].m_bConSend)
    {
        bRet = m_pFtp->conputFile(ftpUrl, usrPwd, &mData, m_transFileList[m_currentIndex].m_strFileName.toStdString());
    }
    else
    {
        bRet = m_pFtp->uploadFile(ftpUrl, usrPwd, &mData, m_transFileList[m_currentIndex].m_strFileName.toStdString());
    }
    if (bRet != 0)
    {
        dealSendError("Fail to uploadFile to ftp.");
        return false;
    }
    else
    {
        // 为了显示100%，暂时不设置为完成
        m_transFileList[m_currentIndex].m_putDone = m_transFileList[m_currentIndex].m_getDone;
        m_transFileList[m_currentIndex].m_errorInfo = "";
        sendTransProgress(m_transFileList[m_currentIndex]);

        //----发送key和iv（若不加密不压缩就不发送key文件）----
        if (m_sendUserAcct[t_sendId].m_bCompress || m_sendUserAcct[t_sendId].m_bEncrypt)
        {
            string strKeyIvPath = m_transFileList[m_currentIndex].m_strFileName.toStdString() + ".key";
            QByteArray baKey;
            QDataStream out(&baKey, QIODevice::WriteOnly);
            //KeyIv &keyiv = m_sendUserAcct[t_sendId].m_keyiv;
            out << m_sendUserAcct[t_sendId].m_keyiv.bComp;
            out << m_sendUserAcct[t_sendId].m_keyiv.bCrypt;
            out << m_sendUserAcct[t_sendId].m_keyiv.key;
            out << m_sendUserAcct[t_sendId].m_keyiv.iv;
            MemoryData mkey;
            mkey.size = baKey.size();
            mkey.memdata = (char *)malloc(mkey.size + 1);
            memcpy(mkey.memdata, baKey.data(), mkey.size);
            mkey.memdata[mkey.size] = 0;
            if (m_pFtp->uploadFile(ftpUrl, usrPwd, &mkey, strKeyIvPath) != 0)
            {
                QSLOG_ERROR(QString("Fail to upload %1.").arg(QString::fromStdString(strKeyIvPath)));
            }
        }
        //-------------

        QThread::msleep(30);
        m_transFileList[m_currentIndex].m_status = Finished;		///<处理完成
        m_transFileList[m_currentIndex].m_errorInfo = "";
        m_transFileList[m_currentIndex].m_strEndDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        sendTransProgress(m_transFileList[m_currentIndex]);

        if (m_currentIndex < m_transFileList.size())
        {
            m_currentIndex++;
        }

        if (m_currentIndex == m_transFileList.size())
        {
            m_transFileList.clear();
            m_hasTask = false;
        }

        return true;
    }
}

void FileDistributeThread::setCollectAcctInfo(const QString& r_id ,const QString& r_host ,const QString& r_user ,const QString& r_passwd ,int r_port ,const QString& r_rltvPath)
{
    QMutexLocker locker(m_mutexData.data());
    m_strCollectID = r_id;
    m_strCollectHost = r_host;
    m_strCollectUserName = r_user;
    m_strCollectPasswd = r_passwd;
    m_iCollectPort = r_port;
    m_strRltvPath = r_rltvPath;
}

void FileDistributeThread::dealError(TransFileInfo &fileInfo, QString &errorInfo)
{
    QSLOG_ERROR(errorInfo);
    fileInfo.m_status = Failed;
    fileInfo.m_errorInfo = errorInfo;
    fileInfo.m_strEndDateTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    sendTransProgress(fileInfo);
}

bool FileDistributeThread::getUserInfo(const QString userID, SendAcctInfo &userInfo)
{
    if (!m_sendUserAcct.contains(userID))
    {
        return false;
    }

    userInfo = m_sendUserAcct[userID];
    return true;
}

void FileDistributeThread::AddSendTask(const TransferSet & tSet)
{
    // 获取用户信息
    for (unsigned int i = 0; i < tSet.vecRcvers.size(); i++)
    {
        SendAcctInfo t_info;
        t_info.m_strSendID = QString::fromStdString(tSet.vecRcvers[i].strUsrname);
        t_info.m_strSendHost = tSet.vecRcvers[i].strIP.c_str();
        t_info.m_strSendUserName = tSet.vecRcvers[i].strUsr.c_str();
        t_info.m_strSendPasswd = tSet.vecRcvers[i].strPwd.c_str();
        t_info.m_strRltvPath = tSet.vecRcvers[i].strRltvPath.c_str();
        t_info.m_iSendPort = tSet.vecRcvers[i].nPort;
        t_info.m_iSendWay = tSet.vecRcvers[i].nSendWay;
        t_info.m_bEncrypt = tSet.vecRcvers[i].bEncrypt;
        t_info.m_bCompress = tSet.vecRcvers[i].bCompress;
        t_info.m_bKeepDir = tSet.vecRcvers[i].bKeepDir;
        t_info.m_bConSend = tSet.vecRcvers[i].bConSend;

        m_sendUserAcct[t_info.m_strSendID] = t_info;
    }

    for (int i=0; i<tSet.fileList.size(); ++i)
    {
        TransFileInfo tInfo;
        tInfo.m_strCollectID = QString::fromStdString(tSet.strName);
        tInfo.m_strCollectPath = QString::fromStdString(tSet.strPath);
        tInfo.m_iCollectWay = tSet.nCollectWay;
        tInfo.m_strCollectIP = tSet.strIP;
        tInfo.m_iCollectPort = tSet.nPort;
        tInfo.m_strCollectUser = tSet.strUsr;
        tInfo.m_strCollectPwd = tSet.strPwd;
        tInfo.m_strFileName =  QString::fromLocal8Bit(tSet.fileList.at(i).strFileName.c_str());
        tInfo.m_strCollectFileName = QString::fromLocal8Bit(tSet.fileList.at(i).strFilePath.c_str());
        for (uint j=0; j<tSet.vecRecvID.size(); ++j)
        {
            tInfo.m_strUsers.append(QString::fromStdString(tSet.vecRecvID.at(j)));
        }
        tInfo.m_status = UnHandled;
        tInfo.m_nSendFlag = tSet.sendFlag;

        DistributeTaskThread *task = new DistributeTaskThread(this, tInfo);
        QThreadPool::globalInstance()->start(task);
    }
}
