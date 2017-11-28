#ifndef status_h__
#define status_h__

#include <QWidget>
#include <QTimer>

/************************************************************************/
/*                  用途：实现工作指示灯闪烁效果控件
					作者: 刘伯俊
					日期: 2017-11-28
/************************************************************************/

class CStatusLabel : public QWidget
{
    Q_OBJECT
public:
    CStatusLabel();

    virtual ~CStatusLabel();

    // 启动闪烁
    void start();

    // 停止闪烁
    void stop();

protected:
    // 重写paintEvent实现指示灯闪烁效果
    virtual void paintEvent(QPaintEvent *event);

private:
    QTimer m_oTimer;;
    // 标识是否启动闪烁
    bool m_bIsStart;
    bool m_bLastGreen;
};
#endif // status_h__
