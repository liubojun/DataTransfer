#ifndef addModel_H_
#define addModel_H_


#include <QDialog>
#include "ui_addModel.h"
#include "param.h"
#include "transferSet.h"

class AddModelDlg : public QDialog
{
    Q_OBJECT
public:
    AddModelDlg(QStringList lstModels, QWidget *parent=NULL);
    ~AddModelDlg() {}

    void initDlg();

    QStringList m_lstModels;

private slots:
    void onBtnOk();
    void onBtnAdd();
    void onBtnDel();

private:
    Ui::addModel ui;
};

#endif
