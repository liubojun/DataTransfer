#include "sendPrdcSetDlg.h"
#include "selUserDlg.h"
#include "ICtkPluginManager.h"
#include "IRadarPathAdapter.h"
#include "IFileDistribute.h"
#include "IDataCollection.h"
#include "commontypedef.h"
#include "ctkLog.h"
#include "CollectManager.h"
#include "event_macro.h"
#include <QMessageBox>

const int colNum = 3;

SendPrdcSetDlg::SendPrdcSetDlg(ICtkPluginManager *pManager, QWidget *parent/*=0*/)
    : m_nTaskFlag(0)
    , m_pManager(pManager)
    , m_pDataCollectionSev(NULL)
    , m_pFileTransfer(NULL)
{
    m_pParam = TransParam::getInstance();
    ui.setupUi(this);
    connect(ui.listWidget_ID, SIGNAL(currentRowChanged(int)), this, SLOT(onChangeTask(int)));
    //connect(ui.btn_apply, SIGNAL(clicked()), this, SLOT(onApplyTask()));
    connect(ui.tBtn_selUser, SIGNAL(clicked()), this, SLOT(onSelUser()));
    connect(ui.btn_add, SIGNAL(clicked()), this, SLOT(onBtnAddTask()));
    connect(ui.btn_del, SIGNAL(clicked()), this, SLOT(onBtnDelTask()));
    m_pThread = QSharedPointer<QThread>(new QThread);

    QObject *op = m_pManager->getService("IRadarPathAdapter");
    m_pMakePath = (qobject_cast<IRadarPathAdapter *>(op))->createRadarProductPath();

    op = m_pManager->getService("IFileDistribute");
    m_pFileTransfer = qobject_cast<IFileDistribute *>(op);
    m_pDistributer = m_pFileTransfer->createDistributeManager();
}

void SendPrdcSetDlg::initDlg()
{
    ui.listWidget_ID->clear();
    m_vecCheckBox.clear();
    m_vecCheckBox.append(ui.checkBox_1);
    m_vecCheckBox.append(ui.checkBox_2);
    m_vecCheckBox.append(ui.checkBox_3);
    m_vecCheckBox.append(ui.checkBox_4);
    m_vecCheckBox.append(ui.checkBox_5);
    m_vecCheckBox.append(ui.checkBox_6);
    m_vecCheckBox.append(ui.checkBox_7);
    m_vecCheckBox.append(ui.checkBox_8);

    // 禁止编辑
    ui.tb_users->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tb_users->setColumnCount(colNum);
    int nWidth = ui.tb_users->width() - colNum;
    for (int i=0; i<colNum; ++i)
    {
        ui.tb_users->setColumnWidth(i, nWidth / colNum);
    }

    const list<PrdcTransferSet> &allSet = m_pParam->m_prdcTransetList.lst;
    if (allSet.size() == 0)
    {
        m_nTaskFlag = 1;
        return;
    }
    list<PrdcTransferSet>::const_iterator it = allSet.begin();
    QString strTmp("");
    for (; it != allSet.end(); it++)
    {
        strTmp = QString::fromStdString(it->strID);
        ui.listWidget_ID->addItem(strTmp);
    }

    ui.listWidget_ID->setCurrentRow(0);
    it = allSet.begin();
    showInfo(*it);
}

void SendPrdcSetDlg::showInfo(const PrdcTransferSet &set)
{
    if (set.strID.empty())
    {
        return;
    }
    ui.le_taskName->setText(QString::fromStdString(set.strID));
    ui.le_taskName->setEnabled(false);
    ui.checkBox_1->setChecked(set.bRadarData);
    ui.checkBox_2->setChecked(set.bMosaicData);
    ui.checkBox_3->setChecked(set.bPupData);
    ui.checkBox_4->setChecked(set.bHighData);
    ui.checkBox_5->setChecked(set.bRadarPic);
    ui.checkBox_6->setChecked(set.bMosaicPic);
    ui.checkBox_7->setChecked(set.bPupPic);
    ui.checkBox_8->setChecked(set.bHighPic);
    ui.tb_users->setRowCount(0);
    ui.tb_users->clearContents();
    for (uint i=0; i<set.vecRecvID.size(); ++i)
    {
        int row = i / colNum;
        int col = i % colNum;
        if (col == 0)
        {
            ui.tb_users->insertRow(row);
        }
        ui.tb_users->setItem(row, col, new QTableWidgetItem(QString::fromStdString(set.vecRecvID.at(i))));
    }
    m_nTaskFlag = 2;
}

