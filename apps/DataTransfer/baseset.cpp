#include "baseset.h"
#include "DataBase.h"
#include <QThreadPool>

CBaseSetDlg::CBaseSetDlg(QWidget *parent /*= NULL*/)
{
    ui.setupUi(this);
    init();
}

CBaseSetDlg::~CBaseSetDlg()
{

}


void CBaseSetDlg::onApply()
{
    int iThreadCount = ui.spinBox_threadnum->text().toInt();
    int iLogPort = ui.spinBox_logport->text().toInt();
    bool enableLog = ui.checkBox_writeLog->isChecked();

    DataBase::getInstance()->updateBaseInfo(iThreadCount, iLogPort, enableLog);
    QThreadPool::globalInstance()->setMaxThreadCount(iThreadCount);
}


void CBaseSetDlg::onOk()
{
    onApply();
    emit wndquit();
}

void CBaseSetDlg::init()
{
    connect(ui.btn_apply, SIGNAL(pressed()), this, SLOT(onApply()));
    connect(ui.btn_cancel, SIGNAL(pressed()), this, SIGNAL(wndcancel()));
    connect(ui.btn_ok, SIGNAL(pressed()), this, SLOT(onOk()));

    // ²éÑ¯Êý¾Ý¿â
    int threadnum = 0;
    int portNum = 0;
    bool enableLog = false;
    DataBase::getInstance()->queryBaseInfo(threadnum, portNum, enableLog);
    ui.spinBox_threadnum->setValue(threadnum);
    ui.spinBox_logport->setValue(portNum);
    ui.checkBox_writeLog->setChecked(enableLog);
}

