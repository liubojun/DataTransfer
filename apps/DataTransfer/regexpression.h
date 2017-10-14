#include <QDialog>

#include "ui_regularexpression.h"

class CRegExpDlg :public QDialog
{
    Q_OBJECT
public:
    CRegExpDlg(QDialog *parent = 0);

protected slots:
    // 值变化时，自动触发匹配
    void valueChanged();

private:
    Ui::RegularExpression ui;
};