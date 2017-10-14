﻿#ifndef collectSetDlg_H_
#define collectSetDlg_H_

#include <QDialog>
#include <QUuid>
#include "ui_collect.h"
#include "selUserDlg.h"
#include "commontypedef.h"

class CollectSetDlg : public QDialog
{
    Q_OBJECT
public:
    CollectSetDlg(int flag=0, QDialog *parent = NULL);
    ~CollectSetDlg();
    // 显示收集信息
    void showTask(const CollectTask &task);

    // 根据收集目录ID获取分发用户名
    QString getSendUserNameFromDirID(const QString &CollectDirId);

    CollectTask m_task;
    TaskUser m_selUser;

private slots:
    // 添加分发用户
    void selUser();
    bool onApply();
    void onCancel();
    void onBrows();
    void onSelFile(bool);
    void onSendUserDel(const QString &user);

    // 添加正则表达式测试
    void onRegExpTest();

    // 测试收集目录是否正常
    void onRemoteColTest();

    // 测试收集路径成功
    void onTestOk();

    // 测试收集路径失败
    void onTestFail();

    // 测试结果超时
    void onTestResultTimeout();

    // 检测网络状况
    void onWaitForTest();

signals:
    void commit(const CollectTask &task);

    // 测试收集路径成功
    void testok();

    // 测试收集路径失败
    void testfail();

private:
    void InitUI();

    // 根据分发用户名获取分发用户全部信息
    CollectUser getSendUserInfoFromName(const QString &name);

    // 根据收集目录ID获取分发用户全部信息
    CollectUser getSendUserInfoFromDirID(const QString &CollectDirId);



private:
    Ui::collectDlg ui;
    QSharedPointer<SelUserDlg>		m_pSelUserDlg;

    int m_flag;		///< 0-新增，1-修改或查看
};

#endif