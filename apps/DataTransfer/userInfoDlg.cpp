#include "userInfoDlg.h"
#include <QMessageBox>
#include <QFileDialog>

UserManageDlg::UserManageDlg(QDialog *parent /*= NULL*/)
    : QDialog(parent)
    , m_nFlag(0)
{
    ui.setupUi(this);

    InitUI();

    connect(ui.radioButton, SIGNAL(toggled(bool)), this, SLOT(onSelWay(bool)));
    connect(ui.btnBrowse, SIGNAL(clicked()), this, SLOT(onBrowse()));
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onAdd()));
    connect(ui.btnApply, SIGNAL(clicked()), this, SLOT(onApply()));
    connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(onOk()));
    connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(onDelt()));
    connect(ui.listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(onSelectUser(int)));
}

UserManageDlg::~UserManageDlg()
{

}

bool UserManageDlg::onApply()
{
    QMessageBox box(QMessageBox::Critical, QStringLiteral("提示"), "");
    box.setStandardButtons(QMessageBox::Ok);
    box.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));

    UserInfo user;
    user.userName = ui.lineEdit->text();
    user.sendType = ui.radioButton->isChecked() ? 0 : 1;
    user.sendSuffix = ui.lineEdit_sendSuffix->text();
    user.rootPath = ui.lineEdit_2->text();
    user.ip = ui.lineEdit_3->text();
    user.port = ui.lineEdit_4->text().toInt();
    user.lgUser = ui.lineEdit_5->text();
    user.lgPass = ui.lineEdit_6->text();
    user.compress = ui.checkBox->isChecked() ? 1 : 0;
    //user.encrypt = ui.checkBox_2->isChecked() ? 1 : 0;
    user.keepDir = ui.checkBox_3->isChecked() ? 1 : 0;

    // 新增用户
    if (m_nFlag == 1)
    {
        user.userID = QUuid::createUuid().toString();

        // 插入数据库
        if (!DataBase::getInstance()->InsertUserInfo(user))
        {
            return false;
        }
        // 插入界面
        QListWidgetItem *pItem = new QListWidgetItem(ui.listWidget);
        pItem->setTextAlignment(Qt::AlignHCenter);
        pItem->setText(user.userName);
        ui.listWidget->addItem(pItem);
        pItem->setToolTip(user.userName);
        // 记录到内存
        m_ItemUser[pItem] = user;
        ui.listWidget->setCurrentRow(ui.listWidget->count() - 1);
    }
    // 修改用户
    else
    {
        QListWidgetItem *pItem = ui.listWidget->currentItem();
        if (m_ItemUser.contains(pItem))
        {
            user.userID = m_ItemUser[pItem].userID;
        }
        if (!DataBase::getInstance()->InsertUserInfo(user))
        {
            return false;
        }
    }
    m_nFlag = 0;
    return true;
}

void UserManageDlg::onAdd()
{
    // 清空内容
    ui.lineEdit->clear();
    ui.radioButton->setChecked(true);
    ui.lineEdit_2->clear();
    ui.checkBox_3->setChecked(true);
    ui.listWidget->setCurrentRow(-1);
    ui.lineEdit_sendSuffix->setText(".tmp");
    m_nFlag = 1;
}

void UserManageDlg::onDelt()
{
    int curSel = ui.listWidget->currentRow();
    if (curSel < 0)
    {
        m_nFlag = 0;
        return;
    }

    QMessageBox box(QMessageBox::Warning, QStringLiteral("提示"),
                    QStringLiteral("你确定要删除用户名为“%1”的分发用户吗?").arg(ui.lineEdit->text()) );
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    box.setButtonText(QMessageBox::Yes, QStringLiteral("是"));
    box.setButtonText(QMessageBox::No, QStringLiteral("否"));
    if (QMessageBox::No == box.exec())
    {
        m_nFlag = 0;
        return;
    }
    // 从数据库、内存、界面删除
    QListWidgetItem *pItem = ui.listWidget->currentItem();
    QMap<QListWidgetItem*, UserInfo>::const_iterator it = m_ItemUser.find(pItem);
    if (it != m_ItemUser.end())
    {
        if (DataBase::getInstance()->DeleteSendUser(it->userID))
        {
            m_ItemUser.remove(pItem);
            ui.listWidget->removeItemWidget(pItem);
            ui.listWidget->takeItem(curSel);
            ui.listWidget->setCurrentRow(0);
        }
    }


}

