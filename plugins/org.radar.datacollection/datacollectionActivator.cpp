#include <QtPlugin>

#include "datacollectionActivator.h"
#include "datacollectionImp.h"

// 是否开启内存泄露检查，该宏定义在cmake中
#ifdef WITH_MEM_CHECK
#include "vld.h"
#endif

DataCollectionActivator::DataCollectionActivator()
	: m_pDataCollection(NULL)
{
}

DataCollectionActivator::~DataCollectionActivator()
{
	if (NULL != m_pDataCollection)
	{
		delete m_pDataCollection;
		m_pDataCollection = NULL;
	}
}

void DataCollectionActivator::start( ctkPluginContext *context )
{
	m_pDataCollection = new DataCollectionImp();
	context->registerService(QStringList("IDataCollection"), m_pDataCollection);
}

void DataCollectionActivator::stop( ctkPluginContext *context )
{
	if (NULL != m_pDataCollection)
	{
		delete m_pDataCollection;
		m_pDataCollection = NULL;
	}

	delete this;
}

// Qt5之后已经移除了此功能
#ifndef WITH_QT5
Q_EXPORT_PLUGIN2(org_radar_datacollection, DataCollectionActivator)
#endif

