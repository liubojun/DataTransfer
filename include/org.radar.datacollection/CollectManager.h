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

    bool readThreadNum();

    bool addCollect(const CollectSet &set);
    void delCollect(const string &strID);
    void mdfyCollect(const CollectSet &set);

    void startAllCollection();

    void stopAllCollection();

    int m_vtbMaxNum;		///< 标准化vtb的线程数

    //////////////////////////////////////////////////////////////////////////
    // 数据传输相关
public:
    bool addTransCollect(const TransferSet &set, bool bFlag=true);
    void delTransCollect(const string &strID);
    void mdfyTransCollect(const TransferSet &set);

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

private:


    bool delLastTime(string strID);

    //void ToFileTable(const map<string, vector<CDataInfo> > &mapTypeRegs, FILENAME_TABLE &fileTable);
    void ToFileTable(const std::map<string, NameMatchRule > &mapTypeRegs, FILENAME_TABLE &fileTable);

    /**
    * @brief  连接消息服务器（用于收发消息）
    * @return bool：成功返回true
    */
    bool connectMsgServer();

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
};

#endif