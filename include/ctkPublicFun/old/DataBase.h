#ifndef DataBase_H_
#define DataBase_H_


#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include "macro.h"
#include "commontypedef.h"

class DLL_EXPORT_CLASS_DECL DataBase
{
public:
    static DataBase *getInstance();
    ~DataBase();

    bool ConnectDB();
    bool InitDB();

    void QueryCollectTask(QList<CollectTask> &colTasks);

    bool QueryCollectTask(CollectTask &task);

    bool QueryUserInfo(CollectUser &user);

    bool QueryUserInfo(QList<UserInfo> &lstUser);

    // 新增分发用户
    bool InsertUserInfo(const UserInfo &user);

    // 新增收集用户
    bool InsertCollectUser(const CollectUser &user);

    // 新增收集任务
    bool InsertCollectTask(const CollectTask &task);

    // 删除收集任务
    bool DeltCollectTask(const QString &dirID);

    bool DeleteSendUser(const QString &userID);

private:
    DataBase();
    static DataBase *s_db;
    static QMutex m_oMutex;

    bool	m_bDB;				///< true表示已连接
    QSqlDatabase m_db;
    QString m_dbName;
    QString m_dbPath;

    QString m_strTbCollect;		///< 收集配置表
    QString m_strTbColUser;		///< 收集用户表
    QString	m_strTbSendUser;	///< 分发用户表
};

#endif