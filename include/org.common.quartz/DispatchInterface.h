#ifndef DISPATCHINTERFACE_H_H
#define DISPATCHINTERFACE_H_H

#include <QObject>
#include <QSharedPointer>
#include "TimerCallBackParam.h"

class DispatchInterface : public QObject
{
	Q_OBJECT
public slots:
	virtual void Dispatch(const TimerParam/*QSharedPointer<TimerCallBackParam>*/ &data) = 0;
};

#endif // DISPATCHINTERFACE_H_H