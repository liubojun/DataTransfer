#ifndef tcpsocket_h__
#define tcpsocket_h__

#include <QTcpSocket>
#include <QTcpServer>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpSocket(QObject *parent = NULL);
signals:
    void showLog(const QString &dirName, const QString &dirId, const QString &info, int infoType);
public slots:

    void readPendingDatagrams();

private:
    quint16 m_iMsgLen;
};


#endif // tcpsocket_h__
