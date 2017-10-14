#ifndef userItem_H_
#define userItem_H_


#include <QWidget>
#include <QListWidget>
#include "ui_userItemWidget.h"
#include "ui_itemWidget.h"


class UserItem : public QWidget
{
    Q_OBJECT
public:
    UserItem(QListWidget *in_pLstWgt, const QString &userId, const QString &userName, const QString &rltvPath);
    ~UserItem();

    void SetName(const QString &strName);
    void SetState(const QString &state, bool bShow);
    void SetHighlight(bool bFlag);
    void ShowGif(bool bFlag);
    void SetEnable(bool bFlag);
    void AddSuccess(const QDateTime &dt);

	QString senduserId();
	QString sendusername();
	QString sendRtvPath();

private slots:
    void onMdfy();
    void onDelt();

signals:
	void onDelete(const QString &userId);

private:
    Ui::userItem ui;

    QString m_name;
    QString m_state;

    int m_nFinish;
	QListWidget *m_pWidget;
	QString m_strUserId;	// 用户id
	QString m_strUserName;	// 用户名
	QString m_strRtvPath;	// 相对路径
};

#endif