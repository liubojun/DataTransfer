#include "CollectHistory.h"
#include "IClientMessage.h"
#include "IRadarPathAdapter.h"
#include "pathbuilder.h"
#include "ConnectConfig.h"
#include "ctkPublicFun.h"
#include "IRadarMosaic.h"
#include <QTimer>

CollectHistory::CollectHistory(ICtkPluginManager *pManager, QObject *pParent/*=NULL*/)
    : m_pManager(pManager)
    , QObject(pParent)
    , m_bRun(true)
{
    m_pThread = QSharedPointer<QThread>(new QThread());
    this->moveToThread(m_pThread.data());
    m_pThread->start();

    connectMsgServer();		//连接消息服务器

    QObject *op = m_pManager->getService("IRadarPathAdapter");
    m_pMakePath = (qobject_cast<IRadarPathAdapter *>(op))->createRadarProductPath();
}

CollectHistory::~CollectHistory()
{

}

bool CollectHistory::connectMsgServer()
{
    QObject *op = m_pManager->getService("IClientMessage");
    m_pIClient = (qobject_cast<IClientMessage *>(op))->createClient();
    if (m_pIClient != NULL)
    {
        CPathBuilder pbd;
        QString qsXmlPath = pbd.getConfigPath() + "/ClientInfo.xml";
        if (QFile::exists(qsXmlPath))
        {
            ClientConfig cConfig;
            if (DeserializeXmlFile2Client(QString(qsXmlPath.toLocal8Bit()).toStdString(), cConfig))
            {
                if (m_pIClient->connect(/*cConfig.m_nType,*/ cConfig.m_sIP, cConfig.m_nPort))
                {
                    return true;
                }
            }
        }
    }

    QSLOG_ERROR("CollectorBase failed to connectMsgServer");
    return false;
}

bool CollectHistory::IsHistyFile(const FileInfo &fileInfo)
{
    if (fileInfo.strMdyTime < m_strStartTime.toStdString() ||
            fileInfo.strMdyTime > m_strEndTime.toStdString())
    {
        return false;
    }

    return true;
}

int CollectHistory::start()
{
    // 防止卡在主线程
    QTimer::singleShot(500, this, SLOT(getHistoryFiles()));
    return 0;
}

int CollectHistory::stop()
{
    m_bRun = false;
    return 0;
}

void CollectHistory::getHistoryFiles()
{
    QSLOG_INFO(QString("Begin to getHistoryFiles from %1 .").arg(m_strPath));

    FileInfoList fileList;

    switch (m_nType)
    {
    case 0:		//单站归档
    {
        m_strCurStation = m_lstStations.first().toStdString();
        QSet<QString> strlstPath;
        getFilePath(strlstPath);
        foreach (QString strPath, strlstPath)
        {
            getAllFiles(fileList, strPath);
            if (!m_bRun)
            {
                return;
            }
        }
    }
    break;
    case 1:		//组网归档
    {
        QSet<QString> setDirPath;
        for (int i=0; i<m_lstStations.size(); ++i)
        {
            setDirPath.clear();
            m_strCurStation = m_lstStations.at(i).toStdString();
            getFilePath(setDirPath);
            foreach (QString strPath, setDirPath)
            {
                getAllFiles(fileList, strPath);
            }

            if (!m_bRun)
            {
                return;
            }
        }
    }
    break;
    case 2:		//高空归档
        break;
    default:
        break;
    }

    QSLOG_INFO(QString("Total %1 HistoryFiles from %2 .").arg(fileList.size()).arg(m_strPath));

    if (fileList.size() < 1)
    {
        return;
    }

    MSGSTRUCT msg;
    msg.head.m_msgid = 10000;	//临时
    msg.head.m_queueid = 10;
    for (int i=0; i<fileList.size(); ++i)
    {
        msg.head.m_msgtime = time(NULL);
        FileInfoToString(msg.body.m_msgdata, fileList.at(i));
        msg.head.m_queuename = m_strQueName.toStdString();
        if (!m_pIClient->pushMsg(msg))
        {
            QSLOG_ERROR(QString("popMsg error."));
        }
    }

    if (m_nType == 1)
    {
        //调用组网插件处理
        if ( m_pManager != NULL )
        {
            QObject* pOb = m_pManager->getService("IRadarMosaic");

            if ( pOb != NULL )
            {
                IRadarMosaic* pRM = qobject_cast<IRadarMosaic*>(pOb);

                if ( pRM != NULL )
                {
                    pRM->archive_start();
                }
            }
        }
    }

    QSLOG_INFO(QString("Finish to Send HistoryFiles from %1 .").arg(m_strPath));
}

