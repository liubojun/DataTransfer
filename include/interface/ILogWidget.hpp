/**
  * @file   ILogWidget.h
  * @brief  多文档-日志窗.
  *
  *  这里是详细说明。
  * @author 刘伯俊
  * @par   Copyright (c):
  *          刘伯俊.
  *         All Rights Reserved
  * @date   2016:07:04
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef ILogWidget_h__
#define ILogWidget_h__

#include <QTextEdit>

class ILogWidget : public QTextEdit
{
    Q_OBJECT
public:
    ILogWidget(QWidget *parent = NULL) : QTextEdit(parent)
    {
        m_iLineNum = 0;

        setAttribute(Qt::WA_DeleteOnClose);

        setReadOnly(true);
    }

    virtual ~ILogWidget() {}

signals:
    // 用此信号通知菜单栏上的Action
    void closed(bool flag);

protected:
    virtual void closeEvent(QCloseEvent *e)
    {
        // 绑定QAction的setChecked槽，所以此处需要用逆向
        emit closed(false);
    }

public slots:
    virtual void addLine(const QString &msg)
    {
        if (m_iLineNum > 500)
        {
            m_iLineNum = 0;
            clear();
        }
        append(msg);
        m_iLineNum++;
    }

private:
    qint32 m_iLineNum;

};
#endif // ILogWidget_h__