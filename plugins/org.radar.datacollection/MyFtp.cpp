#include "MyFtp.h"
#include "CollectorBase.h"
#include <QDebug>

MyFtp::MyFtp(QObject *parent)
    : m_bConnected(false)
{
    m_pFile = new QFile("bbb.txt");
    bool bFlag = m_pFile->open(QIODevice::ReadWrite);
    m_pFtp = new QFtp(this);
    m_pFtp->connectToHost("192.168.21.28", 21);
    m_pFtp->login("nriet", "123456");
    m_pFtp->cd("data1");
    if (bFlag)
    {
        m_pFtp->get("aaa.txt", m_pFile);
    }
    m_pFtp->close();
    //当每条命令开始执行时发出相应的信号
    connect(m_pFtp, SIGNAL(commandStarted(int)), this, SLOT(ftpCmdStart(int)));
    //当每条命令执行结束时发出相应的信号
    connect(m_pFtp, SIGNAL(commandFinished(int, bool)), this, SLOT(ftpCmdFinish(int,bool)));
    connect(m_pFtp, SIGNAL(listInfo(const QUrlInfo &)), this, SLOT(ftpListInfo(const QUrlInfo &)));
    //connect(m_pFtp, SIGNAL(done(bool)), this, SLOT(ftpDone()));
}

MyFtp::MyFtp(CollectorBase *parent)
    : m_bConnected(false)
      //, m_strCurDirNewTime("")
    , m_strCurDirNewTime(0)
{
    m_pParent = parent;

    m_pFtp = new QFtp(this);
    //当每条命令开始执行时发出相应的信号
    connect(m_pFtp, SIGNAL(commandStarted(int)), this, SLOT(ftpCmdStart(int)));
    //当每条命令执行结束时发出相应的信号
    connect(m_pFtp, SIGNAL(commandFinished(int, bool)), this, SLOT(ftpCmdFinish(int,bool)));
    //获取当前目录下的内容
    connect(m_pFtp, SIGNAL(listInfo(const QUrlInfo &)), this, SLOT(ftpListInfo(const QUrlInfo &)));
}

MyFtp::~MyFtp()
{

}

void MyFtp::ftpCmdStart(int)
{
    switch (m_pFtp->currentCommand())
    {
    case QFtp::ConnectToHost:
        qDebug() << QString::fromLocal8Bit("正在连接到服务器...");
        break;
    case QFtp::Login:
        qDebug() << QString::fromLocal8Bit("正在登录...");
        break;
    case QFtp::Get:
        qDebug() << QString::fromLocal8Bit("正在下载...");
        break;
    case QFtp::Close:
        qDebug() << "正在关闭连接...";
        break;
    case QFtp::List:
        break;
    default:
        break;
    }
}

void MyFtp::ftpCmdFinish(int, bool err)
{
    switch (m_pFtp->currentCommand())
    {
    case QFtp::ConnectToHost:
    {
        if (err)
        {
            qDebug() << QString("连接服务器出错：%1").arg(m_pFtp->errorString());
        }
        else
        {
            qDebug() << "连接服务器成功";
        }
        break;
    }
    case QFtp::Login:
        if (err)
        {
            qDebug() << QString("登录出错：%1").arg(m_pFtp->errorString());
        }
        else
        {
            qDebug() << "登录成功";
            m_bConnected = true;
        }
        break;
    case QFtp::Get:
        if (err)
        {
            qDebug() << QString("下载出错：%1").arg(m_pFtp->errorString());
        }
        else
        {
            qDebug() << "下载完成";
            //m_pFile->close();
        }
        break;
    case QFtp::Close:
        qDebug() << "已经关闭连接";
        break;
    case QFtp::List:
    {
        if (m_strCurDirNewTime > m_strCurDirLastTime)
        {
            m_pParent->m_pTsctTime->mapDirTime[m_strCurDir.toStdString()] = m_strCurDirNewTime;
        }
        scanNextDir();
    }
    break;
    default:
        break;
    }
}

void MyFtp::setHostPort(const QString &strIP, const int &nPort)
{
    m_strIP = strIP;
    m_nPort = nPort;
}

void MyFtp::setUserPwd(const QString &strUser, const QString &strPwd)
{
    m_strUser = strUser;
    m_strPwd = strPwd;
}

