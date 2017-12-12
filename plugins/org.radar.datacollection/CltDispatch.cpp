#include "CltDispatch.h"
#include "shareddirCollector.h"

#include <QTimer>
CltDispatch::CltDispatch(CollectorBase *pParent)
{
    m_pParent = pParent;

    //m_bHasFinished = true;
}

CltDispatch::~CltDispatch()
{
    m_pParent = NULL;
}

void CltDispatch::Dispatch(const QSharedPointer<TimerCallBackParam> &data)
{
    if (NULL == m_pParent)
    {
        return;
    }
    //QSLOG_DEBUG("DISPATHCH1");
    QtConcurrent::run(m_pParent, &CollectorBase::getNewFiles);
    //QTimer::singleShot(0, this, SLOT(dowork()));
}

void CltDispatch::dowork()
{
    //m_bHasFinished = false;
    //QSLOG_DEBUG("DISPATHCH2");
    m_pParent->getNewFiles();
    //m_bHasFinished = true;
}
