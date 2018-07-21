#include "collectSetDlg.h"
#include "userItem.h"
#include "regexpression.h"
#include "pathbuilder.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QPalette>
#include <QTimer>
#include <curl/curl.h>
#include <QDebug>
#include <QThread>
#include "QsLog/ctkLog.h"
#include "renameDlg.h"
#include "change_name.h"
#include "subdirfilter.h"
#include "subdirtemplateedit.h"
#include "LibCurlSFtp.h"
#include "LibCurlFtp.h"
//#include "curlftp.h"

CollectSetDlg::CollectSetDlg(int flag, QDialog *parent /*= NULL*/)
    : m_flag(flag)
    , QDialog(parent)

{
    ui.setupUi(this);
    InitUI();
}

CollectSetDlg::~CollectSetDlg()
{

}

void CollectSetDlg::InitUI()
{
//     ui.tableWidget->setColumnCount(3);
//     ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
//     //ui.tableWidget->resizeColumnsToContents();
//     ui.tableWidget->setColumnWidth(0, 100);
//     ui.tableWidget->setColumnWidth(1, 120);
//     ui.tableWidget->resizeRowsToContents();
//     ui.tableWidget->setRowCount(1);

    // 新增
    if (m_flag == 0)
    {
        ui.radFile->setChecked(true);
        onSelFile(true);
        QString dirName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        dirName = QStringLiteral("FILE传输-") + dirName;
        ui.le_DirName->setText(dirName);
        int sRand = qrand() % 60;
        QString timeR = QString::number(sRand) + " * * * * *";
        ui.lineEdit_3->setText(timeR);
        ui.lineEdit_4->setText(".*");

		ui.comboBox_timerule->setCurrentIndex(0);
    }
	ui.label_result->setText("");
    //ui.checkBox_6->setVisible(false);
    connect(ui.btnApply, SIGNAL(clicked()), this, SLOT(onApply()));
    connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
    connect(ui.btnBrowse, SIGNAL(clicked()), this, SLOT(onBrows()));
    connect(ui.radFile, SIGNAL(toggled(bool)), this, SLOT(onSelFile(bool)));
    //connect(ui.btnSelUser, SIGNAL(clicked()), this, SLOT(selUser()));
    connect(ui.btn_test, SIGNAL(clicked()), this, SLOT(onRegExpTest()));
	connect(ui.btn_testSource, SIGNAL(clicked()), this, SLOT(onWaitForTestSource()));
	connect(ui.btn_testDest, SIGNAL(clicked()), this, SLOT(onWaitForTestDest()));
    //connect(ui.btn_test_2, SIGNAL(clicked()), this, SLOT(onRemoteColTest()));
    connect(this, SIGNAL(testok(const QString &)), this, SLOT(onTestOk(const QString &)));
    connect(this, SIGNAL(testfail(const QString &)), this, SLOT(onTestFail(const QString &)));
    connect(ui.btn_rename_edit, SIGNAL(clicked()), this, SLOT(onRenameRuleEdit()));
    connect(ui.comboBox_sendUser, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onSendUserChange(const QString &)));
    connect(ui.compare_content, SIGNAL(stateChanged(int)), this, SLOT(onCompareContent(int)));

    connect(ui.pbt_subdirfilter, SIGNAL(clicked()), this, SLOT(onSubDirFilterEdit()));
    // added by liubojun @20171112,支持分发换名
    std::vector<std::string> rules = CChangeName::get_rules();

    for (size_t i = 0; i < rules.size(); ++i)
    {
        ui.comboBox_rename->addItem(QString::fromLocal8Bit(rules[i].c_str()));
    }

    // 子目录过滤
    CSubDirFilter oSubDirFilter;
    oSubDirFilter.init();
    QStringList strRules = oSubDirFilter.getAllRuleId();

    foreach(QString strRule, strRules)
    {
        ui.cbx_subdirfilter->addItem(strRule);
    }

    // 从数据库读取分发用户
    QList<UserInfo> lstUser;
    if (!DataBase::getInstance()->QueryUserInfo(lstUser) || lstUser.size() == 0)
    {
        return;
    }
    int nIndex = 0;
    for (int i = 0; i < lstUser.size(); ++i)
    {
        nIndex++;
        ui.comboBox_sendUser->addItem(lstUser.at(i).userName);
        //m_iUser[nIndex] = lstUser.at(i).userID;
    }
}

