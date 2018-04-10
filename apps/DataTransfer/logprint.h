#ifndef logprint_h__
#define logprint_h__

#include <string>
#include <RCF/RCF.hpp>
#include <QThread>
#include <QUdpSocket>
//#include "tcpserver.h"
#include "logprotocol.h"

using namespace std;


class MainWindow;


class LogPrintImpl : public QThread
{
    Q_OBJECT
public:

    LogPrintImpl(MainWindow *in_pWnd);

    LogPrintImpl();

    ~LogPrintImpl();

    void run();

    void stop();

    //void print(const string &dirName, const string &dirId, const string &info, int infoType);
    void print(const QString &dirName, const QString &dirId, const QString &info, int infoType);

public slots:

    void start();

    void readPendingDatagrams();

    void newconnections();

    void timerUpDate();
signals:
    void showLog(const QString &dirName, const QString &dirId, const QString &info, int infoType);
private:
    MainWindow *m_pWnd;
    //RCF::RcfInitDeinit rcfInit;
    //RCF::RcfServer *server;
    //QThread m_oThread;

    QUdpSocket m_oUdpSocket;
    int m_iLogPort;
    // QTcpSocket m_oUdpSocket;
    // TcpServer m_oServer;
};


#endif // logprint_h__
