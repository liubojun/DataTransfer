#ifndef CollectManager_H_
#define CollectManager_H_

#include "lastCollectTime.h"
#include "collectSet.h"
#include "transferSet.h"
#include "IBaseInterface.h"
#include "ILogPrint.h"
#include "IDataCollection.h"
#include "ICollectManager.h"
#include <QSharedPointer>
#include <QMutex>
#include <QWaitCondition>


class CollectorBase;
class IClient;
class CltDispatch;

typedef QMap<QString, CollectorBase* >::const_iterator MyIterator;

class CollectManager : public ICollectManager
{
    Q_OBJECT
public:
    CollectManager(ICtkPluginManager *pCtkManager, QWaitCondition &in_oCond, QMutex &in_oLocker, int &logsize);
    virtual ~CollectManager();

    /**
    * @brief  根据收集类型创建相应的收集器
    * @param  collection_type type 收集类型
    * @return QSharedPointer<CollectorBase>：收集器的智能指针
    */
    // QSharedPointer<CollectorBase> creatCollector(collection_type type);
    virtual CollectorBase *creatCollector(collection_type type);

    void stopAllCollection();

    int m_vtbMaxNum;		///< 标准化vtb的线程数

    //////////////////////////////////////////////////////////////////////////
    // 数据传输相关
public:

    virtual bool addSyncTransfer(const CollectTask &set);
    virtual bool mdfySyncTransfer(const CollectTask &set);
    virtual bool deltSyncTransfer(const QString &dirID);

    /**
    * @brief  收集测试连接
    * @param  const TransferSet & set：传输设置信息
    * @return bool：成功返回true，否则返回false
    */
    bool TestSrcCollection(const TransferSet &set);

    /**
     * @brief  测试分发连接
     * @param  const SendSet & set：分发设置信息
     * @return bool：成功返回true，否则返回false
     */
    bool TestDesCollection(const SendSet &set);

    bool TestDispatch(const QString &str);

    TransferSetList m_lstTransSet;			///< 维护数据传输设置

    bool m_bFirstReadTcXml;
    //////////////////////////////////////////////////////////////////////////

    CollectTimeList m_lastTimeList;
    ICtkPluginManager *m_pCtkManager;
    QSharedPointer<IClient> m_pIClient;
    QMutex m_msgLock;		///< 消息互斥锁
    QMutex m_timeLock;		///< 扫描时间互斥锁

private slots:
    void onMsgResp();
    virtual void onDoWork(const QString &taskID);
    virtual void onEnable(const QString&, bool);

    /**
     * @brief  将当前收集任务相关的信息从时间表中移除
     * @param  QString in_identify：收集任务唯一标识
     * @return void：无
    */
    void removeCollectorFromTimeTable(QString in_identify);

private:


    bool delLastTime(string strID);

    //void ToFileTable(const map<string, vector<CDataInfo> > &mapTypeRegs, FILENAME_TABLE &fileTable);
    void ToFileTable(const std::map<string, NameMatchRule > &mapTypeRegs, FILENAME_TABLE &fileTable);

    /**
    * @brief  连接消息服务器（用于收发消息）
    * @return bool：成功返回true
    */
    bool connectMsgServer();

    /**
     * @brief  将当前收集任务加入到时间表中
     * @param  CollectorBase *in_pBaseCollector：收集任务
     * @return void：无
     */
    void addCollector2TimeTable(CollectorBase *in_pBaseCollector);

    /**
    * @brief  停止收集任务
    * @param  CollectorBase *in_pBaseCollector：收集任务
    * @return void：无
    */
    void stopCollector(CollectorBase *in_pBaseCollector);


private:
    QString m_strTimeXmlPath;
    QString m_strCollectXmlPath;
    // QMap<QString, QSharedPointer<CollectorBase> > m_pCollects;
    QMap<QString, CollectorBase*> m_pCollects;
    QSharedPointer<QThread> m_pThread;
    CollectSetList m_lstCollectSet;			///< 维护数据收集设置

    QWaitCondition &m_oCond;
    QMutex &m_oLocker;
    int &m_iLogsize;

    // 保存当前示例创建的所有的调度器
    QMap<QString, QSharedPointer<CltDispatch> > m_oDispatchers;

    // 配套调度器的时间
    QMap<QString, QSharedPointer<QObject> > m_oTimerObjs;

    // 配套调度器的线程
    // QMap<QString, QSharedPointer<QThread> > m_oWorkers;

    // 配套调度器控制锁
    QMutex m_oDispatchLocker;
};

#endif