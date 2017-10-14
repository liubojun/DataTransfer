#ifndef selUserDlg_H_
#define selUserDlg_H_


#include <QDialog>
#include "ui_selUser.h"
#include "DataBase.h"


class SelUserDlg : public QDialog
{
    Q_OBJECT
public:
    SelUserDlg(int nFlag, CollectUser user, QDialog *parent = NULL);
    SelUserDlg(QDialog *parent = NULL);
    ~SelUserDlg();

    void InitUI();

    QMap<int, QString> m_iUser;
    CollectUser m_selUser;

private slots:
    void onCommit();
    void onCancel();

private:
    Ui::selUserDlg ui;

    int m_nFlag;
};

#endif