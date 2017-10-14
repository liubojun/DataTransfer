#include "userManageDlg.h"
#include "cviewHeader.h"
#include "userInfo.h"
#include "ctkLog.h"
#include "event_macro.h"
#include <QCheckBox>

UserManagerDlg::UserManagerDlg(QWidget *parent /*= NULL*/)
    : m_Header(NULL)
{
    ui.setupUi(this);
    m_pParam = TransParam::getInstance();
    initDlg();
    ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_popMenu = new QMenu(ui.tableWidget);
    m_actEdit = new QAction(QStringLiteral("编辑"), this);
    m_actDelt = new QAction(QStringLiteral("删除"), this);
    m_popMenu->addAction(m_actEdit);
    m_popMenu->addAction(m_actDelt);
    connect(ui.btn_add, SIGNAL(clicked()), this, SLOT(onBtnAdd()));
    connect(ui.tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onRBtnMenu(QPoint)));
    connect(m_actEdit, SIGNAL(triggered()), this, SLOT(onEditUser()));
    connect(m_actDelt, SIGNAL(triggered()), this, SLOT(onDeleUser()));
    connect(ui.btn_batchDel, SIGNAL(clicked()), this, SLOT(onBtnBatchDel()));
}

UserManagerDlg::~UserManagerDlg()
{

}

void UserManagerDlg::initDlg()
{
    if (m_Header == NULL)
    {
        m_Header = new CViewHeader(0, Qt::Horizontal, ui.tableWidget);
    }
    connect(m_Header, SIGNAL(clickRow(bool)), this, SLOT(onSelAllUser(bool)));
    ui.tableWidget->setHorizontalHeader(m_Header);
    ui.tableWidget->horizontalHeader()->setStretchLastSection(true);		//最后一列自适应
    //int nWidth = ui.tableWidget->width();
    ui.tableWidget->setColumnWidth(0, 55);
    ui.tableWidget->setColumnWidth(1, 80);
    ui.tableWidget->setColumnWidth(2, 240);
    ui.tableWidget->setColumnWidth(3, 70);
    ui.tableWidget->setColumnWidth(4, 80);
    ui.tableWidget->setColumnWidth(5, 80);
    ui.tableWidget->setColumnWidth(6, 100);
    // 整行选中
    ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 只允许单行选中
    ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    // 禁止编辑
    ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    int nRowIndex = 0;
    const list<SendSet> &allUser = m_pParam->m_userList.lsts;
    list<SendSet>::const_iterator it = allUser.begin();
    for (; it != allUser.end(); ++it)
    {
        insertOneRow(nRowIndex, *it);
        nRowIndex++;
    }
}

void UserManagerDlg::onBtnAdd()
{
    m_pUserInfoDlg = QSharedPointer<UserInfoDlg>(new UserInfoDlg());
    m_pUserInfoDlg->show();
    connect(m_pUserInfoDlg.data(), SIGNAL(finish(int, SendSet)), this, SLOT(onSaveUserInfo(int, SendSet)));
}

void UserManagerDlg::onSaveUserInfo(int type, SendSet user)
{
    if (type == 0)		//添加用户
    {
        int rowCount = ui.tableWidget->rowCount();
        insertOneRow(rowCount, user);
        m_pParam->m_userList.lsts.push_back(user);
    }
    else				//修改用户
    {
        ui.tableWidget->removeRow(m_nEditRow);
        insertOneRow(m_nEditRow, user);

        list<SendSet> &allUser = m_pParam->m_userList.lsts;
        list<SendSet>::iterator it = allUser.begin();
        while (it != allUser.end())
        {
            if (it->strUsrname == user.strUsrname)
            {
                *it = user;
                break;
            }
            it++;
        }
    }

    m_pParam->saveUserInfo();
}

void UserManagerDlg::insertOneRow(int rowIndex, const SendSet &usr)
{
    QString strTmp("");
    ui.tableWidget->insertRow(rowIndex);
    //--插入复选框，并使其居中----
    QWidget *widget = new QWidget(ui.tableWidget);
    QCheckBox *ckBox = new QCheckBox(widget);
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(ckBox);
    hlayout->setMargin(0);
    hlayout->setAlignment(ckBox, Qt::AlignCenter);
    widget->setLayout(hlayout);
    ui.tableWidget->setCellWidget(rowIndex, 0, widget);
    //m_mapCheckbox[rowIndex] = ckBox;
    // 用户id
    ui.tableWidget->setItem(rowIndex, 1, new QTableWidgetItem(QString::fromStdString(usr.strUsrname)));
    // 分发方式
    if (usr.nSendWay == 0)
    {
        strTmp = QString::fromStdString(usr.strRltvPath);
    }
    else
    {
        strTmp = QString("ftp://%1:%2%3").arg(QString::fromStdString(usr.strIP)).arg(usr.nPort).arg(QString::fromStdString(usr.strRltvPath));
    }
    ui.tableWidget->setItem(rowIndex, 2, new QTableWidgetItem(strTmp));
    // 保持原目录结构
    if (usr.bKeepDir)
    {
        strTmp = QStringLiteral("是");
    }
    else
    {
        strTmp = QStringLiteral("否");
    }
    QTableWidgetItem *p1 = new QTableWidgetItem(strTmp);
    p1->setTextAlignment(Qt::AlignCenter);
    ui.tableWidget->setItem(rowIndex, 3, p1);
    // 断点续传
//     if (usr.bConSend)
//     {
//         strTmp = QStringLiteral("是");
//     }
//     else
//     {
//         strTmp = QStringLiteral("否");
//     }
//     QTableWidgetItem *p2 = new QTableWidgetItem(strTmp);
//     p2->setTextAlignment(Qt::AlignCenter);
//     ui.tableWidget->setItem(rowIndex, 4, p2);

//     if (usr.bEncrypt)
//     {
//         strTmp = QStringLiteral("是");
//     }
//     else
//     {
//         strTmp = QStringLiteral("否");
//     }
//     QTableWidgetItem *p3 = new QTableWidgetItem(strTmp);
//     p3->setTextAlignment(Qt::AlignCenter);
//     ui.tableWidget->setItem(rowIndex, 5, p3);

//     if (usr.bCompress)
//     {
//         strTmp = QStringLiteral("是");
//     }
//     else
//     {
//         strTmp = QStringLiteral("否");
//     }
//     QTableWidgetItem *p4 = new QTableWidgetItem(strTmp);
//     p4->setTextAlignment(Qt::AlignCenter);
//     ui.tableWidget->setItem(rowIndex, 6, p4);

    strTmp = QString::fromStdString(usr.strUsr);
    ui.tableWidget->setItem(rowIndex, 4, new QTableWidgetItem(strTmp));

    strTmp = QString::fromStdString(usr.strPwd);
    ui.tableWidget->setItem(rowIndex, 5, new QTableWidgetItem(strTmp));

    strTmp = QString::fromStdString(usr.strIP);
    ui.tableWidget->setItem(rowIndex, 6, new QTableWidgetItem(strTmp));

    strTmp = QString::number(usr.nPort);
    ui.tableWidget->setItem(rowIndex, 7, new QTableWidgetItem(strTmp));

    m_Header->setChecked(false);
}

