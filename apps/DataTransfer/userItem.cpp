#include "userItem.h"
#include "DataBase.h"
#include "selUserDlg.h"

UserItem::UserItem(QListWidget *in_pLstWgt, const QString &userId, const QString &userName, const QString &rltvPath) : m_pWidget(in_pLstWgt), m_strUserId(userId)
{
    ui.setupUi(this);

    ui.leUser->setText(userName);
    ui.lePath->setText(rltvPath);
    m_strUserName = userName;
    m_strRtvPath = rltvPath;

    QObject::connect(ui.btnMdfy, SIGNAL(clicked()), this, SLOT(onMdfy()));
    QObject::connect(ui.btnDelt, SIGNAL(clicked()), this, SLOT(onDelt()));
}

UserItem::~UserItem()
{

}

void UserItem::onMdfy()
{
    CollectUser oUser;
    oUser.user.userName = m_strUserName;
    oUser.rltvPath = m_strRtvPath;
    SelUserDlg oSelUserDlg(1, oUser);

    if (QDialog::Accepted == oSelUserDlg.exec())
    {
        ui.leUser->setText(oSelUserDlg.m_selUser.user.userName);
        ui.lePath->setText(oSelUserDlg.m_selUser.rltvPath);
        m_strUserName = oSelUserDlg.m_selUser.user.userName;
        m_strRtvPath = oSelUserDlg.m_selUser.rltvPath;
        m_strUserId = oSelUserDlg.m_selUser.user.userID;
    }
}

void UserItem::onDelt()
{
    //DataBase::getInstance()->DeleteCollectSenderUser(m_strDirId, m_strUserId);
    //m_pWidget->takeItem(0);
    emit onDelete(m_strUserId);
    delete this;
}

QString UserItem::senduserId()
{
    return m_strUserId;
}

QString UserItem::sendusername()
{
    return m_strUserName;
}

QString UserItem::sendRtvPath()
{
    return m_strRtvPath;
}
