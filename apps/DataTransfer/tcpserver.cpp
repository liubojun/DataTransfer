#include "tcpserver.h"

void TcpServer::incomingConnection(qintptr handle)
{
    TcpSocket *tcpSocket = new TcpSocket(this);
    tcpSocket->setSocketDescriptor(handle);
    connect(tcpSocket, SIGNAL(readyRead()),
            tcpSocket, SLOT(readPendingDatagrams()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(deleteLater()));

    connect(tcpSocket, SIGNAL(showLog(const QString &, const QString &, const QString &, int)), this, SIGNAL(showLog(const QString &, const QString &, const QString &, int)));
}