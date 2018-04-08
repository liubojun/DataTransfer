#ifndef tcpserver_h__
#define tcpserver_h__


#include <QTcpServer>
#include "tcpsocket.h"

class TcpServer : public QTcpServer
{
	Q_OBJECT
public:
	virtual void incomingConnection(qintptr handle);

signals:
	void showLog(const QString &dirName, const QString &dirId, const QString &info, int infoType);
};

#endif // tcpserver_h__