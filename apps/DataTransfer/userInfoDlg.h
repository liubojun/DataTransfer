#ifndef userInfoDlg_H_
#define userInfoDlg_H_


#include <QDialog>
#include <QUuid>
#include "ui_userManage.h"
#include "DataBase.h"

class UserManageDlg : public QDialog
{
    Q_OBJECT
public:
    UserManageDlg(QDialog *parent = NULL);
    ~UserManageDlg();

signals:
    void testok(QString url);

    void testfail(QString url);

private slots:
    void onAdd();
    void onDelt();
    bool onApply();
    void onOk();
    void onCancel();
    void onSelWay(bool bFile);
    void onBrowse();
    void onSelectUser(int);

    // 测试当前远程分发用户是否可用
    void onTest();

    void onRemoteColTest();

    void onTestOk(const QString &);

    void onTestFail(const QString &);

    void onTestResultTimeout();
private:
    void InitUI();

private:
    Ui::userManage ui;
    int m_nFlag;	///< 1-新增

    QMap<QListWidgetItem*, UserInfo> m_ItemUser;
};

#endif