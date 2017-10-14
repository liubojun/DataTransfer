#include "transferSetDlg.h"
#include "selUserDlg.h"
#include "addModel.h"
#include "IDataCollection.h"
#include <QMessageBox>


TransferSetDlg::TransferSetDlg(ICtkPluginManager *pManager, QWidget *parent/*=0*/)
    : m_pManager(pManager)
{
    m_pParam = TransParam::getInstance();
    ui.setupUi(this);
    initDlg();
    connect(ui.listWidget_ID, SIGNAL(currentRowChanged(int)), this, SLOT(onChangeTask(int)));
    connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(onSaveTask()));
    connect(ui.btn_apply, SIGNAL(clicked()), this, SLOT(onApplyTask()));
    connect(ui.tBtn_selUser, SIGNAL(clicked()), this, SLOT(onSelUser()));
    connect(ui.btn_add, SIGNAL(clicked()), this, SLOT(onBtnAddTask()));
    connect(ui.btn_del, SIGNAL(clicked()), this, SLOT(onBtnDelTask()));
    connect(ui.btn_addModel, SIGNAL(clicked()), this, SLOT(onBtnAddModel()));
    connect(m_collectWayGroup, SIGNAL(buttonClicked(int)), this, SLOT(onSelCollectWay(int)));
}

TransferSetDlg::~TransferSetDlg()
{

}

void TransferSetDlg::initDlg()
{
    // 相关控件初始化
    m_collectWayGroup = new QButtonGroup;
    m_collectWayGroup->addButton(ui.rBtn_File1, 0);
    m_collectWayGroup->addButton(ui.rBtn_File2, 1);
    m_collectWayGroup->addButton(ui.rBtn_Ftp, 2);
    m_collectWayGroup->setExclusive(true);
    m_collectWayGroup->button(2)->setChecked(true);

    m_vecDispath.append(ui.le_dp1);
    m_vecDispath.append(ui.le_dp2);
    m_vecDispath.append(ui.le_dp3);
    m_vecDispath.append(ui.le_dp4);
    m_vecDispath.append(ui.le_dp5);
    m_vecDispath.append(ui.le_dp6);

    const int colNum = 4;
    ui.tb_users->setColumnCount(colNum);
    int nWidth = ui.tb_users->width() - colNum;
    for (int i=0; i<colNum; ++i)
    {
        ui.tb_users->setColumnWidth(i, nWidth / colNum);
    }
    //////////////////////////////////////////////////////////////////////////

    const list<TransferSet> &allSet = m_pParam->m_transetList.lsSets;
    if (allSet.size() == 0)
    {
        return;
    }
    list<TransferSet>::const_iterator it = allSet.begin();
    QString strTmp("");
    for (; it != allSet.end(); it++)
    {
        strTmp = QString::fromLocal8Bit(it->strName.c_str());	// 配置文件乱码时这样转换
        ui.listWidget_ID->addItem(strTmp);
    }

    ui.listWidget_ID->setCurrentRow(0);
    it = allSet.begin();
    showInfo(*it);
}

void TransferSetDlg::showInfo(const TransferSet &set)
{
    // 收集策略
    ui.le_taskName->setText(QString::fromLocal8Bit(set.strName.c_str()));
    ui.le_taskName->setEnabled(false);
    int id = 2;
    if (set.nCollectWay == 0)
    {
        id = set.nWatchWay;
    }
    m_collectWayGroup->button(id)->setChecked(true);
    onSelCollectWay(id);
    ui.le_path->setText(QString::fromLocal8Bit(set.strPath.c_str()));
    QStringList strDispatch = QString::fromStdString(set.strDispatch).split(" ");
    for (int i=0; i<strDispatch.count() && i<m_vecDispath.count(); i++)
    {
        m_vecDispath[i]->setText(strDispatch.at(i));
    }
    ui.le_usr->setText(QString::fromStdString(set.strUsr));
    ui.le_pwd->setText(QString::fromStdString(set.strPwd));
    ui.le_ip->setText(QString::fromStdString(set.strIP));
    ui.le_port->setText(QString::number(set.nPort));
    ui.list_model->clear();
    for (uint i=0; i<set.vecRegs.size(); ++i)
    {
        ui.list_model->addItem(QString::fromStdString(set.vecRegs.at(i)));
    }

    // 分发策略
    ui.tb_users->setRowCount(0);
    ui.tb_users->clearContents();
    for (uint i=0; i<set.vecRecvID.size(); ++i)
    {
        int row = i / 4;
        int col = i % 4;
        if (col == 0)
        {
            ui.tb_users->insertRow(row);
        }
        ui.tb_users->setItem(row, col, new QTableWidgetItem(QString::fromStdString(set.vecRecvID.at(i)) + ";"));
    }
}

