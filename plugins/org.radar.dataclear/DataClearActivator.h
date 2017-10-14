#ifndef DataClearActivator_h__
#define DataClearActivator_h__

#include <ctkPluginActivator.h>
#include "IDataClear.h"

class DataClearActivator : public QObject, public ctkPluginActivator
{
	Q_OBJECT
	Q_INTERFACES(ctkPluginActivator)
#ifdef WITH_QT5
	Q_PLUGIN_METADATA(IID "org_radar_dataclear")
#endif

public:
	DataClearActivator();
	virtual ~DataClearActivator();
	virtual void start(ctkPluginContext *context);
	virtual void stop(ctkPluginContext *context);

private:	
	void DestroyResource();
	IDataClear *m_pIDataClear;	
};
#endif // DataClearActivator_h__