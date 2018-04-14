#ifndef clearItemWidget_h__
#define clearItemWidget_h__

#include <QWidget>
#include <QMovie>
#include "ui_clearItemWidget.h"

#include "status.h"

class ClearItemWidget : public QWidget
{
    Q_OBJECT
public:
    ClearItemWidget(const QString &name, QWidget *parent = NULL);

    void ShowGif(bool bFlag);

    void SetEnable(bool bFlag);

    void setName(const QString &name);

    void addSuccess(const QDateTime &dt);
private:
    Ui::clearItemWidget ui;

    QMovie *m_pMovie;
    quint64 m_nFinish;
    //CStatusLabel *m_pStatusWnd;
};
#endif // clearItemWidget_h__