void SendPrdcSetDlg::onChangeTask(int rowNo)
{
    const list<PrdcTransferSet> &allSet = m_pParam->m_prdcTransetList.lst;
    if (rowNo < 0 || rowNo >= (int)allSet.size())
    {
        clearAllInfo();
        return;
    }

    list<PrdcTransferSet>::const_iterator it = allSet.begin();
    for (int i=0; i<rowNo; i++)
    {
        it++;
    }

    showInfo(*it);

    m_nTaskFlag = 2;
}

void SendPrdcSetDlg::onApplyTask()
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
            QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("任务名不能为空！"));
            return;
        }

        list<PrdcTransferSet>::iterator it = m_pParam->m_prdcTransetList.lst.begin();
        while (it != m_pParam->m_prdcTransetList.lst.end())
        {
            if (it->strID == strTaskId.toStdString())
            {
                QMessageBox::critical(this, QStringLiteral("提示"), QStringLiteral("任务名已存在！"));
                return;
            }
            it++;
        }
    }

    PrdcTransferSet set;
    set.strID = strTaskId.toStdString();
    set.bRadarData = ui.checkBox_1->isChecked();
    set.bMosaicData = ui.checkBox_2->isChecked();
    set.bPupData = ui.checkBox_3->isChecked();
    set.bHighData = ui.checkBox_4->isChecked();
    set.bRadarPic = ui.checkBox_5->isChecked();
    set.bMosaicPic = ui.checkBox_6->isChecked();
    set.bPupPic = ui.checkBox_7->isChecked();
    set.bHighPic = ui.checkBox_8->isChecked();
    string userId("");
    for (int i=0; i<ui.tb_users->rowCount(); ++i)
    {
        for (int j=0; j<ui.tb_users->columnCount(); ++j)
        {
            QTableWidgetItem *pItem = ui.tb_users->item(i, j);
            if (pItem != 0)
            {
                userId = pItem->text().toStdString();
                set.vecRecvID.push_back(userId);
            }
        }
    }

    // 记录到配置文件
    if (m_nTaskFlag == 1)	// 新增
    {
        m_pParam->m_prdcTransetList.lst.push_back(set);
        ui.listWidget_ID->addItem(strTaskId);
        ui.listWidget_ID->setCurrentRow(ui.listWidget_ID->count() - 1);
    }
    else					// 编辑
    {
        list<PrdcTransferSet>::iterator it = m_pParam->m_prdcTransetList.lst.begin();
        while (it != m_pParam->m_prdcTransetList.lst.end())
        {
            if (it->strID == strTaskId.toStdString())
            {
                *it = set;
                break;
            }
            it++;
        }
    }

    m_tranSet = set;
    m_nTaskFlag = 0;

    m_pParam->savePrdcTranSet();

    // 启动收集分发
    startTask(set);
}

void SendPrdcSetDlg::onBtnDelTask()
{
    list<PrdcTransferSet> &allSet = m_pParam->m_prdcTransetList.lst;
    int nRowNo = ui.listWidget_ID->currentRow();
    if (nRowNo < 0 || nRowNo >= (int)allSet.size())
    {
        QSLOG_DEBUG("nRowNo is out of range.");
        return;
    }
    if (QMessageBox::No == QMessageBox::warning(this, QStringLiteral("系统提示"), QStringLiteral("你确定要删除该任务吗？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
    {
        return;
    }
    list<PrdcTransferSet>::iterator it = allSet.begin();
    for (int i=0; i<nRowNo; ++i)
    {
        it++;
    }
    QString strID = ui.listWidget_ID->currentItem()->text();
    stopTask(strID);
    allSet.erase(it);
    m_pParam->savePrdcTranSet();
    ui.listWidget_ID->takeItem(nRowNo);
    ui.listWidget_ID->setCurrentRow(0);
}

void SendPrdcSetDlg::onBtnAddTask()
{
    clearAllInfo();
    m_nTaskFlag = 1;
    ui.listWidget_ID->setCurrentRow(-1);
}

void SendPrdcSetDlg::onSelUser()
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
            int row = i / colNum;
            int col = i % colNum;
            if (col == 0)
            {
                ui.tb_users->insertRow(row);
            }
            ui.tb_users->setItem(row, col, new QTableWidgetItem(m_lstUserID.at(i)));
        }
    }
}

void SendPrdcSetDlg::getUserID()
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

void SendPrdcSetDlg::showEvent(QShowEvent *e)
{
    initDlg();
    return QWidget::showEvent(e);
}

