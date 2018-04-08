#include "mainwindow.h"
#include "logprint.h"
#include "QsLog/ctkLog.h"

#include <QTimer>
#include <QDebug>

LogPrintImpl::LogPrintImpl(MainWindow *in_pWnd) : m_pWnd(in_pWnd)//, server(RCF::TcpEndpoint(50001))
{

    //start();
    //m_bRun = true;
    //qDebug() << QThread::currentThreadId();

    int threadnum, logport;
    bool enableLog;
    DataBase::getInstance()->queryBaseInfo(threadnum, logport, enableLog);
    //server = new RCF::RcfServer(RCF::TcpEndpoint(logport));
    connect(this, SIGNAL(showLog(const QString &, const QString &, const QString &, int)), m_pWnd,
            SLOT(print(const QString &, const QString &, const QString &, int)));
    //m_oThread.start();
    //this->moveToThread(&m_oThread);
    // if (!m_oUdpSocket.bind(QHostAddress::LocalHost, logport))
	if (!m_oServer.listen(QHostAddress::LocalHost, logport))
    {
        //QSLOG_ERROR(QString("udp bind port failure, reason: %1").arg(m_oUdpSocket.errorString()));
    }
    else
    {
        QSLOG_DEBUG(QString("tcp bind port %1 success").arg(logport));
		connect(&m_oServer, SIGNAL(showLog(const QString &, const QString &, const QString &, int)), this, SIGNAL(showLog(const QString &, const QString &, const QString &, int)));
		//connect(&m_oServer, SIGNAL(newConnection()), this, SLOT(newconnections()));
        //connect(&m_oUdpSocket, SIGNAL(readyRead()),
        //        this, SLOT(readPendingDatagrams()));
    }
}

LogPrintImpl::LogPrintImpl()
{
    //delete server;
    //server = NULL;
}

LogPrintImpl::~LogPrintImpl()
{

}

//void LogPrintImpl::print(const QString &dirName, const QString &dirId, const QString &info, int infoType)
//{
//    // QSLOG_DEBUG("PRINT1");
//    emit showLog(dirName, dirId, info, infoType);
//    //m_pWnd->print(dirName, dirId, info, infoType);
//}

void LogPrintImpl::readPendingDatagrams()
{
    //qDebug() << QThread::currentThreadId();
    //while (m_oUdpSocket.hasPendingDatagrams())
    //{
    //    QByteArray datagram;
    //    datagram.resize(m_oUdpSocket.pendingDatagramSize());
    //    QHostAddress sender;
    //    quint16 senderPort;

    //    m_oUdpSocket.readDatagram(datagram.data(), datagram.size(),
    //                              &sender, &senderPort);

    //    QDataStream stream(&datagram, QIODevice::ReadOnly);
    //    QString strDirName;
    //    QString strDirId;
    //    QString strLogInfo;
    //    int iInfoType;
    //    stream >> strDirName >> strDirId >> strLogInfo >> iInfoType;
    //    emit showLog(strDirName, strDirId, strLogInfo, iInfoType);
    //    // processTheDatagram(datagram);
    //}


}

void LogPrintImpl::run()
{
    //qDebug() << QThread::currentThreadId();
    //QTimer::singleShot(0, this, SLOT(start()));


}

void LogPrintImpl::stop()
{
    //server->stop();
}

void LogPrintImpl::newconnections()
{
	QTcpSocket *tcpSocket = m_oServer.nextPendingConnection();
	connect(tcpSocket, SIGNAL(readyRead()),
		this, SLOT(readPendingDatagrams()));
	connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

//void LogPrintImpl::start()
//{
//    //QSLOG_DEBUG("start rcf server...");
//    //try
//    //{
//    //    //RCF::ThreadPoolPtr tpPtr(new RCF::ThreadPool(5));
//    //    //server.setThreadPool(tpPtr);
//
//
//    //    server->bind<I_LogPrint>(*this);
//    //    server->start();
//    //}
//    //catch (std::exception &ex)
//    //{
//    //    QSLOG_ERROR(QString("start rcf server error:").arg(ex.what()));
//    //}
//}
