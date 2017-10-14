#include "ccrontimer.h"
#include <limits.h>
#include <assert.h>
#include <QDateTime>
#include <QTimerEvent>
#include <stdio.h>
#include <time.h>
#include <QDebug>
#include <QMutexLocker>
#include <QMetaType>

#include "DispatchInterface.h"

CCronTimer::CCronTimer(DispatchInterface *in_pCallBackObject, const QSharedPointer<TimerCallBackParam> &in_callBackParam)
{
    c_singleShot = false;
    c_timerId = 0;
    c_nextExec = -1;

    m_pCallBackObject = in_pCallBackObject;
    m_oCallBackParam = in_callBackParam;
}


void CCronTimer::setSingleShot(bool singleShot)
{
    c_singleShot = singleShot;
}

void CCronTimer::start(const CCronCalculator &cc)
{
    c_cronCalc.setCronspec(cc.getCronspec());
    QMutexLocker lock(&c_mtxNextExec);
    c_nextExec = c_cronCalc.nextExecution();
    c_timerId = startTimer(calcDiffTime());
    assert(c_timerId);
}

void CCronTimer::stop()
{
    QMutexLocker lock(&c_mtxNextExec);
    c_nextExec = -1;
    killTimer(c_timerId);
}

void CCronTimer::timerEvent(QTimerEvent *pEvent)
{
    if(pEvent->timerId() != c_timerId)
        return;

    // qDebug() << "intern timeout";
    QMutexLocker lock(&c_mtxNextExec);
    killTimer(c_timerId);
    c_timerId = 0;
    if(c_nextExec > time(NULL))
    {
        c_timerId = startTimer(calcDiffTime());
        assert(c_timerId);
    }
    else
    {
        // 超时，将需要传递的参数抛出
        // emit timeout(m_oCallBackParam);
        m_pCallBackObject->Dispatch(m_oCallBackParam);
        if(!c_singleShot)
        {
            c_nextExec = c_cronCalc.nextExecution();
            c_timerId = startTimer(calcDiffTime());
        }
    }
}

time_t CCronTimer::calcDiffTime()
{
    time_t t = QDateTime::currentDateTime().msecsTo(QDateTime::fromTime_t(0).addSecs(c_nextExec));
    if(t < 0)
        //return 0;
        return 1000;	// 默认1s[11/18/2016 wujun]
    if(t > INT_MAX)
        return INT_MAX;
    return t;
}
