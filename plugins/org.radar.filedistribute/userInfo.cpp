#include "userInfo.h"
#include "ctkLog.h"
#include <QMessageBox>

UserInfoDlg::UserInfoDlg(int nType/*=0*/, QWidget *parent/*=NULL*/)
    : m_nType(nType)
{
    ui.setupUi(this);
    m_sendWay = new QButtonGroup(this);
    m_sendWay->addButton(ui.rBtn_file);
    m_sendWay->addButton(ui.rBtn_ftp);
    m_sendWay->setExclusive(true);
    ui.rBtn_file->setChecked(true);

    connect(ui.btn_ok, SIGNAL(clicked()), this, SLOT(onBtnOk()));
    connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(onCancel()));

    this->setWindowTitle(QStringLiteral("添加用户"));
    this->setWindowModality(Qt::ApplicationModal);

    // 只保留关闭按钮
    Qt::WindowFlags flags = Qt::Window;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
}

void UserInfoDlg::setUserInfo(SendSet set)
{
    this->setWindowTitle(QStringLiteral("修改用户"));
    m_userInfo = set;
    ui.le_ID->setText(QString::fromStdString(set.strUsrname));
    ui.le_ID->setEnabled(false);
    if (set.nSendWay == 1)
    {
        ui.rBtn_ftp->setChecked(true);
    }
    ui.le_path->setText(QString::fromStdString(set.strRltvPath));
//     ui.ckb_Comp->setChecked(set.bCompress);
//     ui.ckb_Conput->setChecked(set.bConSend);
//     ui.ckb_Encrypt->setChecked(set.bEncrypt);
    ui.ckb_KeepDir->setChecked(set.bKeepDir);
    ui.le_IP->setText(QString::fromStdString(set.strIP));
    ui.le_port->setText(QString::number(set.nPort));
    ui.le_lgName->setText(QString::fromStdString(set.strUsr));
    ui.le_lgPwd->setText(QString::fromStdString(set.strPwd));
}

void UserInfoDlg::onBtnOk()
{
    // 中文提示对话框
    QMessageBox box(QMessageBox::Critical, QStringLiteral("提示"), "");
    box.setStandardButtons(QMessageBox::Ok);
    box.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));

    // 检验合法性
    if (ui.le_ID->text().trimmed().isEmpty())
    {
        box.setText(QStringLiteral("用户名不能为空！"));
        box.exec();
        return;
    }

    if (ui.le_path->text().trimmed().isEmpty())
    {
        box.setText(QStringLiteral("相对路径不能为空！"));
        box.exec();
        return;
    }

    if (m_nType == 0)
    {
    }
    else
    {

    }

    m_userInfo.strUsrname = ui.le_ID->text().toStdString();
    m_userInfo.nSendWay = ui.rBtn_file->isChecked() ? 0 : 1;
    m_userInfo.strRltvPath = ui.le_path->text().toStdString();
//     m_userInfo.bConSend = ui.ckb_Conput->isChecked();
//     m_userInfo.bCompress = ui.ckb_Comp->isChecked();
//     m_userInfo.bEncrypt = ui.ckb_Encrypt->isChecked();
    m_userInfo.bKeepDir = ui.ckb_KeepDir->isChecked();
    m_userInfo.strIP = ui.le_IP->text().toStdString();
    m_userInfo.nPort = ui.le_port->text().toInt();
    m_userInfo.strUsr = ui.le_lgName->text().toStdString();
    m_userInfo.strPwd = ui.le_lgPwd->text().toStdString();

    emit finish(m_nType, m_userInfo);

    this->close();
}

void UserInfoDlg::onCancel()
{
    close();
}
