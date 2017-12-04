#ifndef ICollectManager_H_
#define ICollectManager_H_

#include "lastCollectTime.h"
#include "collectSet.h"
#include "transferSet.h"
#include "commontypedef.h"
#include "IBaseInterface.h"
#include "ILogPrint.h"
#include "IDataCollection.h"
#include <QSharedPointer>
#include <QWaitCondition>
#include <QMutex>

class CollectorBase;

class ICollectManager : public ILogPrint
{
    Q_OBJECT
public:

    // ICollectManager(QWaitCondition &in_oCondition, QMutex &in_oLocker);

    virtual ~ICollectManager() {}

    virtual CollectorBase *creatCollector(collection_type type) = 0;

    virtual bool addCollect(const CollectSet &set) = 0;
    virtual void delCollect(const string &strID) = 0;
    virtual void mdfyCollect(const CollectSet &set) = 0;

    virtual void startAllCollection() = 0;

    virtual void stopAllCollection() = 0;

    //////////////////////////////////////////////////////////////////////////
    // 数据传输相关

signals:
    void newTransFile(const TransferSet &tSet);
    //void showLog(const QString &dirName, const QString &info);
    void showLog(const CollectTask &task, const QString &info, int type);
    //void doWork(const QString &taskID);
    void taskState(const CollectTask&, int, int);
    void startGif(const QString&, bool);

public slots:
    virtual void onDoWork(const QString &taskID) = 0;
    virtual void onEnable(const QString&, bool) = 0;

public:
    virtual bool addTransCollect(const TransferSet &set, bool bFlag=true) = 0;
    virtual void delTransCollect(const string &strID) = 0;
    virtual void mdfyTransCollect(const TransferSet &set) = 0;

    virtual bool addSyncTransfer(const CollectTask &set) = 0;


    virtual bool mdfySyncTransfer(const CollectTask &set) = 0;
    virtual bool deltSyncTransfer(const QString &dirID) = 0;

    /**
    * @brief  测试收集连接
    * @param  const TransferSet & set：传输设置信息
    * @return bool：成功返回true，否则返回false
    */
    virtual bool TestSrcCollection(const TransferSet &set) = 0;

    /**
     * @brief  测试分发连接
     * @param  const SendSet & set：分发设置信息
     * @return bool：成功返回true，否则返回false
     */
    virtual bool TestDesCollection(const SendSet &set) = 0;

    virtual bool TestDispatch(const QString &str) = 0;
};

#endif