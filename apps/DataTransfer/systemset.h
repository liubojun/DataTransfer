#ifndef systemset_h__
#define systemset_h__

#include "ui_systemset.h"
#include <QDialog>

class CSystemSetDlg : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief:默认构造函数
     * @param: QDialog * parent
     * @return: 描述返回值
     */
    CSystemSetDlg(QDialog *parent = NULL);

    /**
     * @brief:默认析构函数
     * @return: 描述返回值
     */
    virtual ~CSystemSetDlg();

private:
    Ui::SystemSet ui;
};
#endif // systemset_h__
