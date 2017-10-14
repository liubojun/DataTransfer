#include "addModel.h"
#include "ctkLog.h"
#include <QMessageBox>

AddModelDlg::AddModelDlg(QStringList lstModels, QWidget *parent/*=NULL*/)
    : QDialog(parent)
    , m_lstModels(lstModels)
{
    ui.setupUi(this);
    initDlg();
    connect(ui.btn_ok, SIGNAL(clicked()), this, SLOT(onBtnOk()));
    connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui.btn_add, SIGNAL(clicked()), this, SLOT(onBtnAdd()));
    connect(ui.btn_del, SIGNAL(clicked()), this, SLOT(onBtnDel()));
}

void AddModelDlg::initDlg()
{
    for (int i=0; i<m_lstModels.size(); ++i)
    {
        ui.listWidget->addItem(m_lstModels.at(i));
    }
}

void AddModelDlg::onBtnOk()
{
    m_lstModels.clear();
    for (int i=0; i<ui.listWidget->count(); ++i)
    {
        m_lstModels.append(ui.listWidget->item(i)->text());
    }
    accept();
}

void AddModelDlg::onBtnAdd()
{
    QString str = ui.lineEdit->text().trimmed();
    if (str.isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("错误提示"), QStringLiteral("模板不能为空！"));
        return;
    }
    if (m_lstModels.contains(str))
    {
        QMessageBox::warning(this, QStringLiteral("错误提示"), QStringLiteral("该模板已存在！"));
        return;
    }
    ui.listWidget->addItem(str);
    m_lstModels.append(str);
}

void AddModelDlg::onBtnDel()
{
    int nSel = ui.listWidget->currentRow();
    if (nSel < 0)
    {
        return;
    }
    QString str = ui.listWidget->currentItem()->text();
    m_lstModels.removeAll(str);
    ui.listWidget->takeItem(nSel);
}
