#include "mainwindow.h"
#include "logprint.h"
#include "QsLog/ctkLog.h"

#include <QTimer>

LogPrintImpl::LogPrintImpl(MainWindow *in_pWnd) : m_pWnd(in_pWnd), server(RCF::TcpEndpoint(50001))
{
    connect(this, SIGNAL(showLog(const string &, const string &, const string &, int)), m_pWnd,
            SLOT(print(const string &, const string &, const string &, int)));
    m_oThread.start();
    this->moveToThread(&m_oThread);
}

LogPrintImpl::LogPrintImpl()
{

}

LogPrintImpl::~LogPrintImpl()
{

}

void LogPrintImpl::print(const string &dirName, const string &dirId, const string &info, int infoType)
{
    QSLOG_DEBUG("PRINT1");
    emit showLog(dirName, dirId, info, infoType);
    //m_pWnd->print(dirName, dirId, info, infoType);
}

void LogPrintImpl::run()
{
    QTimer::singleShot(0, this, SLOT(start()));
}

void LogPrintImpl::stop()
{
    server.stop();
}

void LogPrintImpl::start()
{
    QSLOG_DEBUG("start rcf server...");
    try
    {
        //RCF::ThreadPoolPtr tpPtr(new RCF::ThreadPool(5));
        //server.setThreadPool(tpPtr);


        server.bind<I_LogPrint>(*this);
        server.start();
    }
    catch (std::exception &ex)
    {
        QSLOG_ERROR(QString("start rcf server error:").arg(ex.what()));
    }
}