void CollectSetDlg::selUser()
{
//     if (m_flag == 0)
//     {
//         m_pSelUserDlg = QSharedPointer<SelUserDlg>(new SelUserDlg());
//     }
//     else
//     {
//         m_pSelUserDlg = QSharedPointer<SelUserDlg>(new SelUserDlg(m_flag, m_selUser));
//     }
    m_pSelUserDlg = QSharedPointer<SelUserDlg>(new SelUserDlg());

    //if (QDialog::Accepted == m_pSelUserDlg->exec())
    //{
    //    // m_selUser.lstUser.append(m_pSelUserDlg->m_selUser);

    //    QListWidgetItem *pItem = new QListWidgetItem();
    //    ui.listWidget->addItem(pItem);
    //    UserItem *pUser = new UserItem(ui.listWidget, m_pSelUserDlg->m_selUser.user.userID, m_pSelUserDlg->m_selUser.user.userName, m_pSelUserDlg->m_selUser.rltvPath);
    //    connect(pUser, SIGNAL(onDelete(const QString &)), this, SLOT(onSendUserDel(const QString &)));
    //    ui.listWidget->setItemWidget(pItem, pUser);
    //    pItem->setSizeHint(QSize(pUser->rect().width(), pUser->rect().height()));
    //}
}

bool CollectSetDlg::onApply()
{
    CollectTask &cSet = m_task;
    cSet.dirName = ui.le_DirName->text();
    if (ui.radFile->isChecked())
    {
        cSet.collectType = 0;
    }
    else if (ui.radFtp->isChecked())
    {
        cSet.collectType = 1;
    }
    else
    {
        cSet.collectType = 2;
    }
    //cSet.collectType = (ui.radFile->isChecked()) ?  0 : 1;
    cSet.ftp_transferMode = ui.comboBox_bin->currentIndex();
    cSet.ftp_connectMode = ui.comboBox_passive->currentIndex();
    cSet.rltvPath = ui.le_RelvPath->text();
    cSet.dispatch = ui.lineEdit_3->text();
    cSet.fileTemplate = ui.lineEdit_4->text();
    cSet.subdirFlag = (ui.checkBox_2->isChecked()) ? 1 : 0;
//    cSet.moveFlag = (ui.checkBox_6->isChecked()) ? 1 : 0;
    // cSet.col_timerange = (ui.col_time_range)
    // 0:所有，1:30分钟，2：1小时，3：2小时，4：3小时，5：6小时，6：12小时，7：1天，8：2天，9：3天

    int tmin = 0;
    // 匹配数字部分
    QString strTime = ui.col_time_range->currentText();
    if (strTime != QStringLiteral("所有"))
    {
        QRegExp oReg("([0-9]{1,})(.*)");
        int pos = oReg.indexIn(strTime);
        if (-1 == pos)
        {
            QMessageBox::warning(this, QStringLiteral("收集策略检查"), QStringLiteral("收集时效配置不正确"));
            return false;
        }
        QString strTimeUnit = oReg.cap(2);
        if (QStringLiteral("天") == strTimeUnit)
        {
            tmin = 60 * 24;
        }
        else if (QStringLiteral("小时") == strTimeUnit)
        {
            tmin = 60;
        }
        else if (QStringLiteral("分钟") == strTimeUnit)
        {
            tmin = 1;
        }
        else
        {
            QMessageBox::warning(this, QStringLiteral("收集策略检查"), QStringLiteral("收集时效配置不正确"));
            return false;
        }
        tmin = oReg.cap(1).toInt() * tmin;
    }
    else
    {
        tmin = -1;
    }



    cSet.col_timerange = tmin;
    cSet.loginUser = ui.lineEdit_5->text();
    cSet.loginPass = ui.lineEdit_6->text();
    cSet.ip = ui.lineEdit_7->text();
    cSet.port = ui.lineEdit_8->text().toInt();
    cSet.recordLatestTime = ui.recordLatestTime->isChecked() ? 1 : 0;
    cSet.compareContent = ui.compare_content->isChecked() ? 1 : 0;
    QMessageBox box(QMessageBox::Critical, QStringLiteral("提示"), "");
    box.setStandardButtons(QMessageBox::Ok);
    box.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
    if (cSet.dirName.isEmpty())
    {
        box.setText(QStringLiteral("任务名不能为空！"));
        box.exec();
        return false;
    }

    //int size = ui.listWidget->count();
    //if (size <= 0)
    //{
    //    box.setText(QStringLiteral("未配置分发用户！"));
    //    box.exec();
    //    return false;
    //}
    if (m_flag == 0)	// 新增
    {
        cSet.dirID = QUuid::createUuid().toString();
    }
    //else				// 修改
    //{
    //    cSet.dirID = m_selUser.dirID;
    //}
    //m_selUser.lstUser.clear();

    //UserItem *pUserItem = (UserItem *) ui.listWidget->itemWidget(ui.listWidget->item(0));

    CollectUser oSendUser = getSendUserInfoFromName(ui.comboBox_sendUser->currentText());
    oSendUser.rename_rule = ui.comboBox_rename->currentText();
	oSendUser.bKeepDir = ui.checkBox_keepSource->isChecked();
	oSendUser.rltvPath = ui.lineEdit_rtvpath->text();
	oSendUser.iTimeRule = ui.comboBox_timerule->currentIndex();
	m_selUser.sendUser = oSendUser;

    cSet.subDirTemplate = ui.cbx_subdirfilter->currentText();

    //// 临时屏蔽
    //// 更新收集用户表
    m_selUser.taskID = cSet.dirID;
    DataBase::getInstance()->InsertCollectUser(m_selUser);
    // 查询用户信息
    //DataBase::getInstance()->QueryUserInfo(m_selUser);

    accept();
    emit commit(cSet);

    return true;
}

