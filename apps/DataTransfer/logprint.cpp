#include "mainwindow.h"
#include "logprint.h"
#include "QsLog/ctkLog.h"

#include <QTimer>
#include <QDebug>
#include <QHostAddress>

LogPrintImpl::LogPrintImpl(MainWindow *in_pWnd) : m_pWnd(in_pWnd)//, server(RCF::TcpEndpoint(50001))
{

    //start();
    //m_bRun = true;
    //qDebug() << QThread::currentThreadId();

    int threadnum;
    bool enableLog;
    DataBase::getInstance()->queryBaseInfo(threadnum, m_iLogPort, enableLog);
    // server = new RCF::RcfServer(RCF::TcpEndpoint(logport));
    connect(this, SIGNAL(showLog(const QString &, const QString &, const QString &, int)), m_pWnd,
            SLOT(print(const QString &, const QString &, const QString &, int)));
    //m_oThread.start();
    //this->moveToThread(&m_oThread);
    if (!m_oUdpSocket.bind(QHostAddress::LocalHost, m_iLogPort))
        //if (!m_oServer.listen(QHostAddress::LocalHost, logport))
    {
        QSLOG_ERROR(QString("udp bind port failure, reason: %1").arg(m_oUdpSocket.errorString()));
    }
    else
    {
        QSLOG_DEBUG(QString("udp bind port %1 success").arg(m_iLogPort));
        //connect(&m_oServer, SIGNAL(showLog(const QString &, const QString &, const QString &, int)), this, SIGNAL(showLog(const QString &, const QString &, const QString &, int)));
        //connect(&m_oServer, SIGNAL(newConnection()), this, SLOT(newconnections()));
        connect(&m_oUdpSocket, SIGNAL(readyRead()),
                this, SLOT(readPendingDatagrams()));
    }

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpDate()));
    timer->start(10000);
}

LogPrintImpl::LogPrintImpl()
{
    //delete server;
    //server = NULL;
}

LogPrintImpl::~LogPrintImpl()
{
    //m_oThread.terminate();

    //m_oThread.wait();
}

//void LogPrintImpl::print(const string &dirName, const string &dirId, const string &info, int infoType)
//{
//    // QSLOG_DEBUG("PRINT1");
//    emit showLog(dirName, dirId, info, infoType);
//    //m_pWnd->print(dirName, dirId, info, infoType);
//}

void LogPrintImpl::readPendingDatagrams()
{
    //qDebug() << QThread::currentThreadId();
    while (m_oUdpSocket.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_oUdpSocket.pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_oUdpSocket.readDatagram(datagram.data(), datagram.size(),
                                  &sender, &senderPort);

        QDataStream stream(&datagram, QIODevice::ReadOnly);
        quint16 iMsgLen = 0;
        QString strDirName;
        QString strDirId;
        QString strLogInfo;
        int iInfoType;
        stream >> iMsgLen >> strDirName >> strDirId >> strLogInfo >> iInfoType;
        emit showLog(strDirName, strDirId, strLogInfo, iInfoType);
        // QSLOG_DEBUG("rcv msg");
        // processTheDatagram(datagram);
    }


}

void LogPrintImpl::run()
{
    //qDebug() << QThread::currentThreadId();
    QTimer::singleShot(0, this, SLOT(start()));


}

void LogPrintImpl::stop()
{
    //server->stop();
    //delete server;
    //server = NULL;
}

void LogPrintImpl::newconnections()
{

}

void LogPrintImpl::start()
{
    //QSLOG_DEBUG("start rcf server...");
    //try
    //{
    //    //RCF::ThreadPoolPtr tpPtr(new RCF::ThreadPool(5));
    //    //server.setThreadPool(tpPtr);


    //    server->bind<I_LogPrint>(*this);
    //    server->start();
    //}
    //catch (std::exception &ex)
    //{
    //    QSLOG_ERROR(QString("start rcf server error:").arg(ex.what()));
    //}
}

void LogPrintImpl::timerUpDate()
{
    if (m_oUdpSocket.bytesAvailable() != 0)
    {
        QSLOG_DEBUG(QString::fromLocal8Bit("接收不到数据，重启"));
        m_oUdpSocket.close();
        //m_oUdpSocket = new QUdpSocket(this);
        if (!m_oUdpSocket.bind(QHostAddress::LocalHost, m_iLogPort))
        {
            QSLOG_ERROR(QString("udp bind port failure, reason: %1").arg(m_oUdpSocket.errorString()));
        }
        else
        {
            QSLOG_DEBUG(QString("udp bind port %1 success").arg(m_iLogPort));
        }
    }
}