void UserManageDlg::onSelWay(bool bFile)
{
    if (bFile)
    {
        ui.label_3->setText(QStringLiteral("目标路径："));
        ui.btnBrowse->show();
//         ui.label_4->hide();
//         ui.label_5->hide();
//         ui.label_6->hide();
//         ui.label_7->hide();
        //ui.lineEdit_3->hide();
        //ui.lineEdit_4->hide();
        //ui.lineEdit_5->hide();
        //ui.lineEdit_6->hide();
        ui.lineEdit_3->setEnabled(false);
        ui.lineEdit_4->setEnabled(false);
        ui.lineEdit_5->setEnabled(false);
        ui.lineEdit_6->setEnabled(false);
        ui.lineEdit_3->clear();
    }
    else
    {
        ui.label_3->setText(QStringLiteral("相对路径："));
        ui.btnBrowse->hide();
        ui.lineEdit_3->setEnabled(true);
        ui.lineEdit_4->setEnabled(true);
        ui.lineEdit_5->setEnabled(true);
        ui.lineEdit_6->setEnabled(true);
        ui.lineEdit_3->setText("/");
    }
}

void UserManageDlg::InitUI()
{
    // 从数据库读取
    QList<UserInfo> lstUser;
    if (!DataBase::getInstance()->QueryUserInfo(lstUser))
    {
        return;
    }
    m_ItemUser.clear();
    if (lstUser.size() == 0)
    {
        return;
    }
    for (int i=0; i<lstUser.size(); ++i)
    {
        QListWidgetItem *pItem = new QListWidgetItem(ui.listWidget);
        pItem->setTextAlignment(Qt::AlignHCenter);
        pItem->setText(lstUser.at(i).userName);
        ui.listWidget->addItem(pItem);
        pItem->setToolTip(lstUser.at(i).userName);
        m_ItemUser[pItem] = lstUser.at(i);
    }
    ui.listWidget->setCurrentRow(0);
    onSelectUser(0);
}

void UserManageDlg::onBrowse()
{
    QString path;
    path = QFileDialog::getExistingDirectory(this, QStringLiteral("选择目标路径"), ui.lineEdit_2->text());
    if (!path.isEmpty())
    {
        ui.lineEdit_2->setText(path);
        ui.lineEdit_2->setCursorPosition(-1);
    }
}

void UserManageDlg::onSelectUser(int nRow)
{
    if (nRow < 0 || nRow > m_ItemUser.size())
    {
        return;
    }

    QListWidgetItem *pItem = ui.listWidget->currentItem();
    QMap<QListWidgetItem*, UserInfo>::const_iterator it = m_ItemUser.find(pItem);
    if (it != m_ItemUser.end())
    {
        ui.lineEdit->setText(it->userName);
        if (it->sendType == 0)
        {
            ui.radioButton->setChecked(true);
            onSelWay(true);
        }
        else
        {
            ui.radioButton_2->setChecked(true);
            onSelWay(false);
        }
        ui.lineEdit_sendSuffix->setText(it->sendSuffix);
        ui.lineEdit_2->setText(it->rootPath);
        ui.lineEdit_3->setText(it->ip);
        ui.lineEdit_4->setText(QString::number(it->port));
        ui.lineEdit_5->setText(it->lgUser);
        ui.lineEdit_6->setText(it->lgPass);
        ui.checkBox->setChecked(it->compress);
        // ui.checkBox_2->setChecked(it->encrypt);
        ui.checkBox_3->setChecked(it->keepDir);
    }
    else
    {
        ui.lineEdit->clear();
        ui.radioButton->setChecked(true);
        onSelWay(true);
        ui.lineEdit_2->clear();
        ui.lineEdit_3->clear();
        ui.lineEdit_4->clear();
        ui.lineEdit_5->clear();
        ui.lineEdit_6->clear();
        ui.checkBox->setChecked(false);
        //  ui.checkBox_2->setChecked(false);
        ui.checkBox_3->setChecked(true);
    }
}

void UserManageDlg::onOk()
{
    if (onApply())
    {

    }
    accept();
}

void UserManageDlg::onCancel()
{
    reject();
}