void CollectSetDlg::onCancel()
{
    reject();
    // close();
}

void CollectSetDlg::onBrows()
{
    QString path;
    path = QFileDialog::getExistingDirectory(this, QStringLiteral("选择源目录"), ui.le_RelvPath->text());
    if (!path.isEmpty())
    {
        ui.le_RelvPath->setText(path);
        ui.le_RelvPath->setCursorPosition(-1);
    }
}

void CollectSetDlg::onSelFile(bool bFile)
{
    if (bFile)
    {
        ui.label_3->setText(QStringLiteral("收集路径："));

    }
    else
    {
        ui.label_3->setText(QStringLiteral("远程路径："));
    }
    ui.comboBox_bin->setVisible(!bFile);
    ui.comboBox_passive->setVisible(!bFile);

    ui.btnBrowse->setEnabled(bFile);
    ui.lineEdit_5->setEnabled(!bFile);
    ui.lineEdit_6->setEnabled(!bFile);
    ui.lineEdit_7->setEnabled(!bFile);
    ui.lineEdit_8->setEnabled(!bFile);

    CollectUser oSendUser = getSendUserInfoFromName(ui.comboBox_sendUser->currentText());
    // 当收集目录以及分发地址均是使用文件目录时，启用文件比对功能
    if (FILE_TYPE == oSendUser.user.sendType && bFile)
    {
        ui.compare_content->setVisible(true);
    }
    else
    {
        ui.compare_content->setVisible(false);
    }
}

