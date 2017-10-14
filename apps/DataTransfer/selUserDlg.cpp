#include "selUserDlg.h"
#include <QMessageBox>

SelUserDlg::SelUserDlg(QDialog *parent /*= NULL*/)
    : QDialog(parent)
    , m_nFlag(0)
{
    ui.setupUi(this);
    InitUI();
}

SelUserDlg::SelUserDlg(int nFlag, CollectUser user, QDialog *parent /*= NULL*/)
    : QDialog(parent)
    , m_nFlag(nFlag)
    , m_selUser(user)
{
    ui.setupUi(this);
    InitUI();
}

SelUserDlg::~SelUserDlg()
{

}

void SelUserDlg::InitUI()
{
    // 从数据库读取分发用户
    QList<UserInfo> lstUser;
    if (!DataBase::getInstance()->QueryUserInfo(lstUser) || lstUser.size()==0)
    {
        return;
    }
    int nIndex = 0;
    for (int i=0; i<lstUser.size(); ++i)
    {
        nIndex++;
        ui.comboBox->addItem(lstUser.at(i).userName);
        m_iUser[nIndex] = lstUser.at(i).userID;
    }

    if (m_nFlag == 1)
    {
        ui.comboBox->setCurrentText(m_selUser.user.userName);
        ui.lineEdit->setText(m_selUser.rltvPath);
    }

    connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(onCommit()));
    connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));


}

void SelUserDlg::onCommit()
{
    int nIndex = ui.comboBox->currentIndex();
    if (nIndex == 0)
    {
        // 中文提示
        QMessageBox box(QMessageBox::Critical, QStringLiteral("提示"), "");
        box.setStandardButtons(QMessageBox::Ok);
        box.setButtonText(QMessageBox::Ok, QStringLiteral("确认"));
        box.setText(QStringLiteral("请选择一个分发用户！"));
        box.exec();
        return;
    }

    m_selUser.user.userID = m_iUser[nIndex];
    m_selUser.user.userName = ui.comboBox->currentText();
    m_selUser.rltvPath = ui.lineEdit->text();

    accept();	// QDialog的slot函数
}

void SelUserDlg::onCancel()
{
    reject();
}