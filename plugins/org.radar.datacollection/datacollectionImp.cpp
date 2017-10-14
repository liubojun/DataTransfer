#include "datacollectionImp.h"
#include "ftpCollector.h"
#include "shareddirCollector.h"
#include "streamCollector.h"
#include "pathbuilder.h"
#include "IMakePath.h"
#include "IRadarPathAdapter.h"
#include "IClientMessage.h"
#include "ConnectConfig.h"
#include "CollectManager.h"
//#include <QHostInfo>


//bool DataCollectionImp::m_bContoMS = false;

DataCollectionImp::DataCollectionImp()
    : m_pActControl(NULL)
{
}

DataCollectionImp::~DataCollectionImp()
{
//     if (m_csDlg != NULL)
//     {
//         delete m_csDlg;
//         m_csDlg = NULL;
//     }

    stopComCollection();
}

// QSharedPointer<CollectorBase> DataCollectionImp::creatCollector(collection_type type)
// {
//     QSharedPointer<CollectorBase> pCollector;
//
//     ICtkPluginManager *pManager = getCtkPluginManager();
//
//     // 以下方法待实现
//     switch (type)
//     {
//     case SHARED_DIR:
//     {
//         pCollector = QSharedPointer<CollectorBase>(new SharedDirCollector(pManager));
//     }
//     break;
//     case FTP:
//     {
//         pCollector = QSharedPointer<CollectorBase>(new FtpCollector(pManager));
//     }
//     break;
//     default:
//         break;
//     }
//
//     //m_pCollector = pCollector;	//一定要保存对象的智能指针
//
//     return pCollector;
// }

QSharedPointer<ICollectManager> DataCollectionImp::createCollector(QWaitCondition &in_oCondition, QMutex &in_oLocker, int &logsize)
{
    return QSharedPointer<ICollectManager>(new CollectManager(getCtkPluginManager(), in_oCondition, in_oLocker, logsize));
}

//bool DataCollectionImp::ReadCollectSet(CollectSetList &allSets, QString &strPath)
//{
//    ////connectMsgServer();
//
//    //m_strCollectDirCfgPath = strPath;
//
//    //try
//    //{
//    //    m_lstCollectSet.lsSets.clear();
//    //    XmlToCollectSet(m_strCollectDirCfgPath.toStdString(), m_lstCollectSet);
//    //    allSets = m_lstCollectSet;
//
//    //    if (m_bFirstReadXml)
//    //    {
//    //        InitDirWatch();
//    //        m_bFirstReadXml = false;
//    //    }
//
//    //}
//    //catch (std::exception &e)
//    //{
//    //    m_bFirstReadXml = false;
//    //    QSLOG_ERROR(QString(e.what()));
//    //    return false;
//    //}
//
//    //QSLOG_DEBUG("Succeed to ReadCollectSet.");
//
//    return true;
//}


// 前台不会调用新增方法了，始终调用修改方法[5/26/2016 wujun]
// void DataCollectionImp::AddTransferSet(const TransferSet &set, bool bFlag)
// {
//     try
//     {
//         if (set.bEnable)
//         {
//             QSharedPointer<CollectorBase> pCollect/* = creatCollector((collection_type)set.nCollectWay)*/;
//             if (pCollect.isNull())
//             {
//                 return;
//             }
//             pCollect->setNameID(QString::fromLocal8Bit(set.strName.c_str()));
//             pCollect->setWatchType(watch_type(set.nWatchWay));
//             pCollect->setUrl(QString::fromLocal8Bit(set.strPath.c_str()));
//             pCollect->setIP(QString::fromStdString(set.strIP));
//             pCollect->setListenPort(set.nPort);
//             pCollect->setUserAndPwd(QString::fromStdString(set.strUsr), QString::fromStdString(set.strPwd));
//             pCollect->setCollectType(1);
//             //if (!set.strDispatch.empty() && !set.vecRcvers.empty())
//             if (!set.vecRcvers.empty())
//             {
//                 pCollect->setQuartzRule(QString::fromStdString(set.strDispatch));
//                 FILENAME_TABLE fTable;
//                 QStringList strlstTmp;
//                 foreach (string reg, set.vecRegs)
//                 {
//                     strlstTmp.append(QString::fromStdString(reg));
//                 }
//                 fTable[X_TYPE] = strlstTmp;
//                 pCollect->setFileNameRules(fTable);
//                 pCollect->m_TransferSet = set;
//                 pCollect->start(m_pThread);
//
//                 m_pTransCollects[set.strName] = pCollect;
//             }
//         }
//
//         //if (!m_bFirstReadTcXml)
//         if (bFlag)
//         {
//             m_lstTransSet.lsSets.push_back(set);
//             TransferSetToXml(m_strTransferCfgPath.toStdString(), m_lstTransSet);
//         }
//     }
//     catch (std::exception &e)
//     {
//         QSLOG_ERROR(QString(e.what()));
//         //回滚操作，临时
//     }
// }

