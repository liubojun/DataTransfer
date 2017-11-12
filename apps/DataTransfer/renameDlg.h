#ifndef renameDlg_h__
#define renameDlg_h__

#include <QDialog>

#include "ui_renameDlg.h"

// 类说明：用于编辑以及测试文件换名规则

class CRenameDlg :public QDialog
{
    Q_OBJECT
public:
    CRenameDlg(QDialog *parent = 0);

protected:
    // 加载rename.rule配置文件
    void init();

protected slots:
    // 编辑文件内容后，保存
    void onSave();

    // 取消保存
    void onCancel();

    // 点击重命名测试按钮
    void onChangeTest();

    // 输入框变化事件
    void onTextEdit_ori_name_Change();

    // 输入框变化事件
    //void onlineEdit_rule_Change(const QString &rule);

private:
    Ui::renameDlg ui;

    // 使用静态变量保存文件路径
    QString renamerulefile;
};
#endif // renameDlg_h__
