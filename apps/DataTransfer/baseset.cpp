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
    int iBroadcastPort = ui.broadcastport->text().toInt();
    bool enableBroadCast = ui.checkBox_enableBroadCast->isChecked();

    GlobalConfig oGConfig;
    oGConfig.bEnableLog = enableLog;
    oGConfig.nLogPort = iLogPort;
    oGConfig.nThreadNum = iThreadCount;
    oGConfig.nBroadcastPort = iBroadcastPort;
    oGConfig.bEnableBroadcast = enableBroadCast;
    DataBase::getInstance()->updateBaseInfo(oGConfig);
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
    GlobalConfig oGConfig;
    DataBase::getInstance()->queryBaseInfo(oGConfig);
    ui.spinBox_threadnum->setValue(oGConfig.nThreadNum);
    ui.spinBox_logport->setValue(oGConfig.nLogPort);
    ui.checkBox_writeLog->setChecked(oGConfig.bEnableLog);
    ui.checkBox_enableBroadCast->setChecked(oGConfig.bEnableBroadcast);
}

