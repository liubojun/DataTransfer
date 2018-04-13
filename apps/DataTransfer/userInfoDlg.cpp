#include "userInfoDlg.h"
#include "pathbuilder.h"
#include "LibCurlFtp.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QPalette>
#include <curl/curl.h>


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
    connect(ui.pbt_test, SIGNAL(clicked()), this, SLOT(onTest()));
    connect(this, SIGNAL(testok(const QString &)), this, SLOT(onTestOk(const QString &)));
    connect(this, SIGNAL(testfail(const QString &)), this, SLOT(onTestFail(const QString &)));
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
    user.userName = ui.lineEdit_username->text();
    user.sendType = ui.radioButton->isChecked() ? 0 : 1;
    user.sendSuffix = ui.lineEdit_sendSuffix->text();
    user.rootPath = ui.lineEdit_path->text();
    user.timebaserule = ui.cb_timebaserule->currentIndex();
    user.ip = ui.lineEdit_ip->text();
    user.port = ui.lineEdit_port->text().toInt();
    user.lgUser = ui.lineEdit_user->text();
    user.lgPass = ui.lineEdit_pwd->text();
    user.compress = ui.checkBox->isChecked() ? 1 : 0;
    //user.encrypt = ui.checkBox_2->isChecked() ? 1 : 0;
    user.keepDir = ui.checkBox_3->isChecked() ? 1 : 0;
    user.ftpTransferType = ui.cbx_ftpTransferType->currentIndex();
    user.ftpTransferMode = ui.cbx_ftpTransferMode->currentIndex();

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
        m_ItemUser[pItem] = user;
    }
    m_nFlag = 0;
    return true;
}

void UserManageDlg::onAdd()
{
    // 清空内容
    ui.lineEdit_username->clear();
    ui.radioButton->setChecked(true);
    ui.lineEdit_path->clear();
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
                    QStringLiteral("你确定要删除用户名为“%1”的分发用户吗?").arg(ui.lineEdit_username->text()));
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
        //ui.lineEdit_ip->hide();
        //ui.lineEdit_port->hide();
        //ui.lineEdit_user->hide();
        //ui.lineEdit_pwd->hide();
        ui.lineEdit_ip->setEnabled(false);
        ui.lineEdit_port->setEnabled(false);
        ui.lineEdit_user->setEnabled(false);
        ui.lineEdit_pwd->setEnabled(false);
        ui.cbx_ftpTransferType->setEnabled(false);
        ui.cbx_ftpTransferMode->setEnabled(false);
        ui.lineEdit_ip->clear();
    }
    else
    {
        ui.label_3->setText(QStringLiteral("相对路径："));
        ui.btnBrowse->hide();
        ui.lineEdit_ip->setEnabled(true);
        ui.lineEdit_port->setEnabled(true);
        ui.lineEdit_user->setEnabled(true);
        ui.lineEdit_pwd->setEnabled(true);
        ui.cbx_ftpTransferType->setEnabled(true);
        ui.cbx_ftpTransferMode->setEnabled(true);
        ui.lineEdit_ip->setText("/");
    }
}

void UserManageDlg::InitUI()
{
    ui.testResult_label->setText("");

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
    path = QFileDialog::getExistingDirectory(this, QStringLiteral("选择目标路径"), ui.lineEdit_path->text());
    if (!path.isEmpty())
    {
        ui.lineEdit_path->setText(path);
        ui.lineEdit_path->setCursorPosition(-1);
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
        ui.lineEdit_username->setText(it->userName);
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
        ui.lineEdit_path->setText(it->rootPath);
        ui.lineEdit_ip->setText(it->ip);
        ui.lineEdit_port->setText(QString::number(it->port));
        ui.lineEdit_user->setText(it->lgUser);
        ui.lineEdit_pwd->setText(it->lgPass);
        ui.checkBox->setChecked(it->compress);
        ui.cb_timebaserule->setCurrentIndex(it->timebaserule);
        // ui.checkBox_2->setChecked(it->encrypt);
        ui.checkBox_3->setChecked(it->keepDir);
        ui.cbx_ftpTransferType->setCurrentIndex(it->ftpTransferType);
        ui.cbx_ftpTransferMode->setCurrentIndex(it->ftpTransferMode);


    }
    else
    {
        ui.lineEdit_username->clear();
        ui.radioButton->setChecked(true);
        onSelWay(true);
        ui.lineEdit_path->clear();
        ui.lineEdit_ip->clear();
        ui.lineEdit_port->clear();
        ui.lineEdit_user->clear();
        ui.lineEdit_pwd->clear();
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

void UserManageDlg::onTest()
{
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::black);
    ui.testResult_label->setPalette(pal);
    ui.testResult_label->setText(QStringLiteral("正在测试网络连接，请稍后..."));
    QTimer::singleShot(300, this, SLOT(onRemoteColTest()));
}

void UserManageDlg::onRemoteColTest()
{
    if (ui.radioButton->isChecked())
    {
        QStringList retUrls = CPathBuilder::getFinalPathFromUrl(ui.lineEdit_path->text());
        foreach(QString strUrl, retUrls)
        {
            QUrl url = QUrl::fromLocalFile(strUrl);
            if (url.isLocalFile())
            {
                QDir qdir(strUrl);
                if (qdir.exists())
                {
                    emit testok(strUrl);
                }
                else
                {
                    emit testfail(strUrl);
                }
            }
        }


    }
    else
    {
        FTP::CFtp oFtp;

        // 使用主动，默认为被动
        oFtp.setTransferMode(1 == ui.cbx_ftpTransferMode->currentIndex() ? FTP::Active : FTP::Passive);
        oFtp.connectToHost(ui.lineEdit_ip->text(), ui.lineEdit_port->text().toInt());
        int ret = oFtp.login(ui.lineEdit_user->text(), ui.lineEdit_pwd->text(), 5);

        if (CURLE_OK != ret)
        {
            emit testfail(QString(ui.lineEdit_ip->text()));
        }
        else
        {
            emit testok(QString(ui.lineEdit_ip->text()));
        }

    }
}

void UserManageDlg::onTestOk(const QString &url)
{
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::green);
    ui.testResult_label->setPalette(pal);
    ui.testResult_label->setText(QStringLiteral("连接[") + url + QStringLiteral("]成功"));
    QTimer::singleShot(3000, this, SLOT(onTestResultTimeout()));
}

void UserManageDlg::onTestFail(const QString &url)
{
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::red);
    ui.testResult_label->setPalette(pal);
    ui.testResult_label->setText(QStringLiteral("连接[") + url + QStringLiteral("]失败"));
    QTimer::singleShot(3000, this, SLOT(onTestResultTimeout()));
}

void UserManageDlg::onTestResultTimeout()
{
    ui.testResult_label->setText("");
}
