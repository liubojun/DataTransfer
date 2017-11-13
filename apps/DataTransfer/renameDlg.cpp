#include "renameDlg.h"
#include <QFile>
#include <QTextStream>

#include "change_name.h"

CRenameDlg::CRenameDlg(QDialog *parent /*= 0*/)
{
    ui.setupUi(this);
    init();
}

void CRenameDlg::init()
{
    renamerulefile = qApp->applicationDirPath() + "/config/rename.rule";
    QFile file(renamerulefile);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QTextStream stream(&file);

    ui.fileContent->setText(stream.readAll());

    connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(onSave()));
    connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(onCancel()));
    connect(ui.lineEdit_rule, SIGNAL(textChanged(const  QString &)), this, SLOT(onlineEdit_rule_Change(const QString &)));
    connect(ui.textEdit_ori_name, SIGNAL(textChanged()), this, SLOT(onTextEdit_ori_name_Change()));
    connect(ui.btn_change, SIGNAL(clicked()), this, SLOT(onChangeTest()));


    ui.btn_change->setDisabled(true);
}

void CRenameDlg::onSave()
{
    QFile file(renamerulefile);
    if (!file.open(QIODevice::WriteOnly))
    {
        return;
    }
    QTextStream stream(&file);

    QString strContent = ui.fileContent->toPlainText();
    stream << strContent;

    file.close();
    CChangeName::init_rename_rules(renamerulefile.toStdString().c_str());


    // 需要实时通知tab2的测试的下拉框以及收集面板中的下拉框
}

void CRenameDlg::onCancel()
{
    reject();
}

void CRenameDlg::onTextEdit_ori_name_Change()
{
    if (!ui.textEdit_ori_name->toPlainText().isEmpty() && !ui.lineEdit_rule->text().isEmpty())
    {
        ui.btn_change->setDisabled(false);
    }
    else
    {
        ui.btn_change->setDisabled(true);
    }
}


void CRenameDlg::onlineEdit_rule_Change(const QString &rule)
{
    if (!ui.textEdit_ori_name->toPlainText().isEmpty() && !ui.lineEdit_rule->text().isEmpty())
    {
        ui.btn_change->setDisabled(false);
    }
    else
    {
        ui.btn_change->setDisabled(true);
    }
}



void CRenameDlg::onChangeTest()
{
    std::string strnewName = CChangeName::change_name_by_name(ui.textEdit_ori_name->toPlainText().toAscii().data(), ui.lineEdit_rule->text().toAscii().data());

    if (strnewName.empty())
    {
        ui.textEdit_new_name->setText("rule is incorrect, can not change name");
    }
    else
    {
        ui.textEdit_new_name->setText(strnewName.c_str());
    }

}



