#include <QtPlugin>

#include "DispatchTimerActivator.h"
#include "DispatchTimerImp.h"

// 是否开启内存泄露检查，该宏定义在cmake中
#ifdef WITH_MEM_CHECK
#include "vld.h"
#endif

DispatchTimerActivator::DispatchTimerActivator():m_pDispatchTimer( NULL )
{
}

DispatchTimerActivator::~DispatchTimerActivator()
{

}

void DispatchTimerActivator::start( ctkPluginContext *context )
{
	if ( m_pDispatchTimer == NULL)
	{
		m_pDispatchTimer = new DispatchTimerImp();
	}	

	context->registerService(QStringList("IDispatchTimer"), m_pDispatchTimer);
}

void DispatchTimerActivator::stop( ctkPluginContext *context )
{
	if (NULL != m_pDispatchTimer)
	{
		delete m_pDispatchTimer;
		m_pDispatchTimer = NULL;
	}

	delete this;
}
#ifndef WITH_QT5
Q_EXPORT_PLUGIN2(org_common_Quartz, DispatchTimerActivator)
#endif

