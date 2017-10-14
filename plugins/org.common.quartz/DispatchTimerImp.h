#ifndef dispatchtimerimp_h__
#define dispatchtimerimp_h__

#include "IDispatchTimer.h"

class DispatchTimerImp: public IDispatchTimer
{
	Q_OBJECT
	Q_INTERFACES(IDispatchTimer)
	Q_INTERFACES(IBaseInterface)
	
public:
	
	virtual bool SetDispatchTimer(const std::string &cronspec , DispatchInterface* pObject, const QSharedPointer<TimerCallBackParam> &callBackParam, QSharedPointer<QObject> &pDelObj);

	virtual void stopDispatchTimer(QSharedPointer<QObject> &pDelObj);

};

#endif // dispatchtimerimp_h__