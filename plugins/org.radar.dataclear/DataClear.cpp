#include "DataClear.h"
#include "CDispatch.h"
#include "IDispatchTimer.h"
#include "QsLog/ctkLog.h"
#include <QThread>
#include <QFileInfo>

DataClear::DataClear()
{
    m_fullpath = "F:\\RadarRoot\\product\\NR003\\VPPI\\201408\\20140814";
    //从配置文件中读取清除方案，按时间，按空间。
    m_style = "1" ;
    m_freeSpace = 99999000;
    m_timeLine = "-2" ;

    m_pThread = QSharedPointer<QThread>(new QThread());
    m_pThread->start();
}

DataClear::~DataClear()
{
    m_pThread->quit();
    m_pThread->wait();
}

bool DataClear::start(const QList<BaseDatas> &m_datas, QSharedPointer<QThread> pThread)
{
    ICtkPluginManager *pManager = getCtkPluginManager();
    QObject *op = pManager->getService("IDispatchTimer");
    IDispatchTimer *iDt = qobject_cast<IDispatchTimer *>(op);

    bool flag = true;

    if (iDt)
    {
        foreach(const BaseDatas &item, m_datas)
        {
            QSharedPointer<CDispatch> cDispatch = QSharedPointer<CDispatch>(new CDispatch(item.m_fullPath, item.m_style, item.m_freeSize, item.m_time, item.m_regex));
            if (!pThread.isNull())
            {
                cDispatch->moveToThread(pThread.data());
            }
            flag = iDt->SetDispatchTimer(item.m_DDrule.toStdString(), cDispatch.data(), QSharedPointer<TimerCallBackParam>(new BaseDatas(item)), m_pTimerObject);
            m_oProcessors.append(cDispatch);
            if( !flag)
            {
                return flag;
            }
        }
        return flag;
    }
    else
    {
        return false;
    }
}

bool DataClear::start(const BaseDatas &data)
{
    ICtkPluginManager *pManager = getCtkPluginManager();
    QObject *op = pManager->getService("IDispatchTimer");
    IDispatchTimer *iDt = qobject_cast<IDispatchTimer *>(op);

    bool flag = true;

    if (iDt)
    {
        QSharedPointer<CDispatch> cDispatch = QSharedPointer<CDispatch>(new CDispatch(data.m_fullPath, data.m_style, data.m_freeSize, data.m_time, data.m_regex));
        cDispatch->setParam(data);
        connect(cDispatch.data(), SIGNAL(clearBegin(const QString &)), this, SIGNAL( taskBegin(const QString &)));
        connect(cDispatch.data(), SIGNAL(clearEnd(const QString &)), this, SIGNAL( taskEnd(const QString &)));
        QSharedPointer<QThread> pThread = QSharedPointer<QThread>(new QThread());
        pThread->start();
        m_pWorkThread.insert(data.m_fullPath, pThread);
        cDispatch->moveToThread(pThread.data());

        flag = iDt->SetDispatchTimer(data.m_DDrule.toStdString(), cDispatch.data(), QSharedPointer<TimerCallBackParam>(new BaseDatas(data)), m_pTimerObject);

        m_mapDispatchers[data.m_fullPath] = cDispatch;
        m_mapTimers[data.m_fullPath] = m_pTimerObject;

        return flag;
    }
    else
    {
        return false;
    }
}

void DataClear::stop(const BaseDatas &data)
{
    ICtkPluginManager *pManager = getCtkPluginManager();
    QObject *op = pManager->getService("IDispatchTimer");
    IDispatchTimer *iDt = qobject_cast<IDispatchTimer *>(op);
    if (iDt)
    {
        QSharedPointer<QObject> pTimer = m_mapTimers.take(data.m_fullPath);
        if (pTimer != NULL)
        {
            iDt->stopDispatchTimer(pTimer);
            m_mapDispatchers.remove(data.m_fullPath);
            QMap<QString, QSharedPointer<QThread> >::iterator iter = m_pWorkThread.find(data.m_fullPath);
            if (iter !=  m_pWorkThread.end())
            {
                iter.value()->quit();
                iter.value()->wait();;
                m_pWorkThread.erase(iter);
            }
        }
    }
}

