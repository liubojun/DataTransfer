#ifndef listItemWidget_H_
#define listItemWidget_H_


#include <QWidget>
#include <QIcon>
#include <QMovie>
#include "ui_itemWidget.h"
#include "status.h"

enum ICONTYPE
{
    // 正常灰化
    FILE_FILE_S=0,
    FTP_FILE_S=1,
    FILE_FTP_S=2,
    FTP_FTP_S=3,
    // 正常
    FILE_FILE=4,
    FTP_FILE=5,
    FILE_FTP=6,
    FTP_FTP=7,
    // 灰化且不正常
    FILE_FILE_SE=8,
    FTP_FILE_SE=9,
    FILE_FTP_SE=10,
    FTP_FTP_SE=11,
    // 不正常
    FILE_FILE_E=12,
    FTP_FILE_E=13,
    FILE_FTP_E=14,
    FTP_FTP_E=15
};

class MyItemWidget : public QWidget
{
    Q_OBJECT
public:
    MyItemWidget(const ICONTYPE &icon, const QString &name, QString state="");
    ~MyItemWidget();

    void SetName(const QString &strName);
    void SetIcon(int colType, int sendType, int enable, int normal);
    void SetIcon(const ICONTYPE &icon);
    void SetState(const QString &state, bool bShow);
    void SetHighlight(bool bFlag);
    void ShowGif(bool bFlag);
    void SetEnable(bool bFlag);
    void AddSuccess(const QDateTime &dt);

private:
    Ui::itemWidget ui;
    QMovie *m_pMovie;
    //CStatusLabel *m_pStatusWnd;

    QIcon m_icon;
    QString m_name;
    QString m_state;

    ICONTYPE m_curIcon;

    quint64 m_nFinish;
};

#endif