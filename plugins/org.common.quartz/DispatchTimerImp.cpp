#include "DispatchTimerImp.h"
#include "ccroncalculator.h"
#include "ccrontimer.h"
#include <QMetaType>
#include "ctkLog.h"

//typedef QSharedPointer<TimerCallBackParam> MyParam;

bool DispatchTimerImp::SetDispatchTimer(const std::string &cronspec , DispatchInterface* pObject, const QSharedPointer<TimerCallBackParam> &callBackParam, QSharedPointer<QObject> &pDelObj)
{
    qRegisterMetaType<CCronCalculator>("CCronCalculator");
    qRegisterMetaType<QSharedPointer<TimerCallBackParam>>("TimerParam");	//多线程中使用信号槽需要注册

    pDelObj = QSharedPointer<QObject>(new CCronTimer(pObject, callBackParam));
    CCronTimer *cT = dynamic_cast<CCronTimer *>(pDelObj.data());

    if ( pDelObj != NULL && cT != NULL)
    {
        if (!QMetaObject::invokeMethod( cT, "start", Qt::QueuedConnection, Q_ARG(CCronCalculator, CCronCalculator(cronspec)) ))
        {
            return false;
        }

        // 参数中转：QSharedPointer<TimerCallBackParam>
        //QObject::connect( cT, SIGNAL(timeout(const QSharedPointer<TimerCallBackParam> &)), pObject, SLOT(Dispatch(const QSharedPointer<TimerCallBackParam> &)) );
        // QObject::connect( cT, SIGNAL(timeout(const TimerParam &)), pObject, SLOT(Dispatch(const TimerParam &)) );

        return true;
    }
    return false;
}

void DispatchTimerImp::stopDispatchTimer(QSharedPointer<QObject> &pDelObj)
{
    CCronTimer *cT = dynamic_cast<CCronTimer *>(pDelObj.data());
    if (cT != NULL)
    {
        cT->stop();
    }
}
