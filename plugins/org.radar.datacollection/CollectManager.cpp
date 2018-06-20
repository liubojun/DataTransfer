#include "CollectManager.h"
#include "shareddirCollector.h"
#include "ftpCollector.h"
#include "pathbuilder.h"

#include "ctkPublicFun.h"
#include "CltDispatch.h"
#include "IDispatchTimer.h"
#include <QSettings>


CollectManager::CollectManager(ICtkPluginManager *pCtkManager, QWaitCondition &in_oCond, QMutex &in_oLocker, int &logsize)
    : m_pCtkManager(pCtkManager)
    , m_vtbMaxNum(1),
      m_oCond(in_oCond),
      m_oLocker(in_oLocker),
      m_iLogsize(logsize)

{
    m_bFirstReadTcXml = true;
}

CollectManager::~CollectManager()
{
    stopAllCollection();
    //m_pThread->quit();
    //m_pThread->wait();
    QSLOG_INFO("~CollectManager()");
}


CollectorBase *CollectManager::creatCollector(collection_type type)
{
    // QSharedPointer<CollectorBase> pCollect;
    CollectorBase *pCollect = NULL;
    switch (type)
    {
    case SHARED_DIR:
        // pCollect = QSharedPointer<CollectorBase>(new SharedDirCollector(this, m_oCond, m_oLocker, m_iLogsize));
        pCollect = new SharedDirCollector(m_oCond, m_oLocker, m_iLogsize);
        break;
    case FTP:
        //pCollect = QSharedPointer<CollectorBase>(new FtpCollector(this, m_oCond, m_oLocker, m_iLogsize));
        pCollect = new FtpCollector(m_oCond, m_oLocker, m_iLogsize);
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

void CollectManager::stopAllCollection()
{
    emit print(QStringLiteral("[%1]: 等待当前任务结束...").arg(QDateTime::currentDateTime().toString(Qt::ISODate)));
    // foreach (QSharedPointer<CollectorBase> pCollect, m_pCollects)
    foreach(CollectorBase *pCollect, m_pCollects)
    {
        pCollect->stop();
        //delete pCollect;
    }
    emit print(QStringLiteral("[%1]: 所有收集任务已停止。").arg(QDateTime::currentDateTime().toString(Qt::ISODate)));
    m_pCollects.clear();
    QSLOG_INFO("All tasks have been finished.");
}


void CollectManager::onMsgResp()
{
    m_msgLock.unlock();
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
    CollectorBase *pCollect = creatCollector((collection_type)set.collectType);
    if (NULL == pCollect)
    {
        QSLOG_ERROR("Fail to creatCollector.");
        return false;
    }

    connect(pCollect, SIGNAL(showLog(const CollectTask&, const QString&, int)), this, SIGNAL(showLog(const CollectTask&, const QString&, int)));
    connect(pCollect, SIGNAL(taskState(const CollectTask&, int, int)), this, SIGNAL(taskState(const CollectTask&, int, int)));
    connect(pCollect, SIGNAL(startGif(const QString&, bool)), this, SIGNAL(startGif(const QString&, bool)));
    pCollect->setCollectTask(set);
    if (set.enable)
    {
        addCollector2TimeTable(pCollect);
        //pCollect->start();
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
        pCollect->deleteSelf();
        //pCollect->deleteLater();
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
        pCollect->deleteSelf();
        addSyncTransfer(set);
        //pCollect->deleteLater();

        // 停止与该收集任务相关的各项内容
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
            //it.value()->reStart();
            addCollector2TimeTable(it.value());
        }
        else
        {
            it.value()->stop();
            stopCollector(it.value());
        }
    }
}

void CollectManager::addCollector2TimeTable(CollectorBase *in_pBaseCollector)
{
    QMutexLocker m_oGuard(&m_oDispatchLocker);
    QObject *op = m_pCtkManager->getService("IDispatchTimer");
    IDispatchTimer *iDt = qobject_cast<IDispatchTimer *>(op);
    if (iDt)
    {
        QSharedPointer<CltDispatch> cDispatch = QSharedPointer<CltDispatch>(new CltDispatch(in_pBaseCollector));
        QSharedPointer<QObject> m_pTimerObj;
        bool bFlag = iDt->SetDispatchTimer(in_pBaseCollector->getCollectTask().dispatch.toStdString(),
                                           cDispatch.data(),
                                           QSharedPointer<TimerCallBackParam>(new TimerCallBackParam()), m_pTimerObj);
        if (bFlag)
        {
            m_oDispatchers.insert(in_pBaseCollector->getIdentify(), cDispatch);
            m_oTimerObjs.insert(in_pBaseCollector->getIdentify(), m_pTimerObj);
            connect(in_pBaseCollector, SIGNAL(showIdentify(QString)), this, SLOT(removeCollectorFromTimeTable(QString)));
        }
        else
        {
            QSLOG_ERROR(QString("FtpCollector SetDispatchTimer failed: %1"));
        }

    }
}

void CollectManager::removeCollectorFromTimeTable(QString in_identify)
{
    QMutexLocker m_oGuard(&m_oDispatchLocker);
    m_oDispatchers.remove(in_identify);
    m_oTimerObjs.remove(in_identify);
    //m_oWorkers.remove(in_identify);
}

void CollectManager::stopCollector(CollectorBase *in_pBaseCollector)
{
    QMutexLocker m_oGuard(&m_oDispatchLocker);
    QObject *op = m_pCtkManager->getService("IDispatchTimer");
    if (op != NULL)
    {
        IDispatchTimer *iDt = qobject_cast<IDispatchTimer *>(op);
        iDt->stopDispatchTimer(m_oTimerObjs[in_pBaseCollector->getIdentify()]);
    }
}