// void DataCollectionImp::DelTransferSet(const string &pathID)
// {
//     list<TransferSet>::iterator it = m_lstTransSet.lsSets.begin();
//     for (; it != m_lstTransSet.lsSets.end(); it++)
//     {
//         if (it->strName == pathID)
//         {
//             QSharedPointer<CollectorBase> pCollect = m_pTransCollects[pathID];
//             if (!pCollect.isNull())
//             {
//                 pCollect->stop();
//                 m_pTransCollects.remove(pathID);
//                 //m_lstTransSet.lsSets.erase(it);		//若用remove方法，需要重载==
//                 //TransferSetToXml(m_strTransferCfgPath.toStdString(), m_lstTransSet);
//                 //break;
//                 pCollect->DeleteTcTime();
//             }
//             m_lstTransSet.lsSets.erase(it);		//若用remove方法，需要重载==
//             TransferSetToXml(m_strTransferCfgPath.toStdString(), m_lstTransSet);
//             break;
//         }
//     }
//
//     ////读取数据传输上次收集的时间
//     //m_lstTransTime.lsts.clear();
//     //if (!XmlToTransCollectTime(m_strTransTimeCfgPath.toStdString(), m_lstTransTime))
//     //{
//     //    QSLOG_ERROR("Read TransCollectTime.xml error.");
//     //}
//     ////删除时间
//     //list<TransCollectTime>::iterator it_time = m_lstTransTime.lsts.begin();
//     //for (; it_time != m_lstTransTime.lsts.end();)
//     //{
//     //    if (it_time->strDirID == pathID)
//     //    {
//     //        it_time = m_lstTransTime.lsts.erase(it_time);	//erase之后，指向下一个
//     //        break;
//     //    }
//     //    else
//     //    {
//     //        it_time++;
//     //    }
//     //}
//     //TransCollectTimeToXml(m_strTransTimeCfgPath.toStdString(), m_lstTransTime);
// }

// void DataCollectionImp::MdyTransferSet(const TransferSet &set)
// {
//     list<TransferSet>::iterator it = m_lstTransSet.lsSets.begin();
//     for (; it != m_lstTransSet.lsSets.end(); it++)
//     {
//         if (it->strName == set.strName)
//         {
//             QSharedPointer<CollectorBase> pCollect = m_pTransCollects.take(set.strName);
//             if (!pCollect.isNull())
//             {
//                 pCollect->stop();
//             }
//             else
//             {
//                 //pCollect = creatCollector((collection_type)set.nCollectWay);
//             }
//             //////////////////////////////////////////////////////////////////////////
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
//                 foreach (string reg, set.vecRegs)
//                 {
//                     strlstTmp.append(QString::fromStdString(reg));
//                 }
//                 fTable[X_TYPE] = strlstTmp;
//                 pCollect->setFileNameRules(fTable);
//                 pCollect->m_TransferSet = set;
//                 pCollect->start(m_pThread);
//             }
//             m_pTransCollects[set.strName] = pCollect;
//             //////////////////////////////////////////////////////////////////////////
//
//             *it = set;
//             TransferSetToXml(m_strTransferCfgPath.toStdString(), m_lstTransSet);
//
//             return;		// erase后，it自增，不能用return
//         }
//     }
//     // 没找到，认为是新增
//     if (it == m_lstTransSet.lsSets.end())
//     {
//         AddTransferSet(set);
//     }
// }

// void DataCollectionImp::InitTcDir()
// {
//     foreach (TransferSet set, m_lstTransSet.lsSets)
//     {
//         AddTransferSet(set);
//     }
// }

// void DataCollectionImp::ReadLastTime()
// {
//     CPathBuilder pabd;
//     QString strConfigPath = pabd.getConfigPath();
//     QString strXmlPath;
//
//     //读取各站各类型文件最新处理时间
//     m_lstStationTime.lsInfos.clear();
//     strXmlPath = strConfigPath + "/LatestFileTime.xml";
//     if (!XmlToStationTime(strXmlPath.toStdString(), m_lstStationTime))
//     {
//         QSLOG_ERROR("Read LatestFileTime.xml error.");
//     }
//
//     //读取数据传输上次收集的时间
//     m_lstTransTime.lsts.clear();
//     if (!XmlToTransCollectTime(m_strTransTimeCfgPath.toStdString(), m_lstTransTime))
//     {
//         QSLOG_ERROR("Read TransCollectTime.xml error.");
//     }
// }

QMap<QString, QString> DataCollectionImp::GetLastVtbPath(const QStringList &stations, const QString &strStart, const QString &strEnd)
{
    ICtkPluginManager *pManager = getCtkPluginManager();
    QObject *obj = pManager->getService("IRadarPathAdapter");
    QSharedPointer<IMakePath> pMakePath = (qobject_cast<IRadarPathAdapter *>(obj))->createRadarProductPath();

    QMap<QString, QString> result;
    for (int i=0; i<stations.size(); ++i)
    {
        QString strPath = pMakePath->getLastVtbPath(stations.at(i), strStart, strEnd);
        if (!strPath.isEmpty())
        {
            result[stations.at(i)] = strPath;
        }
    }

    return result;
}

