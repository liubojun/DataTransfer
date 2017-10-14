/**
  * @file   CltDispatch.h
  * @brief  调度的方式收集数据文件.
  *
  *  这里是详细说明。
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:04:19
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef CltDispatch_H_
#define CltDispatch_H_

#include <QThread>
#include "DispatchInterface.h"
class CollectorBase;

class CltDispatch : public DispatchInterface
{
    Q_OBJECT
public:
    CltDispatch(CollectorBase *pParent);
    ~CltDispatch();
public slots:
    /**
     * @brief  定时调用
     * @param  const QSharedPointer<TimerCallBackParam> & data
     * @return void：描述返回值
     */
    virtual void Dispatch(const QSharedPointer<TimerCallBackParam> &data);

    /**
     * @brief  工作入口
     * @return void：描述返回值
     */
    void dowork();
private:
    CollectorBase *m_pParent;

    bool m_bHasFinished;
};

#endif