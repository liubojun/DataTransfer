#include "CollectManager.h"
#include "shareddirCollector.h"
#include "ftpCollector.h"
#include "pathbuilder.h"
#include "ConnectConfig.h"
#include "IClientMessage.h"
#include "ctkPublicFun.h"
#include <QSettings>


CollectManager::CollectManager(ICtkPluginManager *pCtkManager, QWaitCondition &in_oCond, QMutex &in_oLocker, int &logsize)
    : m_pCtkManager(pCtkManager)
    , m_vtbMaxNum(1),
      m_oCond(in_oCond),
      m_oLocker(in_oLocker),
      m_iLogsize(logsize)

{
    m_bFirstReadTcXml = true;
    connectMsgServer();		//连接消息服务器

    m_pThread = QSharedPointer<QThread>(new QThread(this));
    QObject::connect(m_pThread.data(), SIGNAL(finished()), m_pThread.data(), SLOT(deleteLater()));
    m_pThread->start();

    readThreadNum();

    QThreadPool::globalInstance()->setMaxThreadCount(8);
}

CollectManager::~CollectManager()
{
    stopAllCollection();
    m_pThread->quit();
    m_pThread->wait();
    QSLOG_INFO("~CollectManager()");
}

void CollectManager::setXmlPath(const QString &collectSetPath, const QString &lastTimePath)
{
    m_strCollectXmlPath = collectSetPath;
    m_strTimeXmlPath = lastTimePath;
    readCollectSet();
    readLastTime();
}

// QSharedPointer<CollectorBase> CollectManager::creatCollector(collection_type type)
CollectorBase *CollectManager::creatCollector(collection_type type)
{
    // QSharedPointer<CollectorBase> pCollect;
    CollectorBase *pCollect = NULL;
    switch (type)
    {
    case SHARED_DIR:
        // pCollect = QSharedPointer<CollectorBase>(new SharedDirCollector(this, m_oCond, m_oLocker, m_iLogsize));
        pCollect = new SharedDirCollector(this, m_oCond, m_oLocker, m_iLogsize);
        break;
    case FTP:
        //pCollect = QSharedPointer<CollectorBase>(new FtpCollector(this, m_oCond, m_oLocker, m_iLogsize));
        pCollect = new FtpCollector(this, m_oCond, m_oLocker, m_iLogsize);
        break;
    default:
        break;
    }

    // if (!pCollect.isNull())
    {
        // connect(pCollect.data(), SIGNAL(newTransFile(const TransferSet&)), this, SIGNAL(newTransFile(const TransferSet&)));
        connect(pCollect, SIGNAL(newTransFile(const TransferSet&)), this, SIGNAL(newTransFile(const TransferSet&)));
    }

    return pCollect;
}

bool CollectManager::addCollect(const CollectSet &set)
{
//     if (!set.bEnable)
//     {
//         return false;
//     }
//
//     QString strPath = QString::fromStdString(set.strPath);
//     QFileInfo qfInfo(strPath);
//     if (!qfInfo.isDir())
//     {
//         emit print(QStringLiteral("[%1]: 目录[%2]不能访问或不存在。").arg(QDateTime::currentDateTime().toString(Qt::ISODate))
//                    .arg(strPath));
//
//         //QSLOG_ERROR(QString("[%1] is not a dir.").arg(strPath));
//         return false;
//     }
//
//     if (m_pCollects.contains(set.strId))
//     {
//         QSLOG_ERROR("Same id.");
//         return false;
//     }
//
//     QSharedPointer<CollectorBase> pCollect = creatCollector(SHARED_DIR);
//     // 绑定log信号槽
//     connect(pCollect.data(), SIGNAL(print(const QString&)), this, SIGNAL(print(const QString&)));
//     pCollect->setNameID(QString::fromStdString(set.strId));
//     pCollect->setWatchType(watch_type(set.nWtype));
//     pCollect->setUrl(strPath);
//     pCollect->setQuartzRule(QString::fromStdString(set.strDispatch));
//     FILENAME_TABLE fTable;
//     ToFileTable(set.mapTypeRegs, fTable);
//     pCollect->setFileNameRules(fTable);
//     //pCollect->setQueNames(set.mapTypeRegs);
//     pCollect->setQueNames(set.vecQueNames);
//     ///pCollect->start(m_pThread);
//
//     m_pCollects[set.strId] = pCollect;

    return true;
}