void UserManagerDlg::onEditUser()
{
    int rowIndex = ui.tableWidget->currentRow();
    const list<SendSet> &allUser = m_pParam->m_userList.lsts;
    if (rowIndex < 0 || rowIndex >= (int)allUser.size())
    {
        return;
    }

    m_nEditRow = rowIndex;

    list<SendSet>::const_iterator it = allUser.begin();
    for (int i=0; i<rowIndex; i++)
    {
        it++;
    }

    m_pUserInfoDlg = QSharedPointer<UserInfoDlg>(new UserInfoDlg(1));
    m_pUserInfoDlg->setUserInfo(*it);
    m_pUserInfoDlg->show();
    connect(m_pUserInfoDlg.data(), SIGNAL(finish(int, SendSet)), this, SLOT(onSaveUserInfo(int, SendSet)));
}

void UserManagerDlg::onRBtnMenu(QPoint pos)
{
    int row = ui.tableWidget->indexAt(pos).row();
    if (row == -1 || ui.tableWidget->currentRow()==-1)
    {
        return;
    }
    m_popMenu->exec(QCursor::pos());
}

void UserManagerDlg::onDeleUser()
{
    int row = ui.tableWidget->currentRow();
    if (row == -1)
    {
        return;
    }

    deleteOneRow(row);

    m_pParam->saveUserInfo();
}

void UserManagerDlg::onBtnBatchDel()
{
    //QVector<int> setRows;
    //QMap<int, QCheckBox*>::const_iterator it = m_mapCheckbox.begin();
    //while (it != m_mapCheckbox.end())
    //{
    //    if (it.value() != NULL && it.value()->isChecked())
    //    {
    //        setRows.push_front(it.key());
    //    }
    //    ++it;
    //}

    //for (int i=0; i<setRows.size(); i++)
    //{
    //    deleteOneRow(setRows.at(i));
    //}

    // 需要逆序删除
    for (int i=ui.tableWidget->rowCount()-1; i>=0; --i)
    {
        QWidget *wdt = ui.tableWidget->cellWidget(i, 0);
        QCheckBox *ckb = qobject_cast<QCheckBox *>(wdt->children().at(0));
        if ( ckb != NULL && ckb->isChecked())
        {
            deleteOneRow(i);
        }
    }

    m_pParam->saveUserInfo();
}

void UserManagerDlg::showEvent(QShowEvent *e)
{
    //initDlg();
    return QWidget::showEvent(e);
}

bool UserManagerDlg::event(QEvent *e)
{
    if (e->type() == ApplyEvent)
    {
        //Apply();
        return true;
    }
    else if (e->type() == OkEvent)
    {
        emit ok();
        //Commit();
        return true;
    }
    else if (e->type() == CancelEvent)
    {
        //cancel();
        emit cancel();
        return true;
    }

    //return QObject::event(e);
    return QWidget::event(e);
}


void UserManagerDlg::onSelAllUser(bool bFlag)
{
    //QMap<int, QCheckBox*>::const_iterator i = m_mapCheckbox.constBegin();
    //while (i != m_mapCheckbox.constEnd())
    //{
    //    if (i.value() != NULL)
    //    {
    //        i.value()->setChecked(bFlag);
    //    }
    //    ++i;
    //}

    for (int r=0; r<ui.tableWidget->rowCount(); ++r)
    {
        // 获取子控件
        QWidget *wdt = ui.tableWidget->cellWidget(r, 0);
        QCheckBox *ckb = qobject_cast<QCheckBox *>(wdt->children().at(0));
        if (ckb != NULL)
        {
            ckb->setChecked(bFlag);
        }
    }
}

void UserManagerDlg::deleteOneRow(int rowIndex)
{
    QString strUser = ui.tableWidget->item(rowIndex, 1)->text();

    list<SendSet> &allUser = m_pParam->m_userList.lsts;
    list<SendSet>::iterator it = allUser.begin();
    while (it != allUser.end())
    {
        if (strUser.toStdString() == it->strUsrname)
        {
            allUser.erase(it);
            break;
        }
        it++;
    }

    ui.tableWidget->removeRow(rowIndex);
    //m_mapCheckbox.remove(rowIndex);
}
