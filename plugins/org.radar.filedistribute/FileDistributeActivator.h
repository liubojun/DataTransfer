#ifndef _H_H_FILEDISTRIBUTE20160524_H_H_
#define _H_H_FILEDISTRIBUTE20160524_H_H_

#include <ctkPluginActivator.h>

class IFileDistribute;
class IFileCrypt;

class CFileDistributeActivator : public QObject, public ctkPluginActivator
{
	Q_OBJECT
	Q_INTERFACES(ctkPluginActivator)
#ifdef WITH_QT5
	Q_PLUGIN_METADATA(IID "org_radar_filedistribute")
#endif

public:
	CFileDistributeActivator();
	virtual ~CFileDistributeActivator();
	virtual void start(ctkPluginContext *context);
	virtual void stop(ctkPluginContext *context);
	
private:	
	void DestroyResource();
	IFileDistribute *m_pFileDistribute;	
	IFileCrypt	*m_pFileCrypt;
};


#endif //_H_H_WRITEDATAFILEACTIVATOR_H_H_
