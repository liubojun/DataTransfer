#include "DataClearActivator.h"
#include "DataClear.h"

//是否开启内存泄露检查，该宏定义在cmake中
#ifdef WITH_MEM_CHECK
#include "vld.h"
#endif

DataClearActivator::DataClearActivator():m_pIDataClear(NULL)
{
}
DataClearActivator::~DataClearActivator()
{

}
void DataClearActivator::start(ctkPluginContext *context)
{
	if ( m_pIDataClear == NULL )
	{
		m_pIDataClear = new DataClear();

		Q_ASSERT( m_pIDataClear != NULL );
	}

	context->registerService("IDataClear", m_pIDataClear);	
}

void DataClearActivator::stop(ctkPluginContext *context)
{
	DestroyResource();
}

void DataClearActivator::DestroyResource()
{
	if ( m_pIDataClear != NULL )
	{
		delete m_pIDataClear;
		m_pIDataClear = NULL;
	}
	delete this;
}

#ifndef WITH_QT5
Q_EXPORT_PLUGIN2(org_radar_dataclear, DataClearActivator)
#endif