#ifndef DataBase_H_
#define DataBase_H_


#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QMap>
#include <QMutex>
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

    bool QueryUserInfo(TaskUser &user);

    bool QueryUserInfo(QList<UserInfo> &lstUser);

    // 新增分发用户
    bool InsertUserInfo(const UserInfo &user);

    // 新增收集用户
    bool InsertCollectUser(const TaskUser &user);

    // 新增收集任务
    bool InsertCollectTask(const CollectTask &task);

    // 删除收集任务
    bool DeltCollectTask(const QString &dirID);

    bool DeleteSendUser(const QString &userID);

    // 删除收集任务中的分发用户
    bool DeleteCollectSenderUser(const QString &dirId, const QString &userId);

    /**
     * @brief  查询目录的最后处理时间
     * @param  const QString & db 数据库地址
     * @param  const QString & dir 最后处理时间
     * @param  QString & latesttime 目录最后处理时间
     * @return bool：处理成功返回true,失败返回false
     */
    bool queryDirLatestTime(const QString &db, const QString &dir, QString &latesttime);

    bool queryClearTask(QList<ClearTask> &name);

    bool queryClearTask(ClearTask &name);

    // 根据目录全路径查找目录ID
    bool queryClearTask(const QString &dir, QString &name);

    bool insertClearTask(const ClearTask &task);

    bool deleteClearTask(const QString &name);

    /**
    * @brief  更新目录的最后处理时间
    * @param  const QString & db 数据库地址
    * @param  const QString & dir 最后处理时间
    * @param  const QString & latesttime 目录最后处理时间
    * @return bool：处理成功返回true,失败返回false
    */
    bool updateDirLatestTime(const QString &db, const QString &dir, const QString &latesttime);

    /**
     * @brief:查询当前可用线程数以及日志监听端口
     * @param: int & threadNum 可用线程数
     * @param: int &logPort 日志监听端口
     * @return:bool 查询成功，返回true，失败返回false
     */
    bool queryBaseInfo(int &threadNum, int &logPort);

    /**
     * @brief:更新基本信息
     * @param: int threadNum 线程数目
     * @param: int logPort 日志端口
     * @return:bool 更新成功，返回true，失败返回false
     */
    bool updateBaseInfo(int threadNum, int logPort);

    /**
     * @brief:查询日志监听端口号
     * @param: int & portNum 日志监听端口号
     * @return:bool 查询成功，返回true，失败返回false
     */
    bool queryLogListenPort(int &portNum);

    /**
     * @brief:更新日志监听端口，更新完毕后，需重启
     * @param: int portNum 日志监听端口号
     * @return:bool 更新成功，返回true，失败返回false
     */
    bool updateLogListemPort(int portNum);

    /**
     * @brief  初始化处理
     * @param  const QString & 数据库地址
     * @return bool：初始化成功返回true,失败返回false
     */
    bool init(const QString &db);

    bool checkTable(const QString &table, const QString &createSql);

private:
    DataBase();
    static DataBase *s_db;
    static QMutex m_oMutex;

    bool	m_bDB;				///< true表示已连接
    QSqlDatabase m_db;
    QString m_dbName;
    QString m_dbPath;

    QString m_strTbCollect;		///< 收集配置表
    // QString m_strTbColUser;		///< 收集用户表
    //QString	m_strTbSendUser;	///< 分发用户表

    QMap<QString, QSqlDatabase> m_name2db;	// 数据库地址与数据库实例映射
};

#endif