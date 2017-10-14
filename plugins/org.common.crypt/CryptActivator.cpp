#include <QtPlugin>

#include "CryptActivator.h"
#include "CryptInfo.h"

// 是否开启内存泄露检查，该宏定义在cmake中
#ifdef WITH_MEM_CHECK
#include "vld.h"
#endif



CryptActivator::CryptActivator()
{
}

CryptActivator::~CryptActivator()
{

}

void CryptActivator::start( ctkPluginContext *context )
{
	m_pCrypt = new CryptInfo();
	context->registerService(QStringList("ICrypt"), m_pCrypt);
}

void CryptActivator::stop( ctkPluginContext *context )
{
	if (NULL != m_pCrypt)
	{
		delete m_pCrypt;
		m_pCrypt = NULL;
	}

	delete this;
}

#ifndef WITH_QT5
Q_EXPORT_PLUGIN2(org_common_Crypt, CryptActivator)
#endif
