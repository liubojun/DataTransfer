#ifndef mainwindow_H_
#define mainwindow_H_

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMenuBar>
#include <QToolButton>
#include <QSplitter>
#include <QVector>
#include <QLabel>
#include <QWaitCondition>
#include <QMutex>

#include "ui_mainwindow.h"
#include "ICtkPluginManager.h"
#include "collectSetDlg.h"
#include "userInfoDlg.h"
#include "DataBase.h"
#include "ICollectManager.h"
#include "listItemWidget.h"
#include "ILogWidget.hpp"
#include "IDataClear.h"
//#include "commontypedef.h"
class DataClearDlg;
class QThread;
// 左侧item的标识，用来区分清理与收集任务
const QString CLEARITEM = "clear";
const QString COLLECTITEM = "collect";

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief  默认构造函数
     * @param  QMainWindow * parent
     * @return ：描述返回值
     */
    MainWindow(ICtkPluginManager *_ctk,QMainWindow *parent = NULL);

    /**
     * @brief  默认析构函数
     * @return ：描述返回值
     */
    virtual ~MainWindow();

    bool startCollect();

    bool startClear();



    /**
     * @brief  完成主窗口的初始化
     * @return ：无
    */
    void initialize();

public slots:
    /**
    * @brief  通过RCF远程调用的方式打印日志
    * @param  const std::string &dirName 收集目录名
    * @param  const std::string &dirId   收集目录ID
    * @param  const std::string &info    日志信息
    * @param  const std::string &infoType日志等级
    * @return ：无
    */
    void print(const string &dirName, const string &dirId, const string &info, int infoType);
protected:
    virtual void closeEvent(QCloseEvent *);
    virtual void changeEvent(QEvent *);

signals:
    // 通知收集任务立即执行
    void doCollectWork(const QString &taskID);

    // 通知清理任务立即执行
    void doClearWork(const QString &taskID);

    // 通知收集任务开始或停止
    void enable(const QString &, bool);

private slots:
    void addCollectSet();
    void manageUser();

    bool addCollect(const CollectTask &task, bool bDb = true);



    bool addClear(const ClearTask &task, bool bDb = true);

    // 显示日志
    void addLog(const QString &dirName, const QString &info, int infoType=0);
    void addLog(const CollectTask &task, const QString &info, int infoType);



    // 打开源
    void openSrcPath();
    // 打开目标
    void openDstPath();
    // 立即执行
    void doNow();
    // 启动/停止
    void startOrstop();
    // 删除任务
    void deltTask();
    // 属性（查看/修改）
    void onProperty();
    // 清空日志
    void onClearLog();
    // 系统设置
    void onSystemSet();

    void onSelectTask(int index);

    // 设置列表图标状态
    void setTaskIcon(const CollectTask &task, int sendWay, int normal);

    // 收集动画
    void setGif(const QString &dirID, bool bFlag);

    // 显示窗体
    void showWin(QSystemTrayIcon::ActivationReason);
    // 后台运行
    void hideWin();
    // 更新状态栏
    void refreshStatus();

    void calRunTime();

    void onUserAction(QAction *act);

    void onNewClearTaskCreated(const ClearTask &task);

    void onClearTaskBegin(const QString &dir);

    void onClearTaskEnd(const QString &dir);

private:
    void InitUI();


    // 获取当前选中的任务ID
    QString getCurTaskID(QListWidgetItem *&pItem);

    // 根据任务ID获取Item窗体
    MyItemWidget *getItemWidget(const QString &taskID);

private:
    Ui::MainWindow ui;								///< 主窗体UI对象
    QLabel	*m_pRunTime;							///< 运行时长
    QLabel	*m_pSysTime;							///< 系统时间
    // QTime  m_runTime;
    QDateTime m_runTime;

    QSharedPointer<QSystemTrayIcon> m_pSysTrayicon;	///< 系统托盘

    uint m_logNum;

    QSharedPointer<CollectSetDlg>	m_pCollectDlg;
    QSharedPointer<UserManageDlg>	m_pUserManageDlg;
    QSharedPointer<DataClearDlg>    m_pClearDlg;

    //QAction *m_pColDistAct;			///< 收集分发设置

    //QAction *m_pParamAct;			///< 参数设置
    QAction *m_pOpenSrcAct;
    QAction *m_pOpenDstAct;
    //QToolButton *m_tbtnOpenDst;		///< 打开目标路径（支持多个）
    // QMenu *m_menuOpenDst;
    QSharedPointer<QActionGroup> m_grpAction;
    QAction *m_pExecuteNowAct;
    QAction *m_pEnableAct;
    QAction *m_pDeletAct;
    QAction *m_pEditAct;
    QAction *m_pAddAct;
    QAction *m_pTrayAct;
    QAction *m_pSet;			// 系统设置
    QAction *m_pClearAct;
    QAction *m_pUserInfoAct;		///< 用户管理设置

    TaskUser	m_tUser;			///< 记录当前选择的任务的分发用户

    DataBase *m_sqlite;
    /// 列表item对应的任务ID
    QMap<QListWidgetItem*, QString> m_ItemTask;

    //----插件相关----
    ICtkPluginManager	*m_pCtk;
    QSharedPointer<ICollectManager>	m_pCollect;

    IDataClear *m_pDataClear;
    //QSharedPointer<QThread> m_pClearThread;
    //----

    // 线程控制，用来控制打印日志
    QWaitCondition m_oConditon;
    QMutex m_oLocker;
    QMutex m_oLocker2;	// 控制往界面表格处打印
    int m_ilogsize;
};

#endif // mainwindow_h__
