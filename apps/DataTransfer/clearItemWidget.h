#ifndef clearItemWidget_h__
#define clearItemWidget_h__

#include <QWidget>
#include <QMovie>
#include "ui_clearItemWidget.h"

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
};
#endif // clearItemWidget_h__