void CollectSetDlg::showTask(const CollectTask &task)
{
    if (m_flag == 0)
    {
        return;
    }

    ui.le_DirName->setText(task.dirName);
    bool bFile = true;
    if (task.collectType == 0)
    {
        ui.radFile->setChecked(true);
        bFile = true;
    }
    else if (task.collectType == 1)
    {
        ui.radFtp->setChecked(true);
        bFile = false;
    }
    else
    {
        ui.radSFtp->setChecked(true);
        bFile = false;
    }
    onSelFile(bFile);


    ui.comboBox_bin->setCurrentIndex(task.ftp_transferMode);

    ui.comboBox_passive->setCurrentIndex(task.ftp_connectMode);


    ui.le_RelvPath->setText(task.rltvPath);
    ui.lineEdit_3->setText(task.dispatch);
    ui.lineEdit_4->setText(task.fileTemplate);
    ui.lineEdit_7->setText(task.ip);
    ui.lineEdit_8->setText(QString::number(task.port));
    ui.lineEdit_5->setText(task.loginUser);
    ui.lineEdit_6->setText(task.loginPass);
    ui.checkBox_2->setChecked(task.subdirFlag);
//    ui.checkBox_6->setChecked(task.moveFlag);




    // 0:所有，1:30分钟，2：1小时，3：2小时，4：3小时，5：6小时，6：12小时，7：1天，8：2天，9：3天
    int index = 0;
    bool enterUserDefineItem = false;
    switch (task.col_timerange)
    {
    case -1:
        index = 0;
        break;
    case 30:
        index = 1;
        break;
    case 60:
        index = 2;
        break;
    case 60*2:
        index = 3;
        break;
    case 60*3:
        index = 4;
        break;
    case 60*6:
        index = 5;
        break;
    case 60*12:
        index = 6;
        break;
    case 60*24:
        index = 7;
        break;
    case 60*48:
        index = 8;
        break;
    case 60*72:
        index = 9;
        break;
    default:
        enterUserDefineItem = true;
        break;
    }

    if (enterUserDefineItem)
    {
        ui.col_time_range->setCurrentText(QString("%1").arg(task.col_timerange) + QStringLiteral("分钟"));
    }
    else
    {
        ui.col_time_range->setCurrentIndex(index);
    }


    ui.recordLatestTime->setChecked(task.recordLatestTime);

    m_task.dirID = task.dirID;
    // 临时屏蔽
//     m_selUser.dirID = task.dirID;
//     DataBase::getInstance()->QueryUserInfo(m_selUser);
//     QTableWidgetItem *pItem = new QTableWidgetItem(QStringLiteral("分发用户："));
//     ui.tableWidget->setItem(0, 0, pItem);
//     pItem = new QTableWidgetItem(m_selUser.user.userName);
//     ui.tableWidget->setItem(0, 1, pItem);
//     pItem = new QTableWidgetItem(m_selUser.rltvPath);
//     ui.tableWidget->setItem(0, 2, pItem);

    CollectUser sendUser = getSendUserInfoFromDirID(task.dirID);
    ui.comboBox_sendUser->setCurrentText(sendUser.user.userName);
    ui.comboBox_rename->setCurrentText(sendUser.rename_rule);
	ui.lineEdit_rtvpath->setText(sendUser.rltvPath);
	ui.comboBox_timerule->setCurrentIndex(sendUser.iTimeRule);
	ui.checkBox_keepSource->setChecked(sendUser.bKeepDir);
    ui.compare_content->setChecked(task.compareContent);
    //for (int i=0; i<tUser.lstUser.size(); ++i)
    //{
    //    const CollectUser &cUser = tUser.lstUser.at(i);
    //    QListWidgetItem *pItem = new QListWidgetItem();
    //    ui.listWidget->addItem(pItem);

    //    UserItem *pUser = new UserItem(ui.listWidget, cUser.user.userID, cUser.user.userName, cUser.rltvPath);
    //    connect(pUser, SIGNAL(onDelete(const QString &)), this, SLOT(onSendUserDel(const QString &)));
    //    ui.listWidget->setItemWidget(pItem, pUser);
    //    pItem->setSizeHint(QSize(pUser->rect().width(), pUser->rect().height()));
    //}

    ui.cbx_subdirfilter->setCurrentText(task.subDirTemplate);
}

void CollectSetDlg::onSendUserDel(const QString &user)
{
    //if (!m_task.dirID.isEmpty())
    //{
    //    for (int i = 0; i < m_selUser.lstUser.size(); ++i)
    //    {
    //        const CollectUser &oUser = m_selUser.lstUser.at(i);
    //        if (oUser.user.userID == user)
    //        {
    //            m_selUser.lstUser.removeAt(i);
    //            break;
    //        }
    //    }
    //    DataBase::getInstance()->DeleteCollectSenderUser(m_task.dirID, user);
    //}

    //ui.listWidget->takeItem(0);
}

void CollectSetDlg::onRegExpTest()
{
    CRegExpDlg oRegDlg;
    oRegDlg.exec();
}

CollectUser CollectSetDlg::getSendUserInfoFromName(const QString &name)
{
    CollectUser retUser;
    QList<UserInfo> lstUser;
    if (!DataBase::getInstance()->QueryUserInfo(lstUser) || lstUser.size() == 0)
    {
        return retUser;
    }
    for (int i = 0; i < lstUser.size(); ++i)
    {
        if (name == lstUser[i].userName)
        {
            retUser.user = lstUser[i];
            break;
        }
    }

    return retUser;
}

