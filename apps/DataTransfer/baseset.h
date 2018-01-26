#ifndef baseset_h__
#define baseset_h__



#include "ui_baseset.h"

#include <QWidget>

class CBaseSetDlg : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief:默认构造函数
     * @param: QWidget * parent
     * @return: 描述返回值
     */
    CBaseSetDlg(QWidget *parent = NULL);

    /**
     * @brief:析构函数
     * @return: 描述返回值
     */
    virtual ~CBaseSetDlg();

    /**
     * @brief:初始化
     * @return:void 描述返回值
     */
    void init();

signals:
    // 确认按钮，保存并关闭
    void wndquit();

    // 退出按钮，关闭
    void wndcancel();

protected slots:
    /**
     * @brief:应用
     * @return:void 描述返回值
     */
    void onApply();

    /**
     * @brief:确认（保存并关闭）
     * @return:void 描述返回值
     */
    void onOk();

private:
    Ui::baseset ui;
};
#endif // baseset_h__
