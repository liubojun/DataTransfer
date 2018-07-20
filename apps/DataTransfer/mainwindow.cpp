#include "mainwindow.h"
#include "ctkLog.h"
#include <QMetaType>
#include <QDesktopServices>
#include <QMessageBox>
#include <QTimer>
#include <QUrl>
//--测试
#include <QFileInfo>
#include <QDir>
#include <QProcess>
#include <QTextCodec>
#include <QThread>
#include <QProcess>
#include "dataclearDlg.h"
#include "clearItemWidget.h"
#include "pathbuilder.h"
#include "IDispatchTimer.h"
#include "systemset.h"
#include "broadcastwnd.h"

//#include "quartz.h"
//--

MainWindow::MainWindow(ICtkPluginManager *ctk,QMainWindow *parent /*= NULL*/)
    : m_sqlite(NULL)
    , m_pCtk(ctk)
    , m_logNum(0)
{
    ui.setupUi(this);
    InitUI();
}

MainWindow::~MainWindow()
{
    if (m_sqlite != NULL)
    {
        delete m_sqlite;
    }

    // m_pClearThread->quit();
}

void MainWindow::InitUI()
{
    QSplitter  *hSplit = new QSplitter(Qt::Horizontal);
    hSplit->addWidget(ui.listWidget);
    hSplit->addWidget(ui.tableWidget);
    hSplit->setStretchFactor(0, 1);
    hSplit->setStretchFactor(1, 2);
    setCentralWidget(hSplit);

    ui.toolBar->setIconSize(QSize(55, 55));
    m_pOpenSrcAct = new QAction(QIcon(":/openSrc.png"), QStringLiteral("打开源"), this);
    m_pOpenDstAct = new QAction(QIcon(":/openDst.png"), QStringLiteral("打开目标"), this);
    //m_menuOpenDst = new QMenu(this);
    //m_tbtnOpenDst = new QToolButton(this);
    //m_tbtnOpenDst->setIcon(QIcon(":/openDst.png"));
    //m_tbtnOpenDst->setMenu(m_menuOpenDst);
    //m_tbtnOpenDst->setPopupMode(QToolButton::MenuButtonPopup);
    m_pExecuteNowAct = new QAction(QIcon(":/doNow.png"), QStringLiteral("立刻执行"), this);
    m_pEnableAct = new QAction(QIcon(":/stop.png"), QStringLiteral("停止"), this);
    m_pDeletAct = new QAction(QIcon(":/delete.png"), QStringLiteral("删除"), this);
    m_pEditAct = new QAction(QIcon(":/property.png"), QStringLiteral("属性"), this);
    m_pAddAct = new QAction(QIcon(":/add.png"), QStringLiteral("新增"), this);
    m_pUserInfoAct = new QAction(QIcon(":/allUser.png"), QStringLiteral("分发用户"), this);

    m_pClearAct = new QAction(QIcon(":/clear.png"), QStringLiteral("清屏"), this);
    m_pDataClean = new QAction(QIcon(":/dataclean.png"), QStringLiteral("清理"), this);
    m_pTrayAct = new QAction(QIcon(":/back.png"), QStringLiteral("后台"), this);
    m_pSet = new QAction(QIcon(":/set.png"), QStringLiteral("设置"), this);

    m_pBroadCastTest = new QAction(QIcon(":/broadcast.png"), QStringLiteral("广播测试"), this);
    m_pHelp = new QAction(QIcon(":/help.png"), QStringLiteral("帮助"), this);

    ui.toolBar->addAction(m_pOpenSrcAct);
    ui.toolBar->addAction(m_pOpenDstAct);
    //ui.toolBar->addWidget(m_tbtnOpenDst);
    ui.toolBar->addAction(m_pExecuteNowAct);
    ui.toolBar->addAction(m_pEnableAct);
    ui.toolBar->addAction(m_pDeletAct);
    ui.toolBar->addAction(m_pEditAct);
    ui.toolBar->addSeparator();
    ui.toolBar->addAction(m_pAddAct);
    ui.toolBar->addSeparator();
    ui.toolBar->addAction(m_pUserInfoAct);
    ui.toolBar->addSeparator();
    ui.toolBar->addAction(m_pDataClean);
    ui.toolBar->addSeparator();
    ui.toolBar->addAction(m_pClearAct);
    ui.toolBar->addAction(m_pBroadCastTest);
    ui.toolBar->addAction(m_pTrayAct);
    ui.toolBar->addSeparator();
    ui.toolBar->addAction(m_pSet);
    ui.toolBar->addAction(m_pHelp);

    ui.tableWidget->setColumnWidth(0, 110);
    ui.tableWidget->setColumnWidth(1, 140);
    ui.tableWidget->horizontalHeader()->setStretchLastSection(true);

    // 绑定信号槽
    connect(m_pAddAct, SIGNAL(triggered()), this, SLOT(addCollectSet()));
    connect(m_pUserInfoAct, SIGNAL(triggered()), this, SLOT(manageUser()));
    connect(m_pOpenSrcAct, SIGNAL(triggered()), this, SLOT(openSrcPath()));
    connect(m_pOpenDstAct, SIGNAL(triggered()), this, SLOT(openDstPath()));
    connect(m_pExecuteNowAct, SIGNAL(triggered()), this, SLOT(doNow()));
    connect(m_pEnableAct, SIGNAL(triggered()), this, SLOT(startOrstop()));
    connect(m_pDeletAct, SIGNAL(triggered()), this, SLOT(deltTask()));
    connect(m_pEditAct, SIGNAL(triggered()), this, SLOT(onProperty()));
    connect(m_pClearAct, SIGNAL(triggered()), this, SLOT(onClearLog()));
    connect(m_pDataClean, SIGNAL(triggered()), this, SLOT(onDataClean()));
    connect(m_pTrayAct, SIGNAL(triggered()), this, SLOT(hideWin()));
    connect(m_pSet, SIGNAL(triggered()), this, SLOT(onSystemSet()));
    connect(m_pBroadCastTest, SIGNAL(triggered()), this, SLOT(onBroadcastTest()));
    connect(m_pHelp, SIGNAL(triggered()), this, SLOT(onHelp()));
    connect(ui.listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(onSelectTask(int)));

    // 初始化状态栏
    m_pRunTime = new QLabel(QStringLiteral("运行时长："));
    m_pSysTime = new QLabel(QStringLiteral("系统时间："));
    m_pSysTime->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui.statusBar->setStyleSheet(QString("QStatusBar::item{border:0px}"));
    ui.statusBar->setSizeGripEnabled(false);
    ui.statusBar->addWidget(m_pRunTime);
    ui.statusBar->addWidget(new QLabel(" "), 1);
    ui.statusBar->addWidget(m_pSysTime);
    // 系统托盘
    m_pSysTrayicon = QSharedPointer<QSystemTrayIcon>(new QSystemTrayIcon(this));
    QIcon sysIcon(":/sysTrayicon.png");
    m_pSysTrayicon->setIcon(sysIcon);
    m_pSysTrayicon->setToolTip(this->windowTitle());
    connect(m_pSysTrayicon.data(), SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(showWin(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::initialize()
{
    QObject *pObj = NULL;
    pObj = m_pCtk->getService("IDataCollection");
    m_ilogsize = 0;
    if (NULL != pObj)
    {
        m_pCollect = (qobject_cast<IDataCollection*>(pObj))->createCollector(m_oConditon, m_oLocker, m_ilogsize);
        // 绑定信号...
        //connect(m_pCollect.data(), SIGNAL(showLog(const QString &, const QString &)), this, SLOT(addLog(const QString &, const QString &)));
        connect(m_pCollect.data(), SIGNAL(showLog(const CollectTask &, const QString &, int)), this, SLOT(addLog(const CollectTask &, const QString &, int)));
        connect(m_pCollect.data(), SIGNAL(taskState(const CollectTask&, int, int)), this, SLOT(setTaskIcon(const CollectTask&, int, int)));
        connect(m_pCollect.data(), SIGNAL(startGif(const QString&, bool)), this, SLOT(setGif(const QString&, bool)));
        connect(this, SIGNAL(doCollectWork(const QString&)), m_pCollect.data(), SLOT(onDoWork(const QString&)));
        connect(this, SIGNAL(enable(const QString&, bool)), m_pCollect.data(), SLOT(onEnable(const QString&, bool)));
    }
    else
    {
        QSLOG_ERROR(QString::fromLocal8Bit("获取数据收集插件失败"));
        return;
    }

    pObj = m_pCtk->getService("IDataClear");
    if (NULL != pObj)
    {
        m_pDataClear = qobject_cast<IDataClear *>(pObj);
        if (NULL == m_pDataClear)
        {
            QSLOG_ERROR(QString::fromLocal8Bit("获取数据清理插件失败"));
            return;
        }
        //m_pClearThread = QSharedPointer<QThread>(new QThread());
        //m_pClearThread->start();
        connect(this, SIGNAL(doClearWork(const QString &)), m_pDataClear, SLOT(doNow(const QString &)));
        connect(m_pDataClear, SIGNAL(taskBegin(const QString &)), this, SLOT(onClearTaskBegin(const QString &)));
        connect(m_pDataClear, SIGNAL(taskEnd(const QString &)), this, SLOT(onClearTaskEnd(const QString &)));
    }
    else
    {
        QSLOG_ERROR(QString::fromLocal8Bit("获取数据清理插件失败"));
        return;
    }

    // 注册信号参数
    qRegisterMetaType<CollectTask>("CollectTask");
    qRegisterMetaType<QSet<QString> >("QSet<QString>");
    qRegisterMetaType<string>("string");
    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");

    // 启动收集任务
    startCollect();

    // 启动清理任务
    startClear();
    ui.listWidget->setCurrentRow(0);

    m_runTime = QDateTime::currentDateTime();

    QTimer::singleShot(1000, this, SLOT(refreshStatus()));
}

void MainWindow::addCollectSet()
{
    m_pCollectDlg = QSharedPointer<CollectSetDlg>(new CollectSetDlg());
    connect(m_pCollectDlg.data(), SIGNAL(commit(CollectTask &)), this, SLOT(addCollect(CollectTask &)));
    m_pCollectDlg->exec();
}

void MainWindow::manageUser()
{
    m_pUserManageDlg = QSharedPointer<UserManageDlg>(new UserManageDlg());
    m_pUserManageDlg->exec();
}

bool MainWindow::startCollect()
{
    m_sqlite = DataBase::getInstance();

    QList<CollectTask> collectTasks;
    m_sqlite->QueryCollectTask(collectTasks);
    for (int i=0; i<collectTasks.size(); ++i)
    {
        addCollect(collectTasks[i], false);
    }

    //ui.listWidget->setCurrentRow(0);

    return true;
}

bool MainWindow::startClear()
{
    m_sqlite = DataBase::getInstance();

    QList<ClearTask> clearTasks;
    m_sqlite->queryClearTask(clearTasks);
    for (int i=0; i<clearTasks.size(); ++i)
    {
        addClear(clearTasks.at(i), false);
    }

    //ui.listWidget->setCurrentRow(0);

    return true;
}

bool MainWindow::addCollect(CollectTask &task, bool bDb /*= true*/)
{
    TaskUser tUser;
    tUser.taskID = task.dirID;
    m_sqlite->QueryUserInfo(tUser);

    int iSendType = tUser.lstUser.at(0).user.sendType >= 1 ? 1 : 0;
    int iCollectType = task.collectType >= 1 ? 1: 0;
    int nIcon = iCollectType + iSendType * 2 + task.enable * 4;
    // 在左侧列表显示
    QListWidgetItem *pItem = new QListWidgetItem(COLLECTITEM);
    ui.listWidget->addItem(pItem);
    MyItemWidget *itemWidget = new MyItemWidget(ICONTYPE(nIcon), task.dirName);
    itemWidget->show();
    ui.listWidget->setItemWidget(pItem, itemWidget);
    pItem->setSizeHint(QSize(itemWidget->rect().width(), itemWidget->rect().height()));

    if (0 == task.collectType)
    {
        //QUrl oUrl(task.rltvPath.replace("\\", "/"));
        task.rltvPath = task.rltvPath.replace("\\", "/");
        //qDebug() << oUrl.path();
    }

    // 添加到数据库
    if (bDb && !DataBase::getInstance()->InsertCollectTask(task))
    {
        return false;
    }
    // 记录到内存
    m_ItemTask[pItem] = task.dirID;
    // 启动收集
    m_pCollect->addSyncTransfer(task);

    return true;
}

bool MainWindow::addClear(const ClearTask &task, bool bDb)
{
    //TaskUser tUser;
    //tUser.taskID = task.dirID;
    //m_sqlite->QueryUserInfo(tUser);

    //int nIcon = task.collectType + /*cUser.user.sendType * 2 +*/ task.enable * 4;
    // 在左侧列表显示
    QListWidgetItem *pItem = new QListWidgetItem(CLEARITEM);
    ui.listWidget->addItem(pItem);
    ClearItemWidget *itemWidget = new ClearItemWidget(task.taskName);
    itemWidget->show();
    ui.listWidget->setItemWidget(pItem, itemWidget);
    pItem->setSizeHint(QSize(itemWidget->rect().width(), itemWidget->rect().height()));
    // 添加到数据库
    if (bDb && !DataBase::getInstance()->insertClearTask(task))
    {
        return false;
    }
    // 记录到内存
    m_ItemTask[pItem] = task.taskName;
    // 启动收集
    //m_pCollect->addSyncTransfer(task);
    if (task.activeFlag)
    {
        BaseDatas data;
        data.m_taskName = task.taskName;
        data.m_DDrule = task.quartzRule;
        data.m_fullPath = task.taskDir;
        data.m_regex.append(task.matchRule);
        data.m_time = task.computeSecond();
        data.m_style = "0";	// 按照时间规则进行清理
        data.taskType = task.taskType;
        data.ip = task.ip;
        data.port = task.port;
        data.user = task.user;
        data.password = task.password;
        data.transfermode = task.transfermode;
        m_pDataClear->start(data);
    }
    else
    {
        //pItem->setDisabled(false);
        itemWidget->SetEnable(false);
    }
    return true;
}

void MainWindow::addLog(const QString &dirName, const QString &info, int infoType)
{
    if (m_logNum >= 1000)
    {
        // 清空tablewidget的内容
        ui.tableWidget->clearContents();
        ui.tableWidget->setRowCount(0);
        m_logNum = 0;
    }

    m_logNum++;
    ui.tableWidget->insertRow(0);

    QString strTime = QDateTime::currentDateTime().toString("MM-dd hh:mm:ss");
#ifdef _WIN32
    QTableWidgetItem *pItem = new QTableWidgetItem(QIcon(":/02.png"), strTime);
#else
    QTableWidgetItem *pItem = new QTableWidgetItem(strTime);
#endif
    ui.tableWidget->setItem(0, 0, pItem);
    pItem = new QTableWidgetItem(dirName);
    ui.tableWidget->setItem(0, 1, pItem);
    pItem = new QTableWidgetItem(info);
    ui.tableWidget->setItem(0, 2, pItem);
}

void MainWindow::addLog(const CollectTask &task, const QString &info, int infoType)
{
    ///*	QTime tt;
    //	tt.start()*/;
    if (m_logNum >= 500)
    {
        // 清空tablewidget的内容
        ui.tableWidget->setRowCount(0);
        ui.tableWidget->clearContents();

        //for(int i = ui.tableWidget->rowCount();i > 0;i--)
        //{
        //	ui.tableWidget->removeRow(0);
        //}

        m_logNum = 0;
    }

    m_logNum++;
    ui.tableWidget->insertRow(0);
    QMutexLocker guard(&m_oLocker2);
    QDateTime qDT = QDateTime::currentDateTime();
    QString strTime = qDT.toString("MM-dd hh:mm:ss");
    QTableWidgetItem *pItem = NULL;
#ifdef _WIN32
    if (1 == infoType)
    {
        pItem = new QTableWidgetItem(QIcon(":/01.png"), strTime);
    }
    else if (2 == infoType)
    {
        pItem = new QTableWidgetItem(QIcon(":/03.png"), strTime);
    }
#else
    pItem = new QTableWidgetItem(strTime);
#endif

    ui.tableWidget->setItem(0, 0, pItem);
    pItem = new QTableWidgetItem(task.dirName);
    ui.tableWidget->setItem(0, 1, pItem);
    pItem = new QTableWidgetItem(info);
    ui.tableWidget->setItem(0, 2, pItem);

    if (infoType == 0)
    {
        // 系统消息
    }
    else if (infoType == 1)
    {
        // 发送成功
        ClearItemWidget *pIwidget = static_cast<ClearItemWidget*>(getItemWidget(task.dirID));
        if (pIwidget != NULL)
        {
            pIwidget->addSuccess(qDT);
        }
    }
    else
    {
        // 发送失败
    }
    ///*QSLOG_INFO(QString("******%1******").arg(tt.elapsed()));*/

    ////m_oLocker.lock();

    ////if (m_ilogsize <= 100)
    ////{
    ////	m_oConditon.wakeAll();
    ////}
    ////m_oLocker.unlock();
    ////m_ilogsize--;
}

//void MainWindow::print(const string &dirName, const string &dirId, const string &info, int infoType)
void MainWindow::printCollectLog(const QString &dirName, const QString &dirId, const QString &info, int infoType)
{
    //QSLOG_DEBUG("PRINT2");
    if (m_logNum >= 500)
    {
        // 清空tablewidget的内容
        ui.tableWidget->setRowCount(0);
        ui.tableWidget->clearContents();

        m_logNum = 0;
    }

    m_logNum++;
    ui.tableWidget->insertRow(0);
    QMutexLocker guard(&m_oLocker2);
    QDateTime qDT = QDateTime::currentDateTime();
    QString strTime = qDT.toString("MM-dd hh:mm:ss");
    QTableWidgetItem *pItem = NULL;
#ifdef _WIN32
    if (1 == infoType)
    {
        pItem = new QTableWidgetItem(QIcon(":/01.png"), strTime);
    }
    else if (2 == infoType)
    {
        pItem = new QTableWidgetItem(QIcon(":/03.png"), strTime);
    }
#else
    pItem = new QTableWidgetItem(strTime);
#endif
    ui.tableWidget->setItem(0, 0, pItem);
    //pItem = new QTableWidgetItem(QString::fromLocal8Bit(dirName.c_str()));
    pItem = new QTableWidgetItem(dirName);
    ui.tableWidget->setItem(0, 1, pItem);
    //pItem = new QTableWidgetItem(QString::fromLocal8Bit(info.c_str()));
    pItem = new QTableWidgetItem(info);
    ui.tableWidget->setItem(0, 2, pItem);

    if (infoType == 0)
    {
        // 系统消息
    }
    else if (infoType == 1)
    {
        // 发送成功
        // MyItemWidget *pIwidget = getItemWidget(QString::fromLocal8Bit(dirId.c_str()));
        MyItemWidget *pIwidget = static_cast<MyItemWidget*>(getItemWidget(dirId));
        if (pIwidget != NULL)
        {
            pIwidget->AddSuccess(qDT);
        }
    }
    else
    {
        // 发送失败
    }
}

void MainWindow::printClearLog(const QString &dirName, const QString &dirId, const QString &info, int infoType)
{
    if (m_logNum >= 500)
    {
        // 清空tablewidget的内容
        ui.tableWidget->setRowCount(0);
        ui.tableWidget->clearContents();

        m_logNum = 0;
    }

    m_logNum++;
    ui.tableWidget->insertRow(0);
    QMutexLocker guard(&m_oLocker2);
    QDateTime qDT = QDateTime::currentDateTime();
    QString strTime = qDT.toString("MM-dd hh:mm:ss");
    QTableWidgetItem *pItem = NULL;
#ifdef _WIN32
    if (1 == infoType)
    {
        pItem = new QTableWidgetItem(QIcon(":/01.png"), strTime);
    }
    else if (2 == infoType)
    {
        pItem = new QTableWidgetItem(QIcon(":/03.png"), strTime);
    }
#else
    pItem = new QTableWidgetItem(strTime);
#endif

    ui.tableWidget->setItem(0, 0, pItem);

    pItem = new QTableWidgetItem(dirName);
    ui.tableWidget->setItem(0, 1, pItem);

    pItem = new QTableWidgetItem(info);
    ui.tableWidget->setItem(0, 2, pItem);

    if (infoType == 0)
    {
        // 系统消息
    }
    else if (infoType == 1)
    {
        // 发送成功
        ClearItemWidget *pIwidget = (ClearItemWidget *)getItemWidget(dirName);
        if (pIwidget != NULL)
        {
            pIwidget->addSuccess(qDT);
        }
    }
    else
    {
        // 发送失败
    }
}


void MainWindow::openSrcPath()
{
    int nIndex = ui.listWidget->currentRow();
    if (nIndex < 0)
    {
        return;
    }
    QListWidgetItem *pItem = ui.listWidget->currentItem();
    QMap<QListWidgetItem*, QString>::const_iterator it = m_ItemTask.find(pItem);
    if (it == m_ItemTask.end())
    {
        return;
    }
    if (m_sqlite == NULL)
    {
        m_sqlite = DataBase::getInstance();
    }

    // 如果是收集任务
    if (it.key()->text() == COLLECTITEM)
    {
        CollectTask task;
        task.dirID = m_ItemTask[pItem];
        if (!m_sqlite->QueryCollectTask(task))
        {
            return;
        }

        QString urlPath;
        if (task.collectType == 0)
        {

            urlPath = CPathBuilder::getFinalPathFromUrl(task.rltvPath).at(0);
            urlPath = QUrl::fromLocalFile(urlPath).toString();
        }
        else if (task.collectType == 1)
        {
            urlPath = QString("ftp://%1:%2@%3:%4%5").arg(task.loginUser).arg(task.loginPass).arg(task.ip)
                      .arg(task.port).arg(CPathBuilder::getFinalPathFromUrl(task.rltvPath).at(0));
        }
        else
        {
            urlPath = QString("sftp://%1:%2@%3:%4%5").arg(task.loginUser).arg(task.loginPass).arg(task.ip)
                      .arg(task.port).arg(CPathBuilder::getFinalPathFromUrl(task.rltvPath).at(0));
        }

        // 另一种方式（windows）
        //     QString path = QDir::currentPath();
        //     path.replace("/", "\\");
        //		stt = "ftp://nriet:123456@192.168.21.28:21/";
        //     QProcess::startDetached("explorer " + path);

        QDesktopServices::openUrl(QUrl(urlPath, QUrl::TolerantMode));
    }
    else
    {
        // 如果是清理任务
        ClearTask task;
        task.taskName = m_ItemTask[pItem];
        if (!m_sqlite->queryClearTask(task))
        {
            return;
        }

        QString urlPath = task.taskDir;
        urlPath = QUrl::fromLocalFile(urlPath).toString();
        QDesktopServices::openUrl(QUrl(urlPath, QUrl::TolerantMode));

    }

}

void MainWindow::openDstPath()
{
    int nIndex = ui.listWidget->currentRow();
    if (nIndex < 0)
    {
        QSLOG_ERROR("nIndex < 0");
        return;
    }
    QListWidgetItem *pItem = ui.listWidget->currentItem();
    QMap<QListWidgetItem*, QString>::const_iterator it = m_ItemTask.find(pItem);
    if (it == m_ItemTask.end())
    {
        QSLOG_ERROR("TASK NOT FOUND");
        return;
    }
    if (m_sqlite == NULL)
    {
        m_sqlite = DataBase::getInstance();
    }
    // 如果是收集任务
    if (it.key()->text() == CLEARITEM)
    {
        return;
    }

    //return;	// 临时

    // 根据任务ID查找用户信息
    CollectUser cUser;
    //cUser.user.
    // cUser.dirID = m_ItemTask[pItem];
    QString taskId = m_ItemTask[pItem];
    TaskUser oTaskUser;
    oTaskUser.taskID = taskId;
    if (!m_sqlite->QueryUserInfo(oTaskUser))
    {
        return;
    }

    cUser = oTaskUser.lstUser.at(0);

    QString urlPath;
    if (cUser.user.sendType == 0)
    {
        QStringList lstPaths = CPathBuilder::getFinalPathFromUrl(cUser.user.rootPath + cUser.rltvPath);
        urlPath = lstPaths.at(0);
        urlPath = QUrl::fromLocalFile(urlPath).toString();
    }
    else if (cUser.user.sendType == 1)
    {
        urlPath = QStringLiteral("ftp://%1:%2@%3:%4%5").arg(cUser.user.lgUser).arg(cUser.user.lgPass).arg(cUser.user.ip)
                  .arg(cUser.user.port).arg(CPathBuilder::getFinalPathFromUrl(cUser.user.rootPath + cUser.rltvPath).at(0));
    }
    else
    {
        urlPath = QStringLiteral("sftp://%1:%2@%3:%4%5").arg(cUser.user.lgUser).arg(cUser.user.lgPass).arg(cUser.user.ip)
                  .arg(cUser.user.port).arg(CPathBuilder::getFinalPathFromUrl(cUser.user.rootPath + cUser.rltvPath).at(0));
    }

    QDesktopServices::openUrl(QUrl(urlPath));
}

void MainWindow::doNow()
{
    int nIndex = ui.listWidget->currentRow();
    if (nIndex < 0)
    {
        return;
    }
    QListWidgetItem *pItem = ui.listWidget->currentItem();
    QMap<QListWidgetItem*, QString>::const_iterator it = m_ItemTask.find(pItem);
    if (it == m_ItemTask.end())
    {
        return;
    }
    if (m_sqlite == NULL)
    {
        return;
    }
    if (it.key()->text() == COLLECTITEM)
    {
        emit doCollectWork(m_ItemTask[pItem]);
    }
    else
    {
        ClearTask task;
        task.taskName = m_ItemTask[pItem];
        if (DataBase::getInstance()->queryClearTask(task))
        {
            // emit doClearWork(task.taskDir);
            emit doClearWork(task.taskName);
        }
    }
}

void MainWindow::onSelectTask(int index)
{
    if (index < 0)
    {
        return;
    }
    QListWidgetItem *pItem = ui.listWidget->currentItem();
    QMap<QListWidgetItem*, QString>::const_iterator it = m_ItemTask.find(pItem);
    if (it == m_ItemTask.end())
    {
        return;
    }
    QString strIndenty = it.key()->text();


    // 查询收集任务信息
    if (m_sqlite == NULL)
    {
        return;
    }

    // 当前选中的是收集任务
    if (strIndenty == COLLECTITEM)
    {
        CollectTask task;
        task.dirID = m_ItemTask[pItem];
        if (!m_sqlite->QueryCollectTask(task))
        {
            return;
        }
        QString logInfo;
        // 更改图片状态
        if (task.enable == 1)
        {
            m_pEnableAct->setIcon(QIcon(":/stop.png"));
        }
        else
        {
            m_pEnableAct->setIcon(QIcon(":/start.png"));
        }
        // 打开目标路径
        //m_menuOpenDst->clear();
        //m_tUser.taskID = task.dirID;
        //m_tUser.lstUser.clear();
        //if (m_sqlite->QueryUserInfo(m_tUser))
        //{
        //    m_grpAction = QSharedPointer<QActionGroup>(new QActionGroup(this));
        //    for (int i=0; i<m_tUser.lstUser.size(); ++i)
        //    {
        //        QAction *pAct = new QAction(m_tUser.lstUser.at(i).user.userName, this);
        //        //m_menuOpenDst->addAction(pAct);
        //        QVariant var;
        //        var.setValue(m_tUser.lstUser[i]);
        //        pAct->setData(var);
        //        m_grpAction->addAction(pAct);
        //    }
        //    connect(m_grpAction.data(), SIGNAL(triggered(QAction*)), this, SLOT(onUserAction(QAction*)));
        //}
    }
    else	// 当前选择的是清理任务
    {
        ClearTask task;
        task.taskName = m_ItemTask[pItem];
        if (!m_sqlite->queryClearTask(task))
        {
            return;
        }

        QString logInfo;
        // 更改图片状态
        if (task.activeFlag == 1)
        {
            m_pEnableAct->setIcon(QIcon(":/stop.png"));
        }
        else
        {
            m_pEnableAct->setIcon(QIcon(":/start.png"));
        }
        // 打开目标路径
        // m_menuOpenDst->setEnabled(false);
        //m_menuOpenDst->clear();

    }
}

void MainWindow::deltTask()
{
    QListWidgetItem *pItem = NULL;
    QString dirID = getCurTaskID(pItem);
    if (dirID.isEmpty() || pItem ==NULL)
    {
        return;
    }

    if (pItem->text() == COLLECTITEM)
    {
        // 从数据库中删除
        m_sqlite->DeltCollectTask(dirID);
        m_ItemTask.remove(pItem);
        // 停止收集
        m_pCollect->deltSyncTransfer(dirID);
        // 从界面删除
        MyItemWidget *itemWidget = (MyItemWidget *)ui.listWidget->itemWidget(pItem);
        itemWidget->close();
        delete itemWidget;
        ui.listWidget->removeItemWidget(pItem);
        //delete itemWidget;
        ui.listWidget->takeItem(ui.listWidget->currentRow());
    }
    else
    {
        ClearTask task;
        task.taskName = dirID;
        m_sqlite->queryClearTask(task);
        // 从数据库中删除
        m_sqlite->deleteClearTask(dirID);
        m_ItemTask.remove(pItem);
        // 停止收集
        // m_pCollect->deltSyncTransfer(dirID);

        BaseDatas data;
        data.m_taskName = task.taskName;
        data.m_fullPath = task.taskDir;
        m_pDataClear->stop(data);
        // 从界面删除
        ClearItemWidget *itemWidget = (ClearItemWidget *)ui.listWidget->itemWidget(pItem);
        itemWidget->close();
        delete itemWidget;
        ui.listWidget->removeItemWidget(pItem);
        //delete itemWidget;
        ui.listWidget->takeItem(ui.listWidget->currentRow());
    }
}

QString MainWindow::getCurTaskID(QListWidgetItem *&pItem)
{
    QString res;

    pItem = ui.listWidget->currentItem();
    QMap<QListWidgetItem*, QString>::const_iterator it = m_ItemTask.find(pItem);
    if (it == m_ItemTask.end())
    {
        pItem = NULL;
        return res;
    }

    res = m_ItemTask[pItem];
    return res;
}

void MainWindow::startOrstop()
{
    QListWidgetItem *pItem = NULL;
    getCurTaskID(pItem);
    if (pItem == NULL)
    {
        return;
    }

    // 如果是收集任务
    if (pItem->text() == COLLECTITEM)
    {
        CollectTask task;
        task.dirID = m_ItemTask[pItem];
        if (!m_sqlite->QueryCollectTask(task))
        {
            return;
        }
        QString logInfo;
        bool bFlag;
        // 更改图片状态
        if (task.enable == 1)
        {
            task.enable = 0;
            m_pEnableAct->setIcon(QIcon(":/start.png"));
            logInfo = QStringLiteral("已停止。");
            bFlag = false;
        }
        else
        {
            task.enable = 1;
            m_pEnableAct->setIcon(QIcon(":/stop.png"));
            logInfo = QStringLiteral("已启动。");
            bFlag = true;
        }
        //((MyItemWidget *)(ui.listWidget->itemWidget(pItem)))->setEnabled(bFlag);    // 可以直接灰化
        ((MyItemWidget *)(ui.listWidget->itemWidget(pItem)))->SetEnable(bFlag);
        // 添加日志
        addLog(task.dirName, logInfo);
        // 通知收集
        emit enable(task.dirID, bFlag);
        // 更新数据库
        bFlag = m_sqlite->InsertCollectTask(task);
    }
    else
    {
        // 如果是清理任务
        ClearTask task;
        task.taskName = m_ItemTask[pItem];
        if (!m_sqlite->queryClearTask(task))
        {
            return;
        }

        QString logInfo;
        bool bFlag;
        // 更改图片状态
        if (task.activeFlag == 1)
        {
            task.activeFlag = 0;
            m_pEnableAct->setIcon(QIcon(":/start.png"));
            logInfo = QStringLiteral("已停止。");
            bFlag = false;
            BaseDatas data;
            data.m_taskName = task.taskName;
            data.m_fullPath = task.taskDir;
            m_pDataClear->stop(data);
        }
        else
        {
            task.activeFlag = 1;
            m_pEnableAct->setIcon(QIcon(":/stop.png"));
            logInfo = QStringLiteral("已启动。");
            bFlag = true;
            BaseDatas data;
            data.m_taskName = task.taskName;
            data.m_DDrule = task.quartzRule;
            data.m_fullPath = task.taskDir;
            data.m_regex.append(task.matchRule);
            data.m_time = task.computeSecond();
            data.m_style = "0";	// 按照时间规则进行清理
            data.taskType = task.taskType;
            data.ip = task.ip;
            data.port = task.port;
            data.user = task.user;
            data.password = task.password;
            data.transfermode = task.transfermode;
            m_pDataClear->start(data);
        }
        //((MyItemWidget *)(ui.listWidget->itemWidget(pItem)))->setEnabled(bFlag);    // 可以直接灰化
        ((ClearItemWidget *)(ui.listWidget->itemWidget(pItem)))->SetEnable(bFlag);
        // 添加日志
        //addLog(task.dirName, logInfo);
        // 通知收集
        //emit enable(task.dirID, bFlag);
        // 更新数据库
        bFlag = m_sqlite->insertClearTask(task);
    }
}

void MainWindow::onProperty()
{
    QListWidgetItem *pItem = NULL;
    getCurTaskID(pItem);
    if (pItem == NULL)
    {
        return;
    }

    // 如果是收集任务
    if (pItem->text() == COLLECTITEM)
    {
        CollectTask task;
        task.dirID = m_ItemTask[pItem];
        if (!m_sqlite->QueryCollectTask(task))
        {
            return;
        }

        m_pCollectDlg = QSharedPointer<CollectSetDlg>(new CollectSetDlg(1));
        m_pCollectDlg->showTask(task);
        QString strOldSenderUser = m_pCollectDlg->getSendUserNameFromDirID(task.dirID);
        if (QDialog::Accepted == m_pCollectDlg->exec())
        {
            bool bRestart = false;
            CollectUser oSender = m_pCollectDlg->getSendUserInfoFromDirID(task.dirID);
            if (task.collectType != m_pCollectDlg->m_task.collectType ||
                    task.dispatch != m_pCollectDlg->m_task.dispatch ||
                    task.rltvPath != m_pCollectDlg->m_task.rltvPath ||
                    task.col_timerange != m_pCollectDlg->m_task.col_timerange ||
                    task.recordLatestTime != m_pCollectDlg->m_task.recordLatestTime ||
                    task.ftp_transferMode != m_pCollectDlg->m_task.ftp_transferMode ||
                    task.ftp_connectMode != m_pCollectDlg->m_task.ftp_connectMode ||
                    strOldSenderUser != oSender.user.userName ||
                    task.compareContent != m_pCollectDlg->m_task.compareContent ||
                    task.subDirTemplate != m_pCollectDlg->m_task.subDirTemplate ||
                    task.fileTemplate != m_pCollectDlg->m_task.fileTemplate ||
                    task.ip != m_pCollectDlg->m_task.ip ||
                    task.port != m_pCollectDlg->m_task.port ||
                    task.loginUser != m_pCollectDlg->m_task.loginUser ||
                    task.loginPass != m_pCollectDlg->m_task.loginPass)
            {
                bRestart = true;
            }

            if (!bRestart)
            {
                return;
            }
            task = m_pCollectDlg->m_task;
            // 设置列表图标状态
            int iSendType = oSender.user.sendType >= 1 ? 1 : 0;
            int iCollectType = m_pCollectDlg->m_task.collectType >= 1 ? 1 : 0;
            int nIcon = iCollectType + iSendType * 2 + task.enable * 4;
            MyItemWidget *pItemWidget = (MyItemWidget *)ui.listWidget->itemWidget(pItem);
            pItemWidget->SetIcon((ICONTYPE)nIcon);
            pItemWidget->SetName(task.dirName);

            if (0 == task.collectType)
            {
                //QUrl oUrl(task.rltvPath.replace("\\", "/"));
                task.rltvPath = task.rltvPath.replace("\\", "/");
            }
            // 记录到数据库
            m_sqlite->InsertCollectTask(task);
            // 修改收集设置
            m_pCollect->mdfySyncTransfer(task);
        }
    }
    else
    {
        // 是清理任务
        ClearTask task;
        task.taskName = m_ItemTask[pItem];
        if (!m_sqlite->queryClearTask(task))
        {
            return;
        }

        m_pClearDlg = QSharedPointer<DataClearDlg>(new DataClearDlg(task.taskName));
        if (QDialog::Accepted == m_pClearDlg->exec())
        {
            bool bRestart = false;
            if (task.activeFlag != m_pClearDlg->m_task.activeFlag ||
                    task.clearUnit != m_pClearDlg->m_task.clearUnit ||
                    task.clearValue != m_pClearDlg->m_task.clearValue ||
                    task.matchRule != m_pClearDlg->m_task.matchRule ||
                    task.quartzRule != m_pClearDlg->m_task.quartzRule ||
                    task.taskDir != m_pClearDlg->m_task.taskDir ||
                    task.taskName != m_pClearDlg->m_task.taskName ||
                    task.taskType != m_pClearDlg->m_task.taskType ||
                    task.ip != m_pClearDlg->m_task.ip ||
                    task.port != m_pClearDlg->m_task.port ||
                    task.user != m_pClearDlg->m_task.user ||
                    task.password != m_pClearDlg->m_task.password ||
                    task.transfermode != m_pClearDlg->m_task.transfermode)
            {
                bRestart = true;

            }
            task = m_pClearDlg->m_task;

            ClearItemWidget *pItemWidget = (ClearItemWidget *)ui.listWidget->itemWidget(pItem);
            pItemWidget->setName(task.taskName);
            // 记录到数据库
            m_sqlite->insertClearTask(task);
            //// 修改收集设置
            //m_pCollect->mdfySyncTransfer(task);
            BaseDatas data;
            data.m_taskName = task.taskName;
            data.m_DDrule = task.quartzRule;
            data.m_fullPath = task.taskDir;
            data.m_regex.append(task.matchRule);
            data.m_time = task.computeSecond();
            data.m_style = "0";	// 按照时间规则进行清理
            data.taskType = task.taskType;
            data.ip = task.ip;
            data.port = task.port;
            data.user = task.user;
            data.password = task.password;
            data.transfermode = task.transfermode;
            m_pDataClear->stop(data);
            m_pDataClear->start(data);
        }
    }
}

void MainWindow::onDataClean()
{
    m_pClearDlg = QSharedPointer<DataClearDlg>(new DataClearDlg());
    connect(m_pClearDlg.data(), SIGNAL(newTaskCreated(const ClearTask &)), this, SLOT(onNewClearTaskCreated(const ClearTask &)));
    m_pClearDlg->exec();
}

void MainWindow::onClearLog()
{
    ui.tableWidget->clearContents();
    ui.tableWidget->setRowCount(0);
    m_logNum = 0;
}

void MainWindow::setTaskIcon(const CollectTask &task, int sendWay, int normal)
{
    TaskUser tUser;
    tUser.taskID = task.dirID;
    m_sqlite->QueryUserInfo(tUser);
    int iSendType = tUser.lstUser.at(0).user.sendType >= 1 ? 1 : 0;
    int iCollectType = task.collectType + iSendType >= 1 ? 1 : 0;
    int nIcon = iCollectType + iSendType * 2 + task.enable * 4 + normal * 8;
    QMap<QListWidgetItem*, QString>::const_iterator it = m_ItemTask.begin();
    while (it != m_ItemTask.end())
    {
        if (it.value() == task.dirID)
        {
            ((MyItemWidget *)ui.listWidget->itemWidget(it.key()))->SetIcon((ICONTYPE)nIcon);
            break;
        }
        it++;
    }
}

void MainWindow::showWin(QSystemTrayIcon::ActivationReason ar)
{
    if (ar == QSystemTrayIcon::DoubleClick)
    {
        if (!m_pSysTrayicon.isNull())
        {
            m_pSysTrayicon->hide();
        }
        showNormal();
    }
}

void MainWindow::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::WindowStateChange)
    {
        if (windowState() & Qt::WindowMinimized)
        {
            hideWin();
        }
    }
}

void MainWindow::hideWin()
{
    QTimer::singleShot(0, this, SLOT(hide()));
    if (!m_pSysTrayicon.isNull())
    {
        m_pSysTrayicon->show();
        m_pSysTrayicon->showMessage(windowTitle(), QStringLiteral("双击该图标可显示界面！"));
    }
}

void MainWindow::refreshStatus()
{
    // 获取系统时间
    QDateTime curDT = QDateTime::currentDateTime();
    QString str = curDT.toString("yyyy-MM-dd hh:mm:ss ");
    m_pSysTime->setText(str);
    // 获取运行时间
    calRunTime();
    QTimer::singleShot(1000, this, SLOT(refreshStatus()));
}

void MainWindow::calRunTime()
{
    // int tm = m_runTime.elapsed() / 1000;
    int tm = QDateTime::currentDateTime().toTime_t() - m_runTime.toTime_t();
    if (tm < 0)
    {
        QSLOG_ERROR("time is overflow.");
    }
    int day = tm / (24 * 60 * 60);
    int hour = (tm - day * (24 * 60 * 60)) / (60 * 60);
    int min = (tm - day * (24 * 60 * 60) - hour * (60 * 60)) / 60;
    int sec = tm - day * (24 * 60 * 60) - hour * (60 * 60) - min * 60;
    QString strTime;
    //if (day == 0)
    //{
    //    strTime = QStringLiteral(" 运行时长：%2:%3:%4").arg(hour).arg(min).arg(sec);
    //}
    //else
    {
        strTime = QStringLiteral(" 运行时长：%1天 %2时 %3分 %4秒").arg(day).arg(hour, 2, 10, QChar('0')).arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
    }
    m_pRunTime->setText(strTime);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    this->hide();
    m_pCollect->stopAllCollection();
    //delete this;
}

void MainWindow::setGif(const QString &dirID, bool bFlag)
{
    QMap<QListWidgetItem*, QString>::const_iterator it = m_ItemTask.begin();
    for (; it!=m_ItemTask.end(); it++)
    {
        if (it.value() == dirID)
        {
            ((MyItemWidget *)ui.listWidget->itemWidget(it.key()))->ShowGif(bFlag);
            break;
        }
    }
}

QWidget * MainWindow::getItemWidget(const QString &taskID)
{
    QWidget *pIwidget = NULL;
    QMap<QListWidgetItem*, QString>::const_iterator it = m_ItemTask.begin();
    while (it != m_ItemTask.end())
    {
        if (it.value() == taskID)
        {
            pIwidget =  ui.listWidget->itemWidget(it.key());
            break;
        }
        it++;
    }
    return pIwidget;
}

void MainWindow::onUserAction(QAction *act)
{
    QVariant var = act->data();
    CollectUser cUser = var.value<CollectUser>();

    QString urlPath;
    if (cUser.user.sendType == 0)
    {
        urlPath = cUser.user.rootPath + cUser.rltvPath;
        urlPath = QUrl::fromLocalFile(urlPath).toString();
    }
    else if (cUser.user.sendType == 1)
    {
        urlPath = QStringLiteral("ftp://%1:%2@%3:%4%5").arg(cUser.user.lgUser).arg(cUser.user.lgPass).arg(cUser.user.ip)
                  .arg(cUser.user.port).arg(cUser.user.rootPath + cUser.rltvPath);
    }
    else
    {
        urlPath = QStringLiteral("sftp://%1:%2@%3:%4%5").arg(cUser.user.lgUser).arg(cUser.user.lgPass).arg(cUser.user.ip)
                  .arg(cUser.user.port).arg(cUser.user.rootPath + cUser.rltvPath);
    }

    QDesktopServices::openUrl(QUrl(urlPath));
}

void MainWindow::onNewClearTaskCreated(const ClearTask &task)
{
    //TaskUser tUser;
    //tUser.taskID = task.dirID;
    //m_sqlite->QueryUserInfo(tUser);

    // 在左侧列表显示
    QListWidgetItem *pItem = new QListWidgetItem(CLEARITEM);
    ui.listWidget->addItem(pItem);
    ClearItemWidget *itemWidget = new ClearItemWidget(task.taskName);
    itemWidget->show();
    ui.listWidget->setItemWidget(pItem, itemWidget);
    pItem->setSizeHint(QSize(itemWidget->rect().width(), itemWidget->rect().height()));

    //// 记录到内存
    m_ItemTask[pItem] = task.taskName;
    //// 启动收集
    //m_pCollect->addSyncTransfer(task);
    BaseDatas data;
    data.m_taskName = task.taskName;
    data.m_DDrule = task.quartzRule;
    data.m_fullPath = task.taskDir;
    data.m_regex.append(task.matchRule);
    data.m_time = task.computeSecond();
    data.m_style = "0";	// 按照时间规则进行清理
    data.taskType = task.taskType;
    data.ip = task.ip;
    data.port = task.port;
    data.user = task.user;
    data.password = task.password;
    data.transfermode = task.transfermode;
    m_pDataClear->start(data);

}

void MainWindow::onClearTaskBegin(const QString &dir)
{
    // 根据路径查找item
    QString name;
    if (!DataBase::getInstance()->queryClearTask(dir, name))
    {
        return;
    }
    QMap<QListWidgetItem*, QString>::const_iterator it = m_ItemTask.begin();
    for (; it!=m_ItemTask.end(); it++)
    {
        if (it.key()->text() == CLEARITEM && it.value() == name)
        {
            ClearItemWidget * item = (ClearItemWidget *)ui.listWidget->itemWidget(it.key());
            item->ShowGif(true);
            item->addSuccess(QDateTime::currentDateTime(), false);
            break;
        }
    }
}

void MainWindow::onClearTaskEnd(const QString &dir)
{
    // 根据路径查找item
    QString name;
    if (!DataBase::getInstance()->queryClearTask(dir, name))
    {
        return;
    }
    QMap<QListWidgetItem*, QString>::const_iterator it = m_ItemTask.begin();
    for (; it!=m_ItemTask.end(); it++)
    {
        if (it.key()->text() == CLEARITEM && it.value() == name)
        {
            ((ClearItemWidget *)ui.listWidget->itemWidget(it.key()))->ShowGif(false);
            break;
        }
    }
}

void MainWindow::onSystemSet()
{
    CSystemSetDlg oSetDlg;
    oSetDlg.exec();
}

void MainWindow::onBroadcastTest()
{
    CBroadCastWnd oBroadWnd;
    oBroadWnd.exec();
}

void MainWindow::onHelp()
{
    QMessageBox::about(this, QString::fromLocal8Bit("数据传输工具"), QString::fromLocal8Bit("数据传输工具\n"
                       "版本：1.1.0\n"
                       "copyright: 雷达系统事业部系统工程中心\n"
                       "E-mail: liubojun@glarun.com"));
};
