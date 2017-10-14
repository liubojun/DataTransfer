/**
  * @file   datacollectionImp.h
  * @brief  数据收集实现类.
  *
  *  这里是详细说明。
  * @author nriet
  * @par   Copyright (c):
  *          nriet.
  *         All Rights Reserved
  * @date   2016:04:07
  * @note   这里是注意事项。
  * @version 1.0
  */
#ifndef datacollectionImp_h__
#define datacollectionImp_h__

#include "IDataCollection.h"
#include "CollectorBase.h"	//待删
#include "ICollectManager.h"
#include "transferSet.h"
//#include "CollectHistory.h"
#include "ILogWidget.hpp"
//#include "collectSetDlg.h"
#include <QTimer>

class CltDispatch;

/**
  * @class <DataCollectionImp>
  * @brief  创建数据收集的实例对象.
  *
  *			这里是类的详细描述
  * @note	这里是注意事项
  * @see 	IDataCollection
 */
class DataCollectionImp : public IDataCollection
{
    Q_OBJECT
    Q_INTERFACES(IBaseInterface)
    Q_INTERFACES(IDataCollection)
public:
    DataCollectionImp();
    virtual ~DataCollectionImp();

	virtual QSharedPointer<ICollectManager> createCollector(QWaitCondition &in_oCondition, QMutex &in_oLocker, int &logsize);

    /**
     * @brief  读取数据传输配置文件
     * @param  TransferSetList & allSets：返回所有的设置
     * @param  QString & strPath：配置文件的路径
     * @return bool：读取成功返回ture，否则返回false
     */
    //virtual bool ReadTransferSet(TransferSetList &allSets, QString &strPath);
    /**
     * @brief  添加数据传输设置
     * @param  const TransferSet & set：数据传输设置
     * @return void：无
     */
    //virtual void AddTransferSet(const TransferSet &set, bool bFlag=true);
    /**
     * @brief  删除指定的数据传输设置
     * @param  const string & pathID：要删除的目录ID
     * @return void：无
     */
    //virtual void DelTransferSet(const string &pathID);
    /**
     * @brief  修改指定的传输设置
     * @param  const TransferSet & set：修改后的设置
     * @return void：无
     */
    //virtual void MdyTransferSet(const TransferSet &set);

    /**
     * @brief  测试收集连接
     * @param  const TransferSet & set
     * @return bool：描述返回值
     */
    //virtual bool TestCollection(const TransferSet &set);

    //virtual bool TestDesCollection(const SendSet &set);

    /**
     * @brief  获取给定子站在指定时间段内的最新vtb数据全路径
     * @param  const QStringList & stations：站号列表
     * @param  const QString & strStart：起始时间
     * @param  const QString & strEnd：终止时间
     * @return QMap<QString, QString>：站号-最新vtb文件全路径
     */
    virtual QMap<QString, QString> GetLastVtbPath(const QStringList &stations, const QString &strStart, const QString &strEnd);

signals:
    void print(const QString &logInfo);

private:
    QSharedPointer<QObject> m_pTimerObject;
    QList<QSharedPointer<CltDispatch> > m_oProcessors;
    //static bool m_bContoMS;		///< 是否连接客服端

    /**
     * @brief  获取本机IP地址
     * @param  QString & strIP
     * @return void：描述返回值
     */
    //void getIpaddr(QString &strIP);
    QString m_strIPaddr;

    /**
    * @brief  插件中界面初始化方法
    * @param  UIInterface * in_pGuiInterace 主窗体抽象出的接口
    * @return void：无
    */
    virtual void initgui(UIInterface *in_pGuiInterace);

    void startComCollection();

    void stopComCollection();

    void createLogView();

private slots:
    void SendHeartMsg();
    void collectControl(bool bFlag);
    void showLogView(bool bFlag);
    void resetLogView();

private:
    UIInterface *m_pGui;							///< gui界面线程对象
    QSharedPointer<QAction> m_pActControl;			///< 控制菜单项
    QSharedPointer<QAction> m_pActView;				///< 日志视图菜单项
    QSharedPointer<ILogWidget>	m_pLogView;			///< 日志窗

    //CollectSetDlg *m_csDlg;							///< 单站收集设置界面

    QSharedPointer<CollectManager>	m_pComCollector;	///< 通用收集器
};

#endif // datacollectionImp_h__
