#include "selUserDlg.h"
#include <QCheckBox>
#include <QHBoxLayout>

SelUserDlg::SelUserDlg(QString taskID, QStringList lstUsers, QWidget *parent/*=NULL*/)
    : QDialog(parent)
    , m_lstUserID(lstUsers)
{
    ui.setupUi(this);
    m_pParam = TransParam::getInstance();
    m_selUsers = m_pParam->getSelectUserID(taskID);
    initDlg();
    connect(ui.btn_ok, SIGNAL(clicked()), this, SLOT(onOk()));
    connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(onCancel()));

    Qt::WindowFlags flags = Qt::Window;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
}

void SelUserDlg::initDlg()
{
    ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
    int nWidth = ui.tableWidget->width();
    ui.tableWidget->setColumnWidth(0, 40);
    ui.tableWidget->setColumnWidth(1, 90);
//    ui.tableWidget->setColumnWidth(2, 6*nWidth/13);
//     ui.tableWidget->setColumnWidth(3, 2*nWidth/13);
//     ui.tableWidget->setColumnWidth(4, nWidth/13);
//     ui.tableWidget->setColumnWidth(5, nWidth/13);

    int rowNo = 0;
    QString strTmp("");
    QString strKey;
    QStringList lstValue;
    QMap<QString, QStringList> res = m_pParam->getAllUserShowInfo();
    // Java风格的遍历
    QMapIterator<QString, QStringList> it(res);
    while (it.hasNext())
    {
        it.next();
        strKey = it.key();
        lstValue = it.value();
        ui.tableWidget->insertRow(rowNo);
        QCheckBox *ckb = new QCheckBox;
        QWidget *widget = new QWidget(ui.tableWidget);
        QHBoxLayout *hlayout = new QHBoxLayout;
        hlayout->addWidget(ckb);
        hlayout->setMargin(0);
        hlayout->setAlignment(ckb, Qt::AlignCenter);
        widget->setLayout(hlayout);
        // 判断是否勾选
        //if (m_selUsers.contains(strKey.toStdString()))
        if (m_lstUserID.contains(strKey))
        {
            ckb->setChecked(true);
        }
        ui.tableWidget->setCellWidget(rowNo, 0, widget);
        ui.tableWidget->setItem(rowNo, 1, new QTableWidgetItem(strKey));
        ui.tableWidget->setItem(rowNo, 2, new QTableWidgetItem(lstValue.at(0)));
        ui.tableWidget->setItem(rowNo, 3, new QTableWidgetItem(lstValue.at(1)));
        ui.tableWidget->setItem(rowNo, 4, new QTableWidgetItem(lstValue.at(2)));
        ui.tableWidget->setItem(rowNo, 5, new QTableWidgetItem(lstValue.at(4)));
        rowNo++;
    }
}

void SelUserDlg::onOk()
{
    m_lstUserID.clear();
    for (int i=0; i<ui.tableWidget->rowCount(); ++i)
    {
        QCheckBox *ckb = (QCheckBox *)ui.tableWidget->cellWidget(i, 0)->children().at(1);
        if (ckb != NULL && ckb->isChecked())
        {
            m_lstUserID.append(ui.tableWidget->item(i, 1)->text());
        }
    }
    accept();
}

void SelUserDlg::onCancel()
{
    reject();
}