void CollectManager::delCollect(const string &strID)
{
//     if (!m_pCollects.contains(strID))
//     {
//         return;
//     }
//     // 停止收集
//     QSharedPointer<CollectorBase> pCollect = m_pCollects.take(strID);
//     if (!pCollect.isNull())
//     {
//         pCollect->stop();
//     }
//     // 删除时间
//     delLastTime(strID);
}

void CollectManager::mdfyCollect(const CollectSet &set)
{
//     if (!m_pCollects.contains(set.strId))
//     {
//         return;
//     }
//     QSharedPointer<CollectorBase> pCollect = m_pCollects.take(set.strId);
//     pCollect->stop();
//     addCollect(set);
}

bool CollectManager::delLastTime(string strID)
{
    std::map<string, CollectTime>::iterator it = m_lastTimeList.id_last.find(strID);
    if (it == m_lastTimeList.id_last.end())
    {
        return false;
    }
    m_lastTimeList.id_last.erase(it);

    return writeLastTime();
}

bool CollectManager::readCollectSet()
{
    m_lstCollectSet.lsSets.clear();
    if (!XmlToCollectSet(m_strCollectXmlPath.toLocal8Bit().data(), m_lstCollectSet))
    {
        return false;
    }

    return true;
}

bool CollectManager::readLastTime()
{
    m_lastTimeList.id_last.clear();
    if (!XmlToCollectTime(m_strTimeXmlPath.toStdString(), m_lastTimeList))
    {
        QSLOG_ERROR("Fail to readLastTime.");
        return false;
    }
    return true;
}

bool CollectManager::writeLastTime()
{
    if (m_lastTimeList.id_last.size() > 1)
    {
        int a = 0;
    }
    if (!CollectTimeToXml(m_strTimeXmlPath.toLocal8Bit().data(), m_lastTimeList))
    {
        QSLOG_ERROR("Fail to writeLastTime.");
        return false;
    }
    return true;
}

void CollectManager::ToFileTable(const std::map<string, NameMatchRule > &mapTypeRegs, FILENAME_TABLE &fileTable)
{
    RADAR_FILE_TYPE type;
    QString strReg;
    std::map<string, NameMatchRule>::const_iterator it = mapTypeRegs.begin();
    for (; it != mapTypeRegs.end(); ++it)
    {
        getRadarFileType(it->first, type);
        fileTable[type].strNameReg = it->second.strNameRegExp;
        fileTable[type].nStationIndex = it->second.nStationIndex;
        fileTable[type].nDateTimeIndex = it->second.nTimeIndex;
    }
}

bool CollectManager::connectMsgServer()
{
    QObject *op = m_pCtkManager->getService("IClientMessage");
    if (op == NULL)
    {
        return false;
    }
    //创建消息客户端
    m_pIClient = (qobject_cast<IClientMessage *>(op))->createClient();
    if (m_pIClient != NULL)
    {
        CPathBuilder pbd;
        QString qsXmlPath = pbd.getConfigPath() + "/ClientInfo.xml";
        if (QFile::exists(qsXmlPath))
        {
            ClientConfig cConfig;
            if (DeserializeXmlFile2Client(qsXmlPath.toLocal8Bit().data(), cConfig))
            {
                if (m_pIClient->connect(cConfig.m_sIP, cConfig.m_nPort))
                {
                    connect(m_pIClient.data(), SIGNAL(msgIncoming(const MSGSTRUCT&)), this, SLOT(onMsgResp()));
                    return true;
                }
            }
        }
    }

    QSLOG_ERROR("CollectorBase failed to connectMsgServer");
    return false;
}

