#ifndef selUserDlg_H_
#define selUserDlg_H_

#include <QWidget>
#include <QDialog>
#include "ui_selUser.h"
#include "param.h"
#include "transferSet.h"

class SelUserDlg : public QDialog
{
    Q_OBJECT
public:
    explicit SelUserDlg(QString taskID, QStringList lstUsers, QWidget *parent=NULL);
    ~SelUserDlg() {}

    void initDlg();

    QStringList m_lstUserID;

private slots:
    void onOk();
    void onCancel();

private:
    Ui::selUser ui;
    TransParam *m_pParam;
    QVector<string> m_selUsers;	// 已选择用户
};


#endif
