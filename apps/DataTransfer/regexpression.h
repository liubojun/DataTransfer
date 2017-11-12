#ifndef regexpression_h__
#define regexpression_h__

#include <QDialog>

#include "ui_RegularExpression.h"

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



#endif // regexpression_h__
