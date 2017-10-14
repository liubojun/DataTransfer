#include "FileDistributeActivator.h"
#include "FileDistribute.h"
#include "FileCrypt.h"

// 是否开启内存泄露检查，该宏定义在cmake中
#ifdef WITH_MEM_CHECK
#include "vld.h"
#endif

CFileDistributeActivator::CFileDistributeActivator()
    : m_pFileDistribute(NULL)
    , m_pFileCrypt(NULL)
{
}

CFileDistributeActivator::~CFileDistributeActivator()
{

}

void CFileDistributeActivator::start(ctkPluginContext *context)
{
    if ( m_pFileDistribute == NULL )
    {
        m_pFileDistribute = new FileDistribute();

        Q_ASSERT( m_pFileDistribute != NULL );
    }

    if ( m_pFileCrypt == NULL )
    {
        m_pFileCrypt = new FileCryptImp();
        Q_ASSERT(m_pFileCrypt != NULL);
    }

    context->registerService("IFileDistribute", m_pFileDistribute);

    context->registerService("IFileCrypt", m_pFileCrypt);
}

void CFileDistributeActivator::stop(ctkPluginContext *context)
{
    DestroyResource();
}

void CFileDistributeActivator::DestroyResource()
{
    if ( m_pFileDistribute != NULL )
    {
        delete m_pFileDistribute;
        m_pFileDistribute = NULL;
    }

    if ( m_pFileCrypt != NULL )
    {
        delete m_pFileCrypt;
        m_pFileCrypt = NULL;
    }

    delete this;
}

#ifndef WITH_QT5
Q_EXPORT_PLUGIN2(org_radar_filedistribute, CFileDistributeActivator)
#endif
