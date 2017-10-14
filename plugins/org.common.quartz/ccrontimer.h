#ifndef CCRONTIMER_H
#define CCRONTIMER_H

//#include "crontimer_global.h"

#include <QObject>

#include "ccroncalculator.h"
#include "TimerCallBackParam.h"
#include <QTimer>
#include <QMutex>
#include <QSharedPointer>

class QTimerEvent;
class DispatchInterface;

class CCronTimer : public QObject
{
    Q_OBJECT
public:

    CCronTimer(DispatchInterface *in_pCallBackObject, const QSharedPointer<TimerCallBackParam> &in_callBackParam);

    void setSingleShot(bool singleShot);

signals:
    void timeout(const TimerParam/*QSharedPointer<TimerCallBackParam>*/ &o_callBackParam);

public slots:
    void start(const CCronCalculator& cc);
    void stop();

private:
    CCronCalculator c_cronCalc;
    bool c_singleShot;
    int c_timerId;
    time_t c_nextExec;
    QMutex c_mtxNextExec;

    void timerEvent(QTimerEvent *pEvent);

    time_t calcDiffTime();

    QSharedPointer<TimerCallBackParam> m_oCallBackParam;

    DispatchInterface *m_pCallBackObject;
};

#endif // CCRONTIMER_H
