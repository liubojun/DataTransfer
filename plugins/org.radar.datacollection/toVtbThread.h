#ifndef toVtbThread_H_
#define toVtbThread_H_

#include <QThreadPool>
#include <QSharedPointer>
#include "fileMsg.h"

class CollectorBase;
class IRadarBaseDataParse;

class ToVtbTask : public QRunnable
{
public:
    // ToVtbTask();
    ToVtbTask(CollectorBase *pParent, IRadarBaseDataParse *pPaser, FileInfo fileInfo);
    ~ToVtbTask();

    virtual void run();

private:
    CollectorBase *m_pParent;
    FileInfo m_fileInfo;
    IRadarBaseDataParse *m_pBaseParser;
};

#endif