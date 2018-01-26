#ifndef logprint_h__
#define logprint_h__

#include <string>
#include <RCF/RCF.hpp>
#include <QThread>

#include "logprotocol.h"

using namespace std;


class MainWindow;

class LogPrintImpl : public QObject
{
    Q_OBJECT
public:

    LogPrintImpl(MainWindow *in_pWnd);

    LogPrintImpl();

    ~LogPrintImpl();

    void run();

    void stop();

    void print(const string &dirName, const string &dirId, const string &info, int infoType);

public slots:

    void start();
signals:
    void showLog(const string &dirName, const string &dirId, const string &info, int infoType);
private:
    MainWindow *m_pWnd;
    RCF::RcfInitDeinit rcfInit;
    RCF::RcfServer *server;
    QThread m_oThread;
};

#endif // logprint_h__
