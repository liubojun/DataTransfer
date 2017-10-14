#ifndef userManageDlg_H_
#define userManageDlg_H_

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QSharedPointer>
#include "ui_userManage.h"
#include "param.h"
#include "userInfo.h"

class CViewHeader;

class UserManagerDlg : public QWidget
{
    Q_OBJECT
public:
    explicit UserManagerDlg(QWidget *parent = NULL);
    ~UserManagerDlg();
    void initDlg();

signals:
    void ok();
    void cancel();

protected:
    virtual void showEvent(QShowEvent *e);
    virtual bool event(QEvent *e);

private slots:
    void onBtnAdd();
    void onSaveUserInfo(int type, SendSet user);
    void onEditUser();
    void onRBtnMenu(QPoint pos);
    void onDeleUser();
    void onBtnBatchDel();
    void onSelAllUser(bool bFlag);

private:
    void insertOneRow(int rowIndex, const SendSet &usr);
    void deleteOneRow(int rowIndex);

private:
    Ui::userManager ui;
    QMenu *m_popMenu;
    QAction *m_actEdit;
    QAction *m_actDelt;
    QSharedPointer<UserInfoDlg> m_pUserInfoDlg;
    TransParam *m_pParam;
    int m_nEditRow;
    //QMap<int, QCheckBox*> m_mapCheckbox;
    QVector<QCheckBox *> m_vecCheckBox;

    CViewHeader *m_Header;
};

#endif