CollectUser CollectSetDlg::getSendUserInfoFromDirID(const QString &CollectDirId)
{
    CollectUser retUser;

    TaskUser user;
    user.taskID = CollectDirId;
    if (!DataBase::getInstance()->QueryUserInfo(user))
    {
        return retUser;
    }

    //if (user.lstUser.size() > 0)
    //{
    //    retUser = user.lstUser[0];
    //}
	retUser = user.sendUser;

    return retUser;
}
/// 在内存中缓存文件内容（加密压缩需要）
//struct MemoryData
//{
//    char *memdata;		///< 起始地址
//    size_t size;		///< 有效大小
//    size_t capacity;	///< 申请的空间大小
//
//    MemoryData()
//    {
//        memdata = NULL;
//        size = 0;
//        capacity = 0;
//    }
//
//    ~MemoryData()
//    {
//        free(memdata);
//    }
//};
//static size_t WriteInMemoryFun(void *contents, size_t size, size_t nmemb, void *userp)
//{
//    size_t realsize = size * nmemb;		//本次回调获取的数据
//    struct MemoryData *mem = (struct MemoryData *)userp;
//
//    mem->memdata = (char *)realloc(mem->memdata, mem->size + realsize + 1);
//    if (mem->memdata == NULL)
//    {
//        /* out of memory! */
//        printf("not enough memory (realloc returned NULL)\n");
//        return 0;
//    }
//
//    memcpy(&(mem->memdata[mem->size]), contents, realsize);
//    mem->size += realsize;
//    mem->memdata[mem->size] = 0;
//
//    return realsize;
//}

void CollectSetDlg::onRemoteColTest()
{
    if (ui.radFile->isChecked())
    {
        QStringList retUrls = CPathBuilder::getFinalPathFromUrl(ui.le_RelvPath->text());
        foreach(QString strUrl, retUrls)
        {
            //QSLOG_DEBUG(strUrl);
            QUrl url = QUrl::fromLocalFile(strUrl);
            if (url.isLocalFile())
            {
                //QSLOG_DEBUG("==============");
                QDir qdir(strUrl);
                if (qdir.exists())
                {
                    emit testok(strUrl);
                    //QSLOG_DEBUG("==============111111111111");
                    //return;
                }
                else
                {
                    emit testfail(strUrl);
                    //QSLOG_DEBUG("==============22222222222");
                }
            }
        }


    }
    else
    {
        QSharedPointer<FtpBase> pFtpBase;
        char url[256] = { 0 };
        if (ui.radFtp->isChecked())
        {
            pFtpBase = QSharedPointer<FtpBase>(new CFtp());

        }
        else
        {
            pFtpBase = QSharedPointer<FtpBase>(new SFtp());
        }
        QStringList retUrls = CPathBuilder::getFinalPathFromUrl(ui.le_RelvPath->text());
        foreach(QString strUrl, retUrls)
        {

            // 使用主动，默认为被动
            if (1 == ui.comboBox_passive->currentIndex())
            {
                pFtpBase->setTransferMode(Active);
            }

            sprintf(url, "ftp://%s:%d%s", ui.lineEdit_7->text().toStdString().c_str(), ui.lineEdit_8->text().toInt(), strUrl.toLocal8Bit().data());
            //sprintf(usrPwd, "%s:%s", ui.lineEdit_5->text().toStdString().c_str(), ui.lineEdit_6->text().toStdString().c_str());
            pFtpBase->connectToHost(ui.lineEdit_7->text(), ui.lineEdit_8->text().toInt());
            pFtpBase->login(ui.lineEdit_5->text(), ui.lineEdit_6->text());
            if (CURLE_OK != pFtpBase->cd(strUrl))
            {
                emit testfail(QString(url));
            }
            else
            {
                emit testok(QString(url));
            }
        }
    }
}

