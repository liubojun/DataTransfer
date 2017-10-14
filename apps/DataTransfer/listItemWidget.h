#ifndef listItemWidget_H_
#define listItemWidget_H_


#include <QWidget>
#include <QIcon>
#include <QMovie>
#include "ui_itemWidget.h"

enum ICONTYPE
{
    // 正常灰化
    FILE_FILE_S=0,
    FTP_FILE_S,
    FILE_FTP_S,
    FTP_FTP_S,
    // 正常
    FILE_FILE,
    FTP_FILE,
    FILE_FTP,
    FTP_FTP,
    // 灰化且不正常
    FILE_FILE_SE,
    FTP_FILE_SE,
    FILE_FTP_SE,
    FTP_FTP_SE,
    // 不正常
    FILE_FILE_E,
    FTP_FILE_E,
    FILE_FTP_E,
    FTP_FTP_E
};

class MyItemWidget : public QWidget
{
    Q_OBJECT
public:
    MyItemWidget(const ICONTYPE &icon, const QString &name, QString state="");
    ~MyItemWidget();

    void SetName(const QString &strName);
    void SetIcon(const ICONTYPE &icon);
    void SetState(const QString &state, bool bShow);
    void SetHighlight(bool bFlag);
    void ShowGif(bool bFlag);
    void SetEnable(bool bFlag);
    void AddSuccess(const QDateTime &dt);

private:
    Ui::itemWidget ui;
    QMovie *m_pMovie;

    QIcon m_icon;
    QString m_name;
    QString m_state;

    ICONTYPE m_curIcon;

    int m_nFinish;
};

#endif