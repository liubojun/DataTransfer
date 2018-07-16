/**
  * @file   IDataCollection.h
  * @brief  数据收集服务接口.
  *
  *  数据收集服务，提供基于共享目录方式的收集；基于FTP方式的收集以及基于消息流方式的收集
  *		1.基于共享（本地）目录的方式收集
  *			提供两种方式，一种是实时监控远程目录，当检测到有新文件后，直接产生一条消息到队列中
  *						 另外一种是定时轮询目录，当检测到有新文件后，直接产生一条消息到队列中
  *		2.基于FTP的方式收集
  *			需要通过定时轮询远程的FTP目录，当检测有到新文件后，需要将远程的新文件取到本地的工作目录，再产生一条消息到队列中
  *		3.基于消息流方式的收集
  *			待定
  * @author 刘伯俊
  * @par   Copyright (c):
  *          刘伯俊.
  *         All Rights Reserved
  * @date   2016:04:07
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef IDataCollection_h__
#define IDataCollection_h__

/// 数据收集方式的枚举定义
typedef enum
{
    SHARED_DIR = 0,		//< 共享目录或者本地目录收集
    FTP = 1,			//< FTP收集
	SFTP = 2			//< SFTP收集
} collection_type;

/// 目录监控方式的枚举定义
typedef enum
{
    TIMER_SCAN,				///< 定时轮询
    FILESYSTEM_WATCHER,		///< 实时监控
    //ONLY_ONCE				///< 归档
} watch_type;

#include "collectSet.h"
#include "transferSet.h"
#include "IBaseInterface.h"
#include "macro.h"
#include <QSharedPointer>
#include <QWaitCondition>
#include <QMutex>

//class CollectorBase;
class CollectManager;
class ICollectManager;

/**
  * @class <IDataCollection>
  * @brief  数据收集服务接口.
  *
  *  这里是类的详细描述
  * @note	服务在系统中均为单例
  * @see 	IBaseInterface
 */
class IDataCollection : public IBaseInterface
{
    Q_OBJECT
    Q_INTERFACES(IBaseInterface)

public:
    /**
     * @brief  默认析构函数
     */
    virtual ~IDataCollection() {}

    /**
     * @brief  根据收集类型创建相应的收集器
     * @param  collection_type type 收集类型
     * @return QSharedPointer<CollectorBase>：收集器的智能指针
     */
    //virtual QSharedPointer<CollectorBase> creatCollector(collection_type type) = 0;

    virtual QSharedPointer<ICollectManager> createCollector(QWaitCondition &in_oCondition, QMutex &in_oLocker, int &logsize) = 0;

    //////////////////////////////////////////////////////////////////////////

    /**
     * @brief  读取数据收集配置文件
     * @param  CollectSetList & allSets：返回所有的设置
     * @param  QString & strPath：配置文件的路径
     * @return bool：读取成功返回true，否则返回false
     */
    //virtual bool ReadCollectSet(CollectSetList &allSets, QString &strPath) = 0;
    /**
     * @brief  添加数据收集设置
     * @param  const CollectSet & set：数据收集设置
     * @return void：无
     */
    //virtual void AddCollectSet(const CollectSet &set) = 0;
    /**
     * @brief  删除指定的数据收集设置
     * @param  const string & strDir：收集的目录
     * @return void：无
     */
    //virtual void DelCollectSet(const string &strDir) = 0;
    /**
     * @brief  修改指定的数据收集设置
     * @param  const CollectSet & set：修改后的数据收集设置
     * @return void：无
     */
    //virtual void MdyCollectSet(const CollectSet &set) = 0;

    /**
     * @brief  读取数据收集配置文件
     * @param  TransferSetList & allSets：返回所有的设置
     * @param  QString & strPath：配置文件的路径
     * @return bool：读取成功返回ture，否则返回false
     */
    //virtual bool ReadTransferSet(TransferSetList &allSets, QString &strPath) = 0;
    /**
     * @brief  添加数据传输设置
     * @param  const TransferSet & set：数据传输设置
     * @return void：无
     */
    //virtual void AddTransferSet(const TransferSet &set, bool bFlag=true) = 0;
    /**
     * @brief  删除指定的数据传输设置
     * @param  const string & pathID：要删除的目录ID
     * @return void：无
     */
    //virtual void DelTransferSet(const string &pathID) = 0;
    /**
     * @brief  修改指定的传输设置
     * @param  const TransferSet & set：修改后的设置
     * @return void：无
     */
    //virtual void MdyTransferSet(const TransferSet &set) = 0;


    /**
     * @brief  收集端连接测试
     * @param  const TransferSet & set：收集端信息
     * @return bool：连接成功返回true，否则返回false
     */
    //virtual bool TestCollection(const TransferSet &set) = 0;

    /**
     * @brief  分发端连接测试
     * @param  const SendSet & set：分发端信息
     * @return bool：连接成功返回true，否则返回false
     */
    //virtual bool TestDesCollection(const SendSet &set) = 0;

    /**
    * @brief  获取给定子站在指定时间段内的最新vtb数据全路径
    * @param  const QStringList & stations：站号列表
    * @param  const QString & strStart：起始时间
    * @param  const QString & strEnd：终止时间
    * @return QMap<QString, QString>：站号-最新vtb文件全路径
    */
    virtual QMap<QString, QString> GetLastVtbPath(const QStringList &stations, const QString &strStart, const QString &strEnd) = 0;

signals:
    /**
     * @brief  收集到新文件发送该信号
     * @param  const TransferSet & tset：传输设置
     * @return void：无
     */
    void NewTransferFiels(const TransferSet &tset);
};

Q_DECLARE_INTERFACE(IDataCollection, "org.radar.datacollection.IDataCollection")

#endif // IDataCollection_h__