void MyFtp::setDataPath(const QString &strPath)
{
    m_strPath = strPath;
    if (m_strPath.isEmpty())
    {
        m_strPath = "/";
    }
}

void MyFtp::getNewFile(FileInfoList &fileList)
{
    if (m_pFtp == NULL)
    {
        emit done();
        return;
    }

    m_pFileList = &fileList;

    m_strCurDir = m_strPath;
    if (m_strCurDir.right(1) != "/")
    {
        m_strCurDir += "/";
        //m_strCurDir = m_strCurDir.left(m_strCurDir.length() - 1);
    }

    if (m_pParent->m_pTsctTime != NULL)
    {
        m_strCurDirLastTime = m_pParent->m_pTsctTime->mapDirTime[m_strCurDir.toStdString()];
    }
    else
    {
        //m_strCurDirLastTime = "0";
        m_strCurDirLastTime = 0;
    }
    m_strCurDirNewTime = m_strCurDirLastTime;

    connectToHost();

    m_lstDirs.clear();
    m_pFtp->list();
}

// void MyFtp::getFiles(const QString &dirPath, FileInfoList &fileList)
// {
//
// }

void MyFtp::ftpListInfo(const QUrlInfo &info)
{
    if (info.isFile())
    {
        FileInfo fileIf;
        //fileIf.strFileName = QString::fromLocal8Bit(info.name().toLatin1()).toLocal8Bit().toStdString();	//失效了??
        QString strTmp = info.name().toLatin1();		//防止中文乱码
        fileIf.strFileName = strTmp.toLocal8Bit();
        //fileIf.strFileName = QString::fromLocal8Bit(strTmp.toLocal8Bit()).toStdString();
        //文件名过滤
        if (!m_pParent->filterFileName(fileIf))
        {
            return;
        }
        //时间比对（有问题，不能获取到秒级??）
        //string strMdyTime =info.lastModified().toString("yyyyMMddHHmmss").toStdString();
        uint strMdyTime = info.lastModified().toTime_t();
        if (strMdyTime > m_strCurDirLastTime)
        {
            //fileIf.strFilePath = QString::fromLocal8Bit((m_strCurDir + info.name()).toLatin1()).toLocal8Bit().toStdString();
            strTmp = (m_strCurDir + info.name()).toLatin1();
            fileIf.strFilePath = strTmp.toLocal8Bit();
            fileIf.nFileSize = info.size();
            m_pFileList->push_back(fileIf);

            if (m_strCurDirNewTime < strMdyTime)
            {
                m_strCurDirNewTime = strMdyTime;
            }
        }
    }
    else if (info.isDir() && !info.isSymLink())
    {
        ////m_lstDirs.append(m_strCurDir + "/" + info.name());
        ////uint nMdyTime = info.lastModified().toTime_t();
        //string strMdyTime =info.lastModified().toString("yyyyMMddHHmmss").toStdString();
        //string strLastTime = m_pParent->m_pTsctTime->mapDirTime[(m_strCurDir + info.name() + "/").toStdString()];
        uint strMdyTime = info.lastModified().toTime_t();
        uint strLastTime = m_pParent->m_pTsctTime->mapDirTime[(m_strCurDir + info.name() + "/").toStdString()];
        if (strMdyTime > strLastTime)
        {
            m_lstDirs.append(m_strCurDir + info.name() + "/");
        }
    }
}

void MyFtp::scanNextDir()
{
    if (!m_lstDirs.isEmpty())
    {
        m_strCurDir = m_lstDirs.takeFirst();
        m_pFtp->cd(m_strCurDir);
        m_strCurDirLastTime = m_pParent->m_pTsctTime->mapDirTime[m_strCurDir.toStdString()];
        m_pFtp->list();
        qDebug()<<QString("List %1").arg(m_strCurDir);
    }
    else
    {
        m_pFtp->close();
        m_bConnected = false;
        emit done();
    }
}

void MyFtp::connectToHost()
{
    if (!m_bConnected)
    {
        m_pFtp->connectToHost(m_strIP, m_nPort);
        m_pFtp->login(m_strUser, m_strPwd);
        m_pFtp->cd(m_strPath);
    }
}