bool CollectManager::TestSrcCollection(const TransferSet &set)
{
    // QSharedPointer<CollectorBase> pCollect = creatCollector((collection_type)set.nCollectWay);
    CollectorBase *pCollect = creatCollector((collection_type)set.nCollectWay);
    // if (pCollect.isNull())
    if (NULL == pCollect)
    {
        QSLOG_ERROR("TestCollection creatCollector error.");
        return false;
    }
    pCollect->setUrl(QString::fromStdString(set.strPath));
    pCollect->setIP(QString::fromStdString(set.strIP));
    pCollect->setListenPort(set.nPort);
    pCollect->setUserAndPwd(QString::fromStdString(set.strUsr), QString::fromStdString(set.strPwd));
    return pCollect->testCollection();
}

bool CollectManager::TestDesCollection(const SendSet &set)
{
    // QSharedPointer<CollectorBase> pCollect = creatCollector((collection_type)set.nSendWay);
    CollectorBase *pCollect = creatCollector((collection_type)set.nSendWay);
    // if (pCollect.isNull())
    if (NULL == pCollect)
    {
        QSLOG_ERROR("TestDesCollection creatCollector error.");
        return false;
    }
    pCollect->setUrl(QString::fromStdString(set.strRltvPath));
    pCollect->setIP(QString::fromStdString(set.strIP));
    pCollect->setListenPort(set.nPort);
    pCollect->setUserAndPwd(QString::fromStdString(set.strUsr), QString::fromStdString(set.strPwd));
    return pCollect->testCollection();
}

void CollectManager::startAllCollection()
{
    emit print(QStringLiteral("[%1]: 启动所有收集任务...").arg(QDateTime::currentDateTime().toString(Qt::ISODate)));
    int nCount = 0;
    foreach (CollectSet set, m_lstCollectSet.lsSets)
    {
        if (addCollect(set))
        {
            nCount++;
        }
    }
    emit print(QStringLiteral("[%1]: 共启动%2个收集任务。").arg(QDateTime::currentDateTime().toString(Qt::ISODate))
               .arg(nCount));
}

void CollectManager::stopAllCollection()
{
    emit print(QStringLiteral("[%1]: 等待当前任务结束...").arg(QDateTime::currentDateTime().toString(Qt::ISODate)));
    // foreach (QSharedPointer<CollectorBase> pCollect, m_pCollects)
    foreach(CollectorBase *pCollect, m_pCollects)
    {
        pCollect->stop();
    }
    emit print(QStringLiteral("[%1]: 所有收集任务已停止。").arg(QDateTime::currentDateTime().toString(Qt::ISODate)));
    m_pCollects.clear();
    QSLOG_INFO("All tasks have been finished.");
}

bool CollectManager::addTransCollect(const TransferSet &set, bool bFlag)
{
//     if (set.bEnable)
//     {
//         QSharedPointer<CollectorBase> pCollect = creatCollector((collection_type)set.nCollectWay);
//         if (pCollect.isNull())
//         {
//             return false;
//         }
//         pCollect->setNameID(QString::fromLocal8Bit(set.strName.c_str()));
//         pCollect->setWatchType(watch_type(set.nWatchWay));
//         pCollect->setUrl(QString::fromLocal8Bit(set.strPath.c_str()));
//         pCollect->setIP(QString::fromStdString(set.strIP));
//         pCollect->setListenPort(set.nPort);
//         pCollect->setUserAndPwd(QString::fromStdString(set.strUsr), QString::fromStdString(set.strPwd));
//         pCollect->setCollectType(1);
//         if (!set.vecRcvers.empty())
//         {
//             pCollect->setQuartzRule(QString::fromStdString(set.strDispatch));
//             FILENAME_TABLE fTable;
//             QStringList strlstTmp;
//             foreach (string reg, set.vecRegs)
//             {
//                 strlstTmp.append(QString::fromStdString(reg));
//             }
//             fTable[X_TYPE].strNameReg = strlstTmp.at(0).toStdString();
//             pCollect->setFileNameRules(fTable);
//             pCollect->m_TransferSet = set;
//             ///pCollect->start(m_pThread);
//
//             m_pCollects[set.strName] = pCollect;
//         }
//     }
//
//     if (bFlag)
//     {
//         m_lstTransSet.lsSets.push_back(set);
//         TransferSetToXml(m_strCollectXmlPath.toLocal8Bit().data(), m_lstTransSet);
//     }

    return true;
}