void DataCollectionImp::SendHeartMsg()
{
    ConnectInfo CI;
    CI.connId   = m_strIPaddr.toStdString();
    CI.connTime = time(NULL);

    std::string s;
    bool bFlag = SerializeConnectionInfo2String(s, CI);
    if (bFlag)
    {
        MSGSTRUCT msg;
        msg.head.m_msgid = 12068;
        msg.body.m_msgdata = s;

        // m_pIClient->heartbeat(msg);		//临时屏蔽
    }
}

// void DataCollectionImp::getIpaddr(QString &strIP)
// {
//     QString localHostName = QHostInfo::localHostName();
//     QHostInfo info = QHostInfo::fromName(localHostName);
//     foreach (QHostAddress addr, info.addresses())
//     {
//         if (addr.protocol() == QAbstractSocket::IPv4Protocol)
//         {
//             strIP = addr.toString();
//             break;
//         }
//     }
// }

void DataCollectionImp::initgui(UIInterface *in_pGuiInterace)
{
    if (NULL == in_pGuiInterace)
    {
        QSLOG_ERROR("in_pGuiInterace is null.");
        return;
    }
    m_pGui = in_pGuiInterace;

    // 添加日志视图菜单项
    m_pActView = QSharedPointer<QAction>(new QAction(QIcon(":/log.png"), QObject::tr("collect log"), NULL));
    m_pActView->setCheckable(true);
    m_pGui->addActToView(m_pActView.data());
    QObject::connect(m_pActView.data(), SIGNAL(triggered(bool)), this, SLOT(showLogView(bool)));

    // 添加收集控制菜单项
    m_pActControl = QSharedPointer<QAction>(new QAction(QIcon(":/collect.png"), QObject::tr("collection"), NULL));
    m_pActControl->setCheckable(true);
    QObject::connect(m_pActControl.data(), SIGNAL(triggered(bool)), this, SLOT(collectControl(bool)));
    m_pGui->addActToControl(m_pActControl.data(), true);

//     // 产品设置菜单
//     if (m_csDlg == NULL)
//     {
//         m_csDlg = new CollectSetDlg();
//     }
//     QTreeWidgetItem *pParent = m_pGui->AppendItem(QObject::tr("single collect"));
//     m_pGui->AppendItem(pParent, QObject::tr("collect set"), m_csDlg);
}

void DataCollectionImp::collectControl(bool bFlag)
{
    if (m_pActControl != NULL)
    {
        if (bFlag)
        {
            if (!m_pActView->isChecked())
            {
                m_pActView->trigger();		//显示日志窗体
            }
            m_pActControl->setChecked(true);
            startComCollection();
        }
        else
        {
            m_pActControl->setChecked(false);
            stopComCollection();
        }
    }
}

void DataCollectionImp::startComCollection()
{
	/*  ICtkPluginManager *pManager = getCtkPluginManager();
	if (pManager != NULL)
	{
	if (m_pComCollector.isNull())
	{
	m_pComCollector = QSharedPointer<CollectManager>(new CollectManager(pManager));
	BIND_LOG(m_pComCollector.data(), m_pLogView.data(), addLine(const QString&));
	}
	CPathBuilder pb;
	QString setXmlPath = pb.getConfigPath() + "/CollectSet.xml";
	QString timeXmlPath = pb.getConfigPath() + "/LatestFileTime.xml";
	m_pComCollector->setXmlPath(setXmlPath, timeXmlPath);
	m_pComCollector->startAllCollection();
	}*/
}

void DataCollectionImp::stopComCollection()
{
    if (!m_pComCollector.isNull())
    {
        m_pComCollector->stopAllCollection();
    }
}

void DataCollectionImp::showLogView(bool bFlag)
{
    if (bFlag)
    {
        if (m_pLogView.isNull())
        {
            createLogView();
            m_pGui->addSubWnd(m_pLogView.data());
        }
        else
        {
            m_pGui->showSubWnd(tr("data collection log view"));
        }
    }
    else
    {
        m_pGui->removeSubWnd(tr("data collection log view"));
    }
}

void DataCollectionImp::createLogView()
{
    m_pLogView = QSharedPointer<ILogWidget>(new ILogWidget());
    m_pLogView->setWindowTitle(tr("data collection log view"));
    QObject::connect(m_pLogView.data(), SIGNAL(closed(bool)), m_pActView.data(), SLOT(setChecked(bool)));
    QObject::connect(m_pLogView.data(), SIGNAL(closed(bool)), this, SLOT(resetLogView()));

    if (!m_pComCollector.isNull())
    {
        BIND_LOG(m_pComCollector.data(), m_pLogView.data(), addLine(const QString&));
    }
}

void DataCollectionImp::resetLogView()
{
    QObject::disconnect(m_pLogView.data(), SIGNAL(closed(bool)), m_pActView.data(), SLOT(setChecked(bool)));
    QObject::disconnect(m_pLogView.data(), SIGNAL(closed(bool)), this, SLOT(resetLogView(bool)));
    m_pLogView.clear();
}