void CollectSetDlg::onRemoteDestTest()
{
	// 查询分发用户的基本信息
	CollectUser oSendUser = getSendUserInfoFromName(ui.comboBox_sendUser->currentText());

	// 0-FILE，1-FTP, 2-SFTP
	if (0 == oSendUser.user.sendType)
	{
		QStringList retUrls = CPathBuilder::getFinalPathFromUrl(oSendUser.user.rootPath + "/" + ui.lineEdit_rtvpath->text());
		//QStringList retUrls = CPathBuilder::getFinalPathFromUrl(ui.le_RelvPath->text());
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
		QSharedPointer<FtpBase> pFtpBase;
		//char url[256] = { 0 };
		if (1 == oSendUser.user.sendType)
		{
			pFtpBase = QSharedPointer<FtpBase>(new CFtp());

		}
		else
		{
			pFtpBase = QSharedPointer<FtpBase>(new SFtp());
		}
		QStringList retUrls = CPathBuilder::getFinalPathFromUrl(oSendUser.user.rootPath + "/" + ui.lineEdit_rtvpath->text());
		foreach(QString strUrl, retUrls)
		{

			// 使用主动，默认为被动
			if (1 == ui.comboBox_passive->currentIndex())
			{
				pFtpBase->setTransferMode(Active);
			}
			QString url = QString("ftp://%1:%2").arg(oSendUser.user.ip).arg(oSendUser.user.port);
			//sprintf(url, "ftp://%s:%d", oSendUser.user.ip.toStdString().c_str(), oSendUser.user.port);
			pFtpBase->connectToHost(oSendUser.user.ip, oSendUser.user.port);
			if (CURLE_OK != pFtpBase->login(oSendUser.user.lgUser, oSendUser.user.lgPass))
			{
				emit testfail(url);
			}
			else
			{
				emit testok(url);
			}
		}
	}
}

void CollectSetDlg::onTestOk(const QString &url)
{
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::green);
    ui.label_result->setPalette(pal);
    ui.label_result->setText(QStringLiteral("连接[") + url + QStringLiteral("]成功"));
    QTimer::singleShot(3000, this, SLOT(onTestResultTimeout()));
}

void CollectSetDlg::onTestFail(const QString &url)
{
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::red);
    ui.label_result->setPalette(pal);
    ui.label_result->setText(QStringLiteral("连接[") + url + QStringLiteral("]失败"));
    QTimer::singleShot(3000, this, SLOT(onTestResultTimeout()));
}

void CollectSetDlg::onTestResultTimeout()
{
    ui.label_result->setText("");
}

void CollectSetDlg::onWaitForTestSource()
{
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::black);
    ui.label_result->setPalette(pal);
    ui.label_result->setText(QStringLiteral("正在测试收集目录网络连接，请稍后..."));
    QTimer::singleShot(300, this, SLOT(onRemoteColTest()));
}

void CollectSetDlg::onWaitForTestDest()
{
	QPalette pal;
	pal.setColor(QPalette::WindowText, Qt::black);
	ui.label_result->setPalette(pal);
	ui.label_result->setText(QStringLiteral("正在测试分发目录网络连接，请稍后..."));
	QTimer::singleShot(300, this, SLOT(onRemoteDestTest()));
}

QString CollectSetDlg::getSendUserNameFromDirID(const QString &CollectDirId)
{
    CollectUser oUser = getSendUserInfoFromDirID(CollectDirId);
    return oUser.user.userName;
}

void CollectSetDlg::onRenameRuleEdit()
{
    CRenameDlg rdg;
    rdg.exec();
}

void CollectSetDlg::onSendUserChange(const QString &userName)
{
    CollectUser oSendUser = getSendUserInfoFromName(userName);
    // 当收集目录以及分发地址均是使用文件目录时，启用文件比对功能
    if (FILE_TYPE == oSendUser.user.sendType && ui.radFile->isChecked())
    {
        ui.compare_content->setVisible(true);
    }
    else
    {
        ui.compare_content->setVisible(false);
    }
    //ui.compare_content->setVisible(bFile);
}

void CollectSetDlg::onCompareContent(int state)
{
    if (Qt::Checked == state)
    {
        QPalette pal;
        pal.setColor(QPalette::WindowText, Qt::red);
        ui.label_result->setPalette(pal);
        ui.label_result->setText(QStringLiteral("启用内容比较会降低文件同步效率！"));
        QTimer::singleShot(3000, this, SLOT(onTestResultTimeout()));
    }

}

void CollectSetDlg::onSubDirFilterEdit()
{
    CSubDirTemplateUi oSubDir;
    oSubDir.exec();
}