void CollectManager::delTransCollect(const string &strID)
{
//     list<TransferSet>::iterator it = m_lstTransSet.lsSets.begin();
//     for (; it != m_lstTransSet.lsSets.end(); it++)
//     {
//         if (it->strName == strID)
//         {
//             QSharedPointer<CollectorBase> pCollect = m_pCollects.take(strID);
//             if (!pCollect.isNull())
//             {
//                 pCollect->stop();
//                 pCollect->DeleteTcTime();
//             }
//             m_lstTransSet.lsSets.erase(it);		//若用remove方法，需要重载==
//             TransferSetToXml(m_strCollectXmlPath.toLocal8Bit().data(), m_lstTransSet);
//             break;
//         }
//     }
}

void CollectManager::mdfyTransCollect(const TransferSet &set)
{
//     list<TransferSet>::iterator it = m_lstTransSet.lsSets.begin();
//     for (; it != m_lstTransSet.lsSets.end(); it++)
//     {
//         if (it->strName == set.strName)
//         {
//             QSharedPointer<CollectorBase> pCollect = m_pCollects.take(set.strName);
//             if (!pCollect.isNull())
//             {
//                 pCollect->stop();
//             }
//             else
//             {
//                 pCollect = creatCollector((collection_type)set.nCollectWay);
//             }
//             pCollect->setNameID(QString::fromLocal8Bit(set.strName.c_str()));
//             pCollect->setWatchType(watch_type(set.nWatchWay));
//             pCollect->setUrl(QString::fromLocal8Bit(set.strPath.c_str()));
//             pCollect->setIP(QString::fromStdString(set.strIP));
//             pCollect->setListenPort(set.nPort);
//             pCollect->setUserAndPwd(QString::fromStdString(set.strUsr), QString::fromStdString(set.strPwd));
//             pCollect->setCollectType(1);
//             if (!set.vecRcvers.empty() && set.bEnable)
//             {
//                 pCollect->setQuartzRule(QString::fromStdString(set.strDispatch));
//                 FILENAME_TABLE fTable;
//                 QStringList strlstTmp;
//                 QString strTmp;
//                 foreach (string reg, set.vecRegs)
//                 {
//                     strTmp = QString::fromStdString(reg);
//                     if (!strTmp.trimmed().isEmpty())
//                     {
//                         strlstTmp.append(strTmp);
//                     }
//                 }
//                 fTable[X_TYPE].strNameReg = strlstTmp.at(0).toStdString();
//                 pCollect->setFileNameRules(fTable);
//                 pCollect->m_TransferSet = set;
//                 ///pCollect->start(m_pThread);
//             }
//             m_pCollects[set.strName] = pCollect;
//             //////////////////////////////////////////////////////////////////////////
//
//             *it = set;
//             //TransferSetToXml(m_strCollectXmlPath.toStdString(), m_lstTransSet);
//             TransferSetToXml(m_strCollectXmlPath.toLocal8Bit().data(), m_lstTransSet);
//
//             return;		// erase后，it自增，不能用return
//         }
//     }
//     // 没找到，认为是新增
//     if (it == m_lstTransSet.lsSets.end())
//     {
//         addTransCollect(set);
//     }
}

bool CollectManager::ReadTransferSet(TransferSetList &allSets, QString &strPath)
{
    m_strCollectXmlPath = strPath;
    m_lstTransSet.lsSets.clear();
    if (!XmlToTransferSet(strPath.toLocal8Bit().data(), m_lstTransSet))
    {
        QSLOG_ERROR("Fail to ReadTransferSet.");
        return false;
    }
    allSets = m_lstTransSet;

    return true;
}

