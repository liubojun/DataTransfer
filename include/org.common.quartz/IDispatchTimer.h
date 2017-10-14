#ifndef idispatchtimer_h__
#define idispatchtimer_h__

#include <QtPlugin>
#include <QObject>
#include <string>
#include <QSharedPointer>
#include "DispatchInterface.h"
#include "IBaseInterface.h"
#include "TimerCallBackParam.h"

class IDispatchTimer: public IBaseInterface
{
    Q_OBJECT
    Q_INTERFACES(IBaseInterface)

public:
    virtual ~IDispatchTimer() {}

    /**
     * @brief  设置定时调度并启动
     * @param  const std::string & cronspec：调度规则
     * @param  DispatchInterface * pObject：响应调度的对象
     * @param  const QSharedPointer<TimerCallBackParam> & callBackParam：传递的参数
     * @param  QSharedPointer<QObject> & pDelObj：调度对象指针
     * @return bool：成功返回true，失败返回false
     */
    virtual bool SetDispatchTimer(const std::string &cronspec, DispatchInterface* pObject, const QSharedPointer<TimerCallBackParam> &callBackParam, QSharedPointer<QObject> &pDelObj) = 0;

    /**
     * @brief  停止定时调度
     * @param  QSharedPointer<QObject> & pDelObj：调度对象指针
     * @return void：无
     */
    virtual void stopDispatchTimer(QSharedPointer<QObject> &pDelObj) = 0;
};

//Q_DECLARE_INTERFACE(IDispatchTimer, "org.common.IDispatchTimer")
Q_DECLARE_INTERFACE(IDispatchTimer, "org.common.quartz.IDispatchTimer")


#endif // idispatchtimer_h__