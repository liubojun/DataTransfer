#include "tcpsocket.h"
#include <QDataStream>

TcpSocket::TcpSocket(QObject *parent /*= NULL*/) : QTcpSocket(parent)
{
    m_iMsgLen = 0;
}

void TcpSocket::readPendingDatagrams()
{
    while (true)
    {
        QDataStream stream(this);

        if (0 == m_iMsgLen)
        {
            if (this->bytesAvailable() < sizeof(quint16))
            {
                return;
            }

            stream >> m_iMsgLen;
        }

        if (this->bytesAvailable() < m_iMsgLen)
        {
            return;
        }

        QString strDirName;
        QString strDirId;
        QString strLogInfo;
        int iInfoType;
        stream >> strDirName >> strDirId >> strLogInfo >> iInfoType;
        emit showLog(strDirName, strDirId, strLogInfo, iInfoType);
        m_iMsgLen = 0;
    }
}