void CollectHistory::getAllFiles(FileInfoList &fileList, QString strPath)
{
    QDir qdir(strPath);
    if (!qdir.exists())
    {
        return;
    }

    QFileInfoList qsList = qdir.entryInfoList();
    for (int i=0; i<qsList.size(); ++i)
    {
        QFileInfo qfInfo = qsList.at(i);
        if (qfInfo.fileName() == "." || qfInfo.fileName() == "..")
        {
            continue;
        }

        if (qfInfo.isFile())
        {
            FileInfo fileinfo;
            if (filterFileName(qfInfo.filePath(), fileinfo))
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

void CollectHistory::getStationPath(QStringList &strlstPath)
{
    QString strStaionRootPath;
    for (int i=0; i<m_lstStations.size(); ++i)
    {
        strStaionRootPath = m_strPath + "/" + m_lstStations.at(i);
        strlstPath.push_back(strStaionRootPath);
    }
}

bool CollectHistory::filterFileName(QString strFilePath, FileInfo &fileInfo)
{
    QFileInfo qfileInfo(strFilePath);

    if (!m_pMakePath.isNull())
    {
        QString strFileTime, strStation;
        FileNameRule fnr;
        fnr.type = VtbType;
        m_pMakePath->getFileInfo(strFilePath, fnr);
        strFileTime = fnr.dTime;
        strStation = fnr.station;
        //判断文件时间是否符合
        fileInfo.strFileTime = strFileTime.toStdString();
        if (fileInfo.strFileTime < m_strStartTime.toStdString() ||
                fileInfo.strFileTime > m_strEndTime.toStdString())
        {
            return false;
        }
        //完善文件信息
        fileInfo.strMdyTime = qfileInfo.lastModified().toString("yyyyMMddHHmmss").toStdString();
        fileInfo.strFilePath = qfileInfo.filePath().replace("\\", "/").toLocal8Bit().data();
        fileInfo.strFileName = qfileInfo.fileName().toLocal8Bit().data();
        //fileInfo.strStation = m_strCurStation;
        fileInfo.strStation = strStation.toStdString();
        fileInfo.strFileType = "VTB_NEW";	//默认为这种类型??
        getRadarFileType(fileInfo.strFileType, fileInfo.nFileType);

        return true;
    }

    return false;
}

// void CollectHistory::getFilePath(const QString &strRootPath, QStringList &strlstPath)
// {
// 	QString strYear = m_strStartTime.left(4);
// 	QString strPath = strRootPath + "/" + strYear;
// 	QDir qdir(strPath);
// 	if (!qdir.exists())
// 	{
// 		// 实时数据路径（三天内）
// 		//strlstPath.push_back(strRootPath);
// 		QSLOG_ERROR("getFilePath error.");
// 		return;
// 	}
// 	else
// 	{
// 		//调用路径适配插件获取路径规则
// 		// 历史数据路径
// 		QDate qdStart = QDate::fromString(m_strStartTime.left(8), "yyyyMMdd");
// 		QDate qdEnd = QDate::fromString(m_strEndTime.left(8), "yyyyMMdd");
// 		QString strDate;
// 		for (QDate d=qdStart; d<=qdEnd; d=d.addDays(1))
// 		{
// 			strDate = d.toString("yyyyMMdd");
// 			strPath = strRootPath + "/" + strDate.left(4) + "/" + strDate.left(6) + "/" + strDate;
// 			if (qdir.exists(strPath))
// 			{
// 				strlstPath.push_back(strPath);
// 			}
// 		}
// 	}
// }

void CollectHistory::getFilePath(QSet<QString> &setPathes)
{
    QDate qdStart = QDate::fromString(m_strStartTime.left(8), "yyyyMMdd");
    QDate qdEnd = QDate::fromString(m_strEndTime.left(8), "yyyyMMdd");
    QString strDirPath;
    QDir qdir;
    for (QDate d=qdStart; d<=qdEnd; d=d.addDays(1))
    {
        strDirPath = m_pMakePath->getVtbDirPath(d, QString::fromStdString(m_strCurStation));
        if (qdir.exists(strDirPath))
        {
            setPathes.insert(strDirPath);
        }
    }
}