void TransferSetDlg::onChangeTask(int rowNo)
{
    const list<TransferSet> &allSet = m_pParam->m_transetList.lsSets;
    if (rowNo < 0 || rowNo >= (int)allSet.size())
    {
        return;
    }

    list<TransferSet>::const_iterator it = allSet.begin();
    for (int i=0; i<rowNo; i++)
    {
        it++;
    }

    showInfo(*it);

    m_nTaskFlag = 2;
}

void TransferSetDlg::onSaveTask()
{
    if (m_nTaskFlag == 0)
    {
        return;
    }

    QString strTaskId = ui.le_taskName->text();
    // 判断是否合法
    if (m_nTaskFlag == 1)
    {
        if (strTaskId.trimmed().isEmpty())
        {
            QMessageBox::warning(this, QStringLiteral("错误提示"), QStringLiteral("任务名不能为空！"));
            return;
        }

        list<TransferSet>::iterator it = m_pParam->m_transetList.lsSets.begin();
        while (it != m_pParam->m_transetList.lsSets.end())
        {
            if (it->strName == strTaskId.toLocal8Bit().data())
            {
                QMessageBox::warning(this, QStringLiteral("错误提示"), QStringLiteral("任务名已存在！"));
                return;
            }
            it++;
        }
    }

    TransferSet set;
    set.strName = strTaskId.toLocal8Bit().data();
    set.strPath = ui.le_path->text().toLocal8Bit().data();
    int nWay = m_collectWayGroup->checkedId();
    if (nWay == 2)
    {
        set.nCollectWay = 1;
        //检查IP地址合理性
        QRegExp rx("^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$");
        bool matchResult= rx.exactMatch(ui.le_ip->text());
        if(!matchResult)
        {
            QMessageBox::warning(this, QStringLiteral("错误提示"), QStringLiteral("IP地址不合法！"));
            return;
        }
    }
    else
    {
        set.nCollectWay = 0;
        set.nWatchWay = nWay;
    }
    for (int i=0; i<ui.list_model->count(); ++i)
    {
        set.vecRegs.push_back(ui.list_model->item(i)->text().toStdString());
    }
    set.strUsr = ui.le_usr->text().toStdString();
    set.strPwd = ui.le_pwd->text().toStdString();
    set.strIP = ui.le_ip->text().toStdString();
    set.nPort = ui.le_port->text().toInt();
    string userId("");
    for (int i=0; i<ui.tb_users->rowCount(); ++i)
    {
        for (int j=0; j<ui.tb_users->columnCount(); ++j)
        {
            QTableWidgetItem *pItem = ui.tb_users->item(i, j);
            if (pItem != 0)
            {
                userId = pItem->text().remove(";").toStdString();
                SendSet userinfo;
                if (m_pParam->getUserInfo(userId, userinfo))
                {
                    set.vecRecvID.push_back(userId);
                    set.vecRcvers.push_back(userinfo);
                }

            }
        }
    }

    // 记录到配置文件
    if (m_nTaskFlag == 1)	// 新增
    {
        m_pParam->m_transetList.lsSets.push_back(set);
        ui.listWidget_ID->addItem(strTaskId);
        ui.listWidget_ID->setCurrentRow(ui.listWidget_ID->count() - 1);
    }
    else					// 编辑
    {
        list<TransferSet>::iterator it = m_pParam->m_transetList.lsSets.begin();
        while (it != m_pParam->m_transetList.lsSets.end())
        {
            if (it->strName == strTaskId.toLocal8Bit().data())
            {
                *it = set;
                break;
            }
            it++;
        }
    }

    m_tranSet = set;
    m_nTaskFlag = 0;

    m_pParam->saveTranSet();
}

