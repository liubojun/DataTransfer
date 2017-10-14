#include "CltDispatch.h"
#include "shareddirCollector.h"

#include <QTimer>
CltDispatch::CltDispatch(CollectorBase *pParent)
{
    m_pParent = pParent;

    m_bHasFinished = true;
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

    if (m_bHasFinished)
    {
        QTimer::singleShot(0, this, SLOT(dowork()));
    }
    else
    {
        QSLOG_DEBUG(QStringLiteral("之前任务还未结束，本次任务不做处理."));
    }
}

void CltDispatch::dowork()
{
    m_bHasFinished = false;
    m_pParent->getNewFiles();
    m_bHasFinished = true;
}
