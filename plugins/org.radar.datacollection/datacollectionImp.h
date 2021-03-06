﻿/**
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

    /**
     * @brief  获取本机IP地址
     * @param  QString & strIP
     * @return void：描述返回值
     */
    //void getIpaddr(QString &strIP);
    QString m_strIPaddr;

    void startComCollection();

    void stopComCollection();

private:
    QSharedPointer<CollectManager>	m_pComCollector;	///< 通用收集器
};

#endif // datacollectionImp_h__