void TransferSetDlg::onApplyTask()
{
    onSaveTask();

    QObject *obj = m_pManager->getService("IDataCollection");
    if (NULL != obj)
    {
        IDataCollection *pCollect = qobject_cast<IDataCollection*>(obj);
        pCollect->MdyTransferSet(m_tranSet);
    }
}

void TransferSetDlg::onSelUser()
{
    getUserID();
    SelUserDlg dlg(ui.le_taskName->text(), m_lstUserID, this);
    if (QDialog::Accepted == dlg.exec())
    {
        m_lstUserID = dlg.m_lstUserID;

        ui.tb_users->setRowCount(0);
        ui.tb_users->clearContents();
        for (int i=0; i<m_lstUserID.size(); ++i)
        {
            int row = i / 4;
            int col = i % 4;
            if (col == 0)
            {
                ui.tb_users->insertRow(row);
            }
            ui.tb_users->setItem(row, col, new QTableWidgetItem(m_lstUserID.at(i) + ";"));
        }
    }
}

void TransferSetDlg::getUserID()
{
    m_lstUserID.clear();
    for (int i=0; i<ui.tb_users->rowCount(); ++i)
    {
        for (int j=0; j<ui.tb_users->columnCount(); ++j)
        {
            QTableWidgetItem *pItem = ui.tb_users->item(i, j);
            if (pItem != 0)
            {
                QString strTmp = pItem->text().remove(";");
                m_lstUserID.append( strTmp);
            }
        }
    }
}

void TransferSetDlg::onBtnAddTask()
{
    clearAllInfo();
    m_nTaskFlag = 1;
}

void TransferSetDlg::onBtnDelTask()
{
    const list<TransferSet> &allSet = m_pParam->m_transetList.lsSets;
    int nRowNo = ui.listWidget_ID->currentRow();
    if (nRowNo < 0 && nRowNo <=allSet.size())
    {
        return;
    }
}

void TransferSetDlg::clearAllInfo()
{
    ui.le_taskName->clear();
    ui.le_taskName->setEnabled(true);
    m_collectWayGroup->button(0)->setChecked(true);
    ui.le_path->clear();
    for (int i=1; i<m_vecDispath.count(); i++)
    {
        m_vecDispath[i]->setText("*");
    }
    m_vecDispath[0]->setText("0");
    ui.le_usr->clear();
    ui.le_pwd->clear();
    ui.le_ip->clear();
    ui.le_port->clear();
    ui.list_model->clear();
    ui.tb_users->setRowCount(0);
    ui.tb_users->clearContents();
}

void TransferSetDlg::onSelCollectWay(int id)
{
    bool bEnable = true;
    if (id == 1)
    {
        bEnable = false;
    }
    for (int i=0; i<m_vecDispath.size(); ++i)
    {
        m_vecDispath.at(i)->setEnabled(bEnable);
    }
}

void TransferSetDlg::onBtnAddModel()
{
    QStringList lstModes;
    for (int i=0; i<ui.list_model->count(); ++i)
    {
        lstModes.append(ui.list_model->item(i)->text());
    }
    AddModelDlg dlg(lstModes);
    if (QDialog::Accepted == dlg.exec())
    {
        ui.list_model->clear();
        m_lstModels = dlg.m_lstModels;
        for (int i=0; i<m_lstModels.size(); ++i)
        {
            ui.list_model->addItem(m_lstModels.at(i));
        }
    }
}
