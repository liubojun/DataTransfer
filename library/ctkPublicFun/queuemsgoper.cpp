#include "queuemsgoper.h"

#include "msgqueue.h"
#include <QString>
#include <QCoreApplication>

QReadWriteLock CQueueMsgOper::s_oMutex;

CQueueMsgOper::CQueueMsgOper()
{

}

CQueueMsgOper::~CQueueMsgOper()
{

}

bool CQueueMsgOper::getQueueGroup(CMsgQueueGroup &io_queue)
{
    s_oMutex.lockForRead();
    if (!DeserializeXml2Queue(QString(qApp->applicationDirPath() + "/config/QueueTask.xml").toLocal8Bit().data(), io_queue))
    {
        s_oMutex.unlock();
        return false;
    }

    s_oMutex.unlock();

    return true;
}

bool CQueueMsgOper::saveQueueGroup(const CMsgQueueGroup &in_queue)
{
    s_oMutex.lockForWrite();

    if (!SerializeQueue2Xml(in_queue, QString(qApp->applicationDirPath() + "/config/QueueTask.xml").toLocal8Bit().data()))
    {
        s_oMutex.unlock();
        return false;
    }
    s_oMutex.unlock();

    return true;
}



