/**
  * @file   IDataClear.h
  * @brief  数据清除接口.
  *
  *  这里是详细说明。
  * @author 刘伯俊
  * @par   Copyright (c):
  *          刘伯俊.
  *         All Rights Reserved
  * @date   2016:04:08
  * @note   这里是注意事项。
  * @version 1.0
  */

#ifndef IDataClear_h__
#define IDataClear_h__

#include <QString>
#include <string>
using namespace std;
#include "IBaseInterface.h"
#include "BaseDatas.h"

class DataClearSet;
class DataClearSetList;

/**
  * @class <>
  * @brief  数据清除接口.
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class IDataClear : public IBaseInterface
{
    Q_OBJECT
    Q_INTERFACES(IBaseInterface)	// tell Qt which interfaces the class implements

public:
    virtual ~IDataClear() {}

    /**
     * @brief  启动一个数据清除
     * @param  const BaseDatas & data：数据清除设置
     * @param  QSharedPointer<QThread> pThread：线程指针
     * @return bool：成功返回true，失败返回false
     */
    virtual bool start(const BaseDatas &data) = 0;

    virtual bool start(const QList<BaseDatas> &m_datas, QSharedPointer<QThread> pThread) = 0;

    virtual void stop(const BaseDatas &m_datas) = 0;

    virtual void stop(const QList<BaseDatas> &m_datas) = 0;

    virtual const QList<BaseDatas> &get() = 0;

    // 暴露给前台的设置函数
    /**
     * @brief  读取数据清除配置文件
     * @param  DataClearSetList & allsets：数据清除设置的集合
     * @param  QString & strPath：配置文件路径
     * @return bool：成功返回true，失败返回false
     */
    virtual bool ReadClearSet(DataClearSetList &allsets, QString &strPath) = 0;
    /**
     * @brief  添加一个数据清除任务
     * @param  const DataClearSet & set：数据清除设置
     * @return void：无
     */
    virtual void AddClearSet(const DataClearSet &set) = 0;
    /**
     * @brief  删除一个数据清除任务
     * @param  const string & strPath：清除的路径
     * @return void：无
     */
    virtual void DelClearSet(const string &strPath) = 0;
    /**
     * @brief  修改一个数据清除任务
     * @param  const DataClearSet & set：数据清除设置
     * @return void：无
     */
    virtual void MdyClearSet(const DataClearSet &set) = 0;

signals:
    void taskBegin(const QString &dir);
    void taskEnd(const QString &dir);
public slots:
    virtual void doNow(const QString &dir) = 0;
};

Q_DECLARE_INTERFACE(IDataClear, "org.radar.dataclear.IDataClear")

#endif // IDataClear_h__