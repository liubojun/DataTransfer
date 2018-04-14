#include "udplogsender.h"
#include "DataBase.h"
#include "ctkLog.h"
#include <QByteArray>
#include <QDataStream>

const QString COLLECTMSG = "Collect";
const QString CLEARMSG = "Clear";

CUdpLogSender::CUdpLogSender()
{
    int threadnum;
    bool enableLog;
    DataBase::getInstance()->queryBaseInfo(threadnum, m_iUdpLogPort, enableLog);
}

CUdpLogSender::~CUdpLogSender()
{
    m_oSocket.close();
}

void CUdpLogSender::sendCollectMsg(const QString &taskName, const QString &taskId, const QString &taskContent, int level)
{
    sendMsg(COLLECTMSG, taskName, taskId, taskContent, level);
}

void CUdpLogSender::sendClearMsg(const QString &taskName, const QString &taskId, const QString &taskContent, int level)
{
    sendMsg(CLEARMSG, taskName, taskId, taskContent, level);
}

void CUdpLogSender::sendMsg(const QString &msgType, const QString &taskName, const QString &taskId, const QString &taskContent, int level)
{
    QByteArray datagram;
    QDataStream stream(&datagram, QIODevice::WriteOnly);
    quint16 iMsgLen = 0;
    stream << iMsgLen << msgType << taskName << taskId << taskContent << level;
    stream.device()->seek(0);
    iMsgLen = datagram.length() - sizeof(quint16);
    stream << iMsgLen;
    qint64 iLen = m_oSocket.writeDatagram(datagram, QHostAddress::LocalHost, m_iUdpLogPort);
}




