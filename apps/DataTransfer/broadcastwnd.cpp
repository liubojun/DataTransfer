#include "broadcastwnd.h"
#include "DataBase.h"
#include "QsLog/ctkLog.h"

CBroadCastWnd::CBroadCastWnd(QDialog *parent /*= NULL*/) : QDialog(parent)
{
    ui.setupUi(this);

    GlobalConfig oGConfig;
    DataBase::getInstance()->queryBaseInfo(oGConfig);

    m_oBindPort = oGConfig.nBroadcastPort;

    if (!m_oSocket.bind(m_oBindPort, QUdpSocket::ShareAddress))
    {
        QSLOG_ERROR(QString("udp bind port failure, reason: %1").arg(m_oSocket.errorString()));
    }

    connect(&m_oSocket, SIGNAL(readyRead()), this, SLOT(rcvMsg()));
}

CBroadCastWnd::~CBroadCastWnd()
{
    m_oSocket.close();
}

void CBroadCastWnd::rcvMsg()
{
    while (m_oSocket.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_oSocket.pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_oSocket.readDatagram(datagram.data(), datagram.size(),
                               &sender, &senderPort);

        ui.broadcastText->append(datagram);
    }
}
