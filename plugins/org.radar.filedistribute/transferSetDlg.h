#ifndef transferSetDlg_H_
#define transferSetDlg_H_

#include <QWidget>
#include <QButtonGroup>
#include "ui_transferSet.h"
#include "param.h"

class ICtkPluginManager;

class TransferSetDlg : public QWidget
{
    Q_OBJECT
public:
    explicit TransferSetDlg(ICtkPluginManager *pManager, QWidget *parent=0);
    ~TransferSetDlg();

    void initDlg();
    void showInfo(const TransferSet &set);

    void getUserID();
    void clearAllInfo();

private slots:
    void onChangeTask(int rowNo);
    void onSaveTask();
    void onApplyTask();
    void onSelUser();
    void onBtnAddTask();
    void onBtnDelTask();
    void onBtnAddModel();
    void onSelCollectWay(int id);

private:
    Ui::transferSet ui;
    QButtonGroup *m_collectWayGroup;
    QVector<QLineEdit *> m_vecDispath;
    TransParam *m_pParam;
    QStringList m_lstUserID;
    QStringList m_lstModels;
    int m_nTaskFlag;	// 0-初始值，1-新增，2-修改
    TransferSet m_tranSet;
    ICtkPluginManager *m_pManager;
};


#endif