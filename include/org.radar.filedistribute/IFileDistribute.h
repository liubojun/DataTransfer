/*************************************************************************
* Copyright(c) 2016,NRIET
* All rights reserved.
* @file		:	IFileDistribute.h
* @brief	:	雷达服务器中文件分发插件接口类声明头文件
* @version	:	V1.0
* @author	:	NRIET
* @date		:	2016-05-24
------------------------------------------------------------------------------------------------------
* @modify	:	修改说明
* @brief	:	修改内容
* @author	:	修改者
* @date		:	修改日期
* @version	:	替换版本
**************************************************************************/

#ifndef _H_H_IFILEDISTRIBUTE20160524_H_H_
#define _H_H_IFILEDISTRIBUTE20160524_H_H_

#include <QtPlugin>
#include <QObject>
#include <QWidget>
#include <windows.h>
#include <vector>
#include "../interface/IBaseInterface.h"
#include "../ctkProtocol/fileMsg.h"
#include "../ctkProtocol/collectSet.h"
#include "../ctkProtocol/transFileInfo.h"
#include "../ctkProtocol/transferSet.h"
#include "IDistributeManager.h"

#pragma warning(disable:4996)

class IFileDistribute : public IBaseInterface
{
    Q_OBJECT
    Q_INTERFACES(IBaseInterface)

public:
    virtual ~IFileDistribute() {}

    /**
     * @brief  创建数据分发管家
     * @return QSharedPointer<FileDistributeThread>：实例指针
     */
    virtual QSharedPointer<IDistributeManager> createDistributeManager() = 0;

    //////////////////////////////////////////////////////////////////////////
    // 以下方法可能要被抛弃

    /**
     * @brief  设置是否发送信号
     * @param  bool sendMsg：是否向父窗口发送实时处理信息
     * @return void：无
     */
    virtual void SetSendMsg(bool sendMsg) = 0;

    /**
     * @brief  停止一个分发任务
     * @param  const string & dirId：收集目录ID
     * @return void：无
     */
    //virtual void StopSendTask(const string &dirId) = 0;

//     /**
//      * @brief  获取当前下载状态
//      * @return bool：true表示有任务在下载，false表示无任务下载
//      */
//     virtual bool HasTask() = 0;

    /**
     * @brief  获取处理结果
     * @param  vector<TransFileInfo> & r_result：存放结果
     * @return void：无
     */
    //virtual void GetProcessResult(vector<TransFileInfo>& r_result) = 0;

public slots:
    /**
     * @brief  添加发布任务
     * @param  const TransferSet & r_set：分发任务
     * @return void：无
     */
    //virtual void AddSendTask(const TransferSet & r_set) = 0;

    /**
     * @brief  使用线程池的方式分发数据（临时测试）
     * @param  const TransferSet & tSet
     * @return void：无
     */
    //virtual void AddTransferTask(const TransferSet &tSet) = 0;
};

Q_DECLARE_INTERFACE(IFileDistribute, "org.radar.filedistribute.IFileDistribute")

#endif//_H_H_IFILEDISTRIBUTE20160524_H_H_