void CollectManager::onMsgResp()
{
    m_msgLock.unlock();
}

bool CollectManager::readThreadNum()
{
    CPathBuilder pb;
    QString strTmp = pb.getConfigPath() + "/threadNum.ini";
    QSettings ini(strTmp, QSettings::IniFormat);
    m_vtbMaxNum = ini.value("VTB/num").toInt();
    if (m_vtbMaxNum < 1)
    {
        m_vtbMaxNum = 3;
    }
    return true;
}

bool CollectManager::TestDispatch(const QString &str)
{
    if (str == "0 0 0 0 0 0")
    {
        return false;
    }
    return true;
}

bool CollectManager::addSyncTransfer(const CollectTask &set)
{
    // QSharedPointer<CollectorBase> pCollect = creatCollector((collection_type)set.collectType);
    CollectorBase *pCollect = creatCollector((collection_type)set.collectType);
    // if (pCollect.isNull())
    if (NULL == pCollect)
    {
        QSLOG_ERROR("Fail to creatCollector.");
        return false;
    }
    //connect(pCollect.data(), SIGNAL(showLog(const QString&, const QString&)), this, SIGNAL(showLog(const QString&, const QString&)));
    //connect(pCollect.data(), SIGNAL(showLog(const CollectTask&, const QString&, int)), this, SIGNAL(showLog(const CollectTask&, const QString&, int)));
    //connect(pCollect.data(), SIGNAL(taskState(const CollectTask&, int, int)), this, SIGNAL(taskState(const CollectTask&, int, int)));
    //connect(pCollect.data(), SIGNAL(startGif(const QString&, bool)), this, SIGNAL(startGif(const QString&, bool)));

    connect(pCollect, SIGNAL(showLog(const CollectTask&, const QString&, int)), this, SIGNAL(showLog(const CollectTask&, const QString&, int)));
    connect(pCollect, SIGNAL(taskState(const CollectTask&, int, int)), this, SIGNAL(taskState(const CollectTask&, int, int)));
    connect(pCollect, SIGNAL(startGif(const QString&, bool)), this, SIGNAL(startGif(const QString&, bool)));
    pCollect->setCollectTask(set);
    if (set.enable)
    {
        pCollect->start();
    }
    m_pCollects[set.dirID] = pCollect;

    return true;
}

bool CollectManager::deltSyncTransfer(const QString &dirID)
{
    // QSharedPointer<CollectorBase> pCollect = m_pCollects.take(dirID);
    CollectorBase *pCollect = m_pCollects.take(dirID);
    // if (!pCollect.isNull())
    if (NULL != pCollect)
    {
        pCollect->stop();
        pCollect->deleteLater();
    }

    return true;
}

bool CollectManager::mdfySyncTransfer(const CollectTask &set)
{
    // 使用QObject自身的deleteLater
    // QMap<QString, QSharedPointer<CollectorBase> >::const_iterator it = m_pCollects.find(set.dirID);
    MyIterator it = m_pCollects.find(set.dirID);
    if (it != m_pCollects.end())
    {
        // QSharedPointer<CollectorBase> pCollect = m_pCollects.take(set.dirID);
        CollectorBase *pCollect = m_pCollects.take(set.dirID);
        pCollect->stop();
        addSyncTransfer(set);
        pCollect->deleteLater();
    }

    return true;
}

void CollectManager::onDoWork(const QString &taskID)
{
    // QMap<QString, QSharedPointer<CollectorBase> >::const_iterator it = m_pCollects.find(taskID);
    MyIterator it = m_pCollects.find(taskID);
    if (it != m_pCollects.end())
    {
        it.value()->doWork();
    }
}

void CollectManager::onEnable(const QString &taskID, bool bEnable)
{
    // QMap<QString, QSharedPointer<CollectorBase> >::const_iterator it = m_pCollects.find(taskID);
    MyIterator it = m_pCollects.find(taskID);
    if (it != m_pCollects.end())
    {
        if (bEnable)
        {
            it.value()->reStart();
        }
        else
        {
            it.value()->stop();
        }
    }
}