void DataClear::stop(const QList<BaseDatas> &m_datas)
{
}

const QList<BaseDatas> & DataClear::get()
{
    return m_oBaseDatas;
}

//bool DataClear::ReadClearSet(DataClearSetList &allsets, QString &strPath)
//{
//    m_strClearCfgPath = strPath;
//    QFileInfo qfInfo(m_strClearCfgPath);
//    if (!qfInfo.isFile() || !qfInfo.exists())
//    {
//        QSLOG_ERROR(QString("[%1] is not a existed file.").arg(m_strClearCfgPath));
//        return false;
//    }
//
//    try
//    {
//        m_lstClearSet.lst.clear();
//        XmlToDataClearSet(strPath.toStdString(), m_lstClearSet);
//        allsets = m_lstClearSet;
//    }
//    catch (std::exception &e)
//    {
//        QSLOG_ERROR(QString(e.what()));
//        return false;
//    }
//
//    return true;
//}

//void DataClear::AddClearSet(const DataClearSet &set)
//{
//    QString strPath = QString::fromStdString(set.strPath);
//    QFileInfo qfInfo(strPath);
//    if (!qfInfo.isDir())
//    {
//        QSLOG_ERROR(QString("AddCollectSet::[%1] is not a dir.").arg(strPath));
//        return;
//    }
//
//    if (m_mapDispatchers.find(strPath) != m_mapDispatchers.end())
//    {
//        QSLOG_ERROR(QString("The dir [%1] has be added.").arg(strPath));
//        return;
//    }
//
//    BaseDatas base;
//    base.m_fullPath = strPath;
//    base.m_DDrule = QString::fromStdString(set.strDrule);
//    foreach (string reg, set.reges)
//    {
//        base.m_regex.append(QString::fromStdString(reg));
//    }
//    base.m_style = QString::number(set.nStyle);
//    base.m_freeSize = QString::number(set.nSize);
//    base.m_time = set.nTime;
//    //启动数据清除线程
//    start(base);
//    //记录到xml文件
//    try
//    {
//        m_lstClearSet.lst.push_back(set);
//        DataClearSetToXml(m_strClearCfgPath.toStdString(), m_lstClearSet);
//    }
//    catch (std::exception &e)
//    {
//        QSLOG_ERROR(QString(e.what()));
//    }
//}

//void DataClear::DelClearSet(const string &strPath)
//{
//    QString qsPath = QString::fromStdString(strPath);
//    foreach (DataClearSet set, m_lstClearSet.lst)
//    {
//        if (set.strPath == strPath)
//        {
//            BaseDatas base;
//            base.m_fullPath = qsPath;
//            stop(base);
//
//            m_lstClearSet.lst.remove(set);
//            DataClearSetToXml(m_strClearCfgPath.toStdString(), m_lstClearSet);
//
//            break;
//        }
//    }
//}

//void DataClear::MdyClearSet(const DataClearSet &set)
//{
//    BaseDatas base;
//    base.m_fullPath = QString::fromStdString(set.strPath);
//    stop(base);
//    m_lstClearSet.lst.remove(set);
//
//    AddClearSet(set);
//}

void DataClear::doNow(const QString &dir)
{
    ICtkPluginManager *pManager = getCtkPluginManager();
    QObject *op = pManager->getService("IDispatchTimer");
    IDispatchTimer *iDt = qobject_cast<IDispatchTimer *>(op);

    bool flag = true;

    QSharedPointer<CDispatch> m_pDispatch = m_mapDispatchers[dir];
    if (!m_pDispatch.isNull())
    {
        m_pDispatch->doNow();
    }
}


