#ifndef collectSetDlg_H_
#define collectSetDlg_H_

#include "ui_datacollect.h"
#include "collectSet.h"
#include <QWidget>

class CollectSetDlg : public QWidget
{
    Q_OBJECT
public:
    CollectSetDlg(QWidget *parent=NULL);
    ~CollectSetDlg();
private slots:
    void onBtnSelectPath();
private:
    void loadCollects();
private:
    Ui::datacollect ui;
    CollectSetList m_lstCollects;
    QString m_strConPath;
};


#endif