bool SendPrdcSetDlg::event(QEvent *e)
{
    if (e->type() == ApplyEvent)
    {
        onApplyTask();
        return true;
    }
    else if (e->type() == OkEvent)
    {
        onApplyTask();
        emit ok();
        return true;
    }
    else if (e->type() == CancelEvent)
    {
        cancel();
        return true;
    }

    return QWidget::event(e);
}

void SendPrdcSetDlg::clearAllInfo()
{
    ui.le_taskName->clear();
    ui.le_taskName->setEnabled(true);
    for (int i=0; i<m_vecCheckBox.size(); ++i)
    {
        m_vecCheckBox[i]->setChecked(false);
    }
    ui.tb_users->setRowCount(0);
    ui.tb_users->clearContents();
}

void SendPrdcSetDlg::startTask(const PrdcTransferSet &set)
{
    if (m_pDataCollectionSev == NULL)
    {
        QObject *obj = m_pManager->getService("IDataCollection");
        if (NULL != obj)
        {
            m_pDataCollectionSev = qobject_cast<IDataCollection*>(obj);
            if (m_pDataCollectionSev != NULL)
            {
                m_pCollector = m_pDataCollectionSev->createCollector();
                QObject::connect(m_pCollector.data(), SIGNAL(newTransFile(const TransferSet &)), this, SLOT(TransFile(const TransferSet &)));
            }
        }
    }

    if (!m_pCollector.isNull())
    {
        QVector<string> vecNewID;
        // 转换为TransferSet，ID也不同
        QVector<TransferSet> tSets = changeToTransferSet(set);
        for (int i=0; i<tSets.size(); ++i)
        {
            const TransferSet &tSet = tSets.at(i);
            m_pCollector->addTransCollect(tSet);
            vecNewID.append(tSet.strName);
        }
        m_mapCollectID.insert(QString::fromStdString(set.strID), vecNewID);
    }
}

void SendPrdcSetDlg::stopTask(const QString &strID)
{
    if (m_mapCollectID.contains(strID))
    {
        QVector<string> vecCollects = m_mapCollectID.take(strID);
        for (int i=0; i<vecCollects.size(); i++)
        {
            m_pCollector->delTransCollect(vecCollects.at(i));
        }
    }
}

QVector<TransferSet> SendPrdcSetDlg::changeToTransferSet(const PrdcTransferSet &set)
{
    QVector<TransferSet> vecRes;
    QStringList lstUrlPath;
    QString strTmp;
    if (set.bPupData)
    {
        strTmp = m_pMakePath->getFileRootPath(PupType, false);
        lstUrlPath.append(strTmp);
    }
    if (set.bPupPic)
    {
        strTmp = m_pMakePath->getFileRootPath(PupType, true);
        lstUrlPath.append(strTmp);
    }
    if (set.bMosaicData)
    {
        strTmp = m_pMakePath->getFileRootPath(MosaciType, false);
        lstUrlPath.append(strTmp);
    }
    if (set.bMosaicPic)
    {
        strTmp = m_pMakePath->getFileRootPath(MosaciType, true);
        lstUrlPath.append(strTmp);
    }
    if (set.bHighData)
    {
        strTmp = m_pMakePath->getFileRootPath(HighType, false);
        lstUrlPath.append(strTmp);
    }
    if (set.bHighPic)
    {
        strTmp = m_pMakePath->getFileRootPath(HighType, true);
        lstUrlPath.append(strTmp);
    }

    vector<SendSet> userInfo;
    for (uint i=0; i<set.vecRecvID.size(); ++i)
    {
        SendSet user;
        if (m_pParam->getUserInfo(set.vecRecvID.at(i), user))
        {
            userInfo.push_back(user);
        }
    }

    for (int i=0; i<lstUrlPath.size(); ++i)
    {
        QString strPath = lstUrlPath.at(i).trimmed();
        if (strPath.isEmpty())
        {
            continue;
        }
        TransferSet tset;
        tset.strName = set.strID + QString("_%1").arg(i).toStdString();
        tset.strPath = strPath.toStdString();
        tset.nCollectWay = 0;
        tset.nWatchWay = 1;
        tset.vecRecvID = set.vecRecvID;
        tset.vecRcvers = userInfo;
        tset.vecRegs.push_back(".*");
        vecRes.append(tset);
    }

    return vecRes;
}

void SendPrdcSetDlg::TransFile(const TransferSet &tset)
{
    if (!m_pDistributer.isNull())
    {
        m_pDistributer->AddSendTask(tset);
    }
}
