#ifndef userInfo_H_
#define userInfo_H_


#include <QWidget>
#include "ui_addUser.h"
#include "transferSet.h"

class UserInfoDlg : public QWidget
{
    Q_OBJECT
public:
    explicit UserInfoDlg(int nType=0, QWidget *parent=NULL);
    ~UserInfoDlg() {}
    void setUserInfo(SendSet set);
    SendSet getUserInfo();

signals:
    void finish(int type, SendSet user);

private slots:
    void onBtnOk();
    void onCancel();

private:
    Ui::addUser ui;
    QButtonGroup *m_sendWay;

    int m_nType;		///< 0-添加用户，1-修改用户
    SendSet m_userInfo;
};


#endif
