#include "udplogsender.h"
#include "DataBase.h"
#include "ctkLog.h"
#include <QByteArray>
#include <QDataStream>

const QString COLLECTMSG = "Collect";
const QString CLEARMSG = "Clear";

CUdpLogSender::CUdpLogSender()
{
    GlobalConfig oGConfig;
    DataBase::getInstance()->queryBaseInfo(oGConfig);
    m_iUdpLogPort = oGConfig.nLogPort;
    m_iUdpBroadCastPort = oGConfig.nBroadcastPort;
}

CUdpLogSender::~CUdpLogSender()
{
    m_oLogSocket.close();
    m_oBroadCastSocket.close();
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
    qint64 iLen = m_oLogSocket.writeDatagram(datagram, QHostAddress::LocalHost, m_iUdpLogPort);
}

void CUdpLogSender::sendBroadCastMsg(const QString &taskName, const QString &file)
{
    if (!DataBase::getInstance()->globalConfig().bEnableBroadcast)
    {
        return;
    }
    // 构造json消息
    // {
    //     "TaskName" : "雷达文件传输",
    //     "DateTime" : "2018-06-22 15:40:32"
    //	   "File" : "Z_RADR_I_Z9220_20170423155400_O_DOR_SA_CAP.bin.bz2"
    // }
    QString strMsg = QString("{\n"
                             "      \"TaskName\" :\"%1\",\n"
                             "      \"DateTime\" :\"%2\",\n"
                             "      \"File\" :\"%3\"\n"
                             "}\n").arg(taskName).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(file);
    std::string strStdMsg = strMsg.toStdString();
    //QSLOG_DEBUG(strStdMsg.c_str());
    m_oBroadCastSocket.writeDatagram(strStdMsg.c_str(), strStdMsg.length(), QHostAddress::Broadcast, m_iUdpBroadCastPort);
}




