#include "DataBase.h"
#include "ctkLog.h"
#include <QCoreApplication>
#include <QFile>
#include <QSqlError>
#include <QFileInfo>
#include <QDir>
#include <QUuid>
#include <QMutex>

DataBase *DataBase::s_db = NULL;
QMutex DataBase::m_oMutex;

QString g_tbDirCol[] = {"DIRID", "DIRNAME", "ENABLE", "COLLECTTYPE", "FTPTRANSFERMODE", "FTPCONNECTMODE", "RLTVPATH", "DISPATCH",
                        "FILETEMPLATE", "SUBDIRCHECK", "MOVEFLAG", "COLTIMERANGE", "RECORDLATESTTIME", "LOGINUSER", "LOGINPASS",
                        "IP", "PORT"
                       };
//QString g_tbColUser[] = {"DIRID", "USERID", "RLTVPATH"};
//QString g_tbSendUser[] = {"USERID", "USERNAME", "SENDTYPE", "SENDSUFFIX", "RLTVPATH", "LOGINUSER", "LOGINPASS",
//                          "IP", "PORT", "KEEPDIR", "COMPRESS", "ENCRYPT", "CONPUT", "MAXTRYCOUNS"
//                         };

DataBase * DataBase::getInstance()
{
    if (s_db == NULL)
    {
        if (s_db == NULL)
        {
            s_db = new DataBase();
        }
    }

    return s_db;
}

DataBase::DataBase()
{
    m_bDB = false;
    m_dbName = "Transfer";
    m_dbPath = qApp->applicationDirPath() + "/config/Transfer.db";
    QFileInfo fi(qApp->applicationDirPath() + "/config");
    if (!fi.exists())
    {

        QDir("").mkpath(fi.absoluteFilePath());
    }
    if (!QFile::exists(m_dbPath))
    {
        InitDB();
    }

    m_strTbCollect = "T_DIR_COL";
    // m_strTbColUser = "T_COL_USER";
    //m_strTbSendUser = "T_SEND_USER";

    // 判断清理表是否存在，如果不存在则自动创建
    checkTable("T_DIR_CLEAR", "CREATE TABLE T_DIR_CLEAR ("
               "[NAME] VHARCHAR(64) NOT NULL, "
               "[DIR] VHARCHAR(256) NOT NULL, "
               "[QUARTZ] VHARCHAR(64) NOT NULL,"
               "[FILE] VHARCHAR(64) NOT NULL, "
               "[UNIT] INT NOT NULL, "
               "[VALUE] INT NOT NULL, "
               "[ACTIVE] INT NOT NULL,  "
               "CONSTRAINT [sqlite_autoindex_T_DIR_CLEAR] PRIMARY KEY ([NAME]))");

    checkTable("T_COL_USER", "CREATE TABLE [T_COL_USER] ("
               "[DIRID] VARCHAR(20) NOT NULL, "
               "[USERID] VARCHAR(20), "
               "[RLTVPATH] VARCHAR(100),"
               "[RENAME_RULE] VARCHAR(20),"
               "CONSTRAINT [] PRIMARY KEY ([DIRID]))");

    checkTable("T_DIR_COL", "CREATE TABLE [T_DIR_COL] ("
               "[DIRID] VARCHAR(20) NOT NULL, "
               "[DIRNAME] VARCHAR(20) NOT NULL, "
               "[ENABLE] INT, "
               "[COLLECTTYPE] INT, "
               "[FTPTRANSFERMODE] INT,"
               "[FTPCONNECTMODE] INT,"
               "[RLTVPATH] VARCHAR(100), "
               "[DISPATCH] VARCHAR(50), "
               "[FILETEMPLATE] VARCHAR(50), "
               "[SUBDIRCHECK] INT, "
               "[MOVEFLAG] INT, "
               "[COLTIMERANGE] INT, "
               "[RECORDLATESTTIME] BOOL, "
               "[LOGINUSER] VARCHAR(20), "
               "[LOGINPASS] VARCHAR(20), "
               "[IP] VARCHAR(20), "
               "[PORT] INT, "
               "CONSTRAINT [sqlite_autoindex_T_DIR_COL_1] PRIMARY KEY ([DIRID]))");

    checkTable("T_SEND_USER", "CREATE TABLE [T_SEND_USER] ("
               "[USERID] VARCHAR(20) NOT NULL, "
               "[USERNAME] VARCHAR(20) NOT NULL, "
               "[SENDTYPE] INT, "
               "[SENDSUFFIX] VARCHAR(20) NOT NULL, "
               "[RLTVPATH] VARCHAR(100), "
               "[TIMEBASEDRULE] INT(1) DEFAULT (0), "
               "[LOGINUSER] VARCHAR(20), "
               "[LOGINPASS] VARCHAR(20), "
               "[IP] VARCHAR(20), "
               "[PORT] INT, "
               "[KEEPDIR] INT,"
               "[COMPRESS] INT,"
               "[ENCRYPT] INT, "
               "[CONPUT] INT, "
               "[MAXTRYCOUNS] INT,"
               "CONSTRAINT [] PRIMARY KEY ([USERID]))");

}

DataBase::~DataBase()
{
    m_db.close();
    QSLOG_INFO("~DataBase()");
}

bool DataBase::ConnectDB()
{
    if (m_bDB)
    {
        return true;
    }

    try
    {
        m_db = QSqlDatabase::addDatabase("QSQLITE", m_dbName);
        m_db.setDatabaseName(m_dbPath);
        if (!m_db.open())
        {
            QSLOG_ERROR(QStringLiteral("连接数据库失败：%1").arg(m_db.lastError().text()));
            return false;
        }
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("连接数据库异常：") + QString(e.what()));
        return false;
    }

    m_bDB = true;
    return true;
}

bool DataBase::InitDB()
{
    return true;
}

void DataBase::QueryCollectTask(QList<CollectTask> &colTasks)
{
    try
    {
        if (!ConnectDB())
        {
            return;
        }

        QSqlQuery query(m_db);
        QString sql = "select * from " + m_strTbCollect;
        bool res = query.exec(sql);
        if (res)
        {
            while (query.next())
            {
                CollectTask cTask;
                cTask.dirID = query.value(0).toString();
                cTask.dirName = query.value(1).toString();
                cTask.enable = query.value(2).toInt();
                cTask.collectType = query.value(3).toInt();
                cTask.ftp_transferMode = query.value(4).toInt();
                cTask.ftp_connectMode = query.value(5).toInt();
                cTask.rltvPath = query.value(6).toString();
                cTask.dispatch = query.value(7).toString();
                cTask.fileTemplate = query.value(8).toString();
                cTask.subdirFlag = query.value(9).toInt();
                cTask.moveFlag = query.value(10).toInt();
                cTask.col_timerange = query.value(11).toInt();
                cTask.recordLatestTime = query.value(12).toBool();
                cTask.loginUser = query.value(13).toString();
                cTask.loginPass = query.value(14).toString();
                cTask.ip = query.value(15).toString();
                cTask.port = query.value(16).toInt();

                colTasks.append(cTask);
            }
        }
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("查询数据库失败：")+QString(e.what()));
    }
}

bool DataBase::QueryCollectTask(CollectTask &task)
{
    try
    {
        if (!ConnectDB())
        {
            return false;
        }

        QSqlQuery query(m_db);
        QString sql = QString("select * from %1 where %2='%3'").arg(m_strTbCollect).arg(g_tbDirCol[0]).arg(task.dirID);
        bool res = query.exec(sql);
        if (!res)
        {
            QSLOG_ERROR("Error: " + query.lastError().text());
        }
        if (query.next())
        {
            task.dirID = query.value(0).toString();
            task.dirName = query.value(1).toString();
            task.enable = query.value(2).toInt();
            task.collectType = query.value(3).toInt();
            task.ftp_transferMode = query.value(4).toInt();
            task.ftp_connectMode = query.value(5).toInt();
            task.rltvPath = query.value(6).toString();
            task.dispatch = query.value(7).toString();
            task.fileTemplate = query.value(8).toString();
            task.subdirFlag = query.value(9).toInt();
            task.moveFlag = query.value(10).toInt();
            task.col_timerange = query.value(11).toInt();
            task.recordLatestTime = query.value(12).toInt();
            task.loginUser = query.value(13).toString();
            task.loginPass = query.value(14).toString();
            task.ip = query.value(15).toString();
            task.port = query.value(16).toInt();

            return true;
        }
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("查询数据库失败：")+QString(e.what()));
    }

    return false;
}

bool DataBase::QueryUserInfo(TaskUser &user)
{
    user.lstUser.clear();
    try
    {
        if (!ConnectDB())
        {
            return false;
        }

        QSqlQuery query(m_db);
        // 先查收集用户表
        QString sql = QString("SELECT USERID, RLTVPATH, RENAME_RULE FROM T_COL_USER WHERE DIRID = '%1'").arg(user.taskID);
        bool res = query.exec(sql);
        if (res)
        {
            while (query.next())
            {
                CollectUser cUser;
                cUser.user.userID = query.value(0).toString();
                cUser.rltvPath = query.value(1).toString();
                cUser.rename_rule = query.value(2).toString();
                user.lstUser.append(cUser);
            }

            // 再查分发用户表
            for (int i=0; i<user.lstUser.size(); ++i)
            {
                UserInfo &uInfo = user.lstUser[i].user;
                sql = QString("SELECT USERID, USERNAME, SENDTYPE, SENDSUFFIX, RLTVPATH, TIMEBASEDRULE, LOGINUSER, LOGINPASS, IP, PORT, KEEPDIR, COMPRESS, ENCRYPT, CONPUT, MAXTRYCOUNS FROM T_SEND_USER WHERE USERID = '%1'").arg(uInfo.userID);
                res = query.exec(sql);
                if (res && query.next())
                {
                    int index = 1;
                    uInfo.userName = query.value(index++).toString();
                    uInfo.sendType = query.value(index++).toInt();
                    uInfo.sendSuffix = query.value(index++).toString();
                    uInfo.rootPath = query.value(index++).toString();
                    uInfo.timebaserule = query.value(index++).toInt();
                    uInfo.lgUser = query.value(index++).toString();
                    uInfo.lgPass = query.value(index++).toString();
                    uInfo.ip = query.value(index++).toString();
                    uInfo.port = query.value(index++).toInt();
                    uInfo.keepDir = query.value(index++).toInt();
                    uInfo.compress = query.value(index++).toInt();
                    uInfo.encrypt = query.value(index++).toInt();
                    uInfo.conput = query.value(index++).toInt();
                    uInfo.tryCount = query.value(index++).toInt();
                }
            }

            return true;
        }
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("查询数据库失败：")+QString(e.what()));
    }

    return false;
}

bool DataBase::QueryUserInfo(QList<UserInfo> &lstUser)
{
    try
    {
        if (!ConnectDB())
        {
            return false;
        }

        QSqlQuery query(m_db);
        // 先查收集用户表
        QString sql = QString("SELECT USERID, USERNAME, SENDTYPE, SENDSUFFIX, RLTVPATH, TIMEBASEDRULE, LOGINUSER, LOGINPASS, IP, PORT, KEEPDIR, COMPRESS, ENCRYPT, CONPUT, MAXTRYCOUNS FROM T_SEND_USER");
        bool res = query.exec(sql);
        if (res)
        {
            while (query.next())
            {
                UserInfo user;
                int index = 0;
                user.userID = query.value(index++).toString();
                user.userName = query.value(index++).toString();
                user.sendType = query.value(index++).toInt();
                user.sendSuffix = query.value(index++).toString();
                user.rootPath = query.value(index++).toString();
                user.timebaserule = query.value(index++).toInt();
                user.lgUser = query.value(index++).toString();
                user.lgPass = query.value(index++).toString();
                user.ip = query.value(index++).toString();
                user.port = query.value(index++).toInt();
                user.keepDir = query.value(index++).toInt();
                user.compress = query.value(index++).toInt();
                user.encrypt = query.value(index++).toInt();
                user.conput = query.value(index++).toInt();
                user.tryCount = query.value(index++).toInt();
                lstUser.push_back(user);
            }

            return true;
        }
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("查询分发用户表失败：")+QString(e.what()));
    }

    return false;
}

bool DataBase::InsertUserInfo(const UserInfo &user)
{
    try
    {
        if (!ConnectDB())
        {
            return false;
        }

        QSqlQuery query(m_db);
        // 先查收集用户表
        QString sql = QString("REPLACE INTO T_SEND_USER(USERID, USERNAME, SENDTYPE, SENDSUFFIX, RLTVPATH, TIMEBASEDRULE, LOGINUSER, LOGINPASS, IP, PORT, KEEPDIR, COMPRESS, ENCRYPT, CONPUT, MAXTRYCOUNS)"
                              "VALUES(:USERID, :USERNAME, :SENDTYPE, :SENDSUFFIX, :RLTVPATH, :TIMEBASEDRULE, :LOGINUSER, :LOGINPASS, :IP, :PORT, :KEEPDIR, :COMPRESS, :ENCRYPT, :CONPUT, :MAXTRYCOUNS)");
        query.prepare(sql);
        query.bindValue(":USERID", user.userID);
        query.bindValue(":USERNAME", user.userName);
        query.bindValue(":SENDTYPE", user.sendType);
        query.bindValue(":SENDSUFFIX", user.sendSuffix);
        query.bindValue(":RLTVPATH", user.rootPath);
        query.bindValue(":TIMEBASEDRULE", user.timebaserule);
        query.bindValue(":LOGINUSER", user.lgUser);
        query.bindValue(":LOGINPASS", user.lgPass);
        query.bindValue(":IP", user.ip);
        query.bindValue(":PORT", user.port);
        query.bindValue(":KEEPDIR", user.keepDir);
        query.bindValue(":COMPRESS", user.compress);
        query.bindValue(":ENCRYPT", user.encrypt);
        query.bindValue(":CONPUT", user.conput);
        query.bindValue(":MAXTRYCOUNS", user.tryCount);
        bool res = query.exec();
        if (!res)
        {
            QSLOG_ERROR(QString("Err: %1").arg(query.lastError().text()));
        }
        else
        {
            return true;
        }
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("新增分发用户失败：")+QString(e.what()));
    }

    return false;
}

bool DataBase::DeleteSendUser(const QString &userID)
{
    try
    {
        if (!ConnectDB())
        {
            return false;
        }

        QSqlQuery query(m_db);
        QString sql = QString("DELETE FROM T_SEND_USER WHERE USERID = '%1'").arg(userID);
        bool res = query.exec(sql);
        if (!res)
        {
            QSLOG_ERROR(QStringLiteral("删除分发用户失败：%1").arg(query.lastError().text()));
            return false;
        }


        sql = QString("DELETE FROM T_COL_USER WHERE USERID = '%1'").arg(userID);
        res = query.exec(sql);
        if (!res)
        {
            QSLOG_ERROR(QStringLiteral("删除收集分发用户失败：%1").arg(query.lastError().text()));
            return false;
        }
        else
        {
            return true;
        }
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("删除分发用户失败：") + QString(e.what()));
    }

    return false;
}

bool DataBase::InsertCollectUser(const TaskUser &user)
{
    try
    {
        if (!ConnectDB())
        {
            return false;
        }

        //m_db.transaction();
        QSqlQuery query(m_db);
        //QString sql;
        for (int i=0; i<user.lstUser.size(); i++)
        {
            QString sql = QString("REPLACE INTO T_COL_USER (DIRID, USERID, RLTVPATH, RENAME_RULE) "
                                  "VALUES(:DIRID, :USERID, :RLTVPATH, :RENAME_RULE)");
            query.prepare(sql);
            query.bindValue(":DIRID", user.taskID);
            query.bindValue(":USERID", user.lstUser.at(i).user.userID);
            query.bindValue(":RLTVPATH", user.lstUser.at(i).rltvPath);
            query.bindValue(":RENAME_RULE", user.lstUser.at(i).rename_rule);
            if (!query.exec())
            {
                QSLOG_ERROR(QString("Err: %1").arg(query.lastError().text()));
            }
        }
        return true;
        //return m_db.commit();
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("新增收集用户失败：")+QString(e.what()));
    }

    return false;
}

bool DataBase::DeleteCollectSenderUser(const QString &dirId, const QString &userId)
{
    try
    {
        if (!ConnectDB())
        {
            return false;
        }

        QSqlQuery query(m_db);
        //QString sql;
        // for (int i=0; i<user.lstUser.size(); i++)
        {
            QString sql = QString("DELETE FROM T_COL_USER WHERE DIRID = '%1' AND USERID = '%2' ").arg(dirId).arg(userId);
            // query.prepare(sql);
            //query.bindValue(":"+g_tbColUser[0], user.taskID);
            //query.bindValue(":"+g_tbColUser[1], user.lstUser.at(i).user.userID);
            //query.bindValue(":"+g_tbColUser[2], user.lstUser.at(i).rltvPath);

            if (!query.exec(sql))
            {
                QSLOG_ERROR(QString("Err: %1").arg(query.lastError().text()));
            }
        }
        return true;
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("新增收集用户失败：")+QString(e.what()));
    }

    return false;
}

bool DataBase::InsertCollectTask(const CollectTask &task)
{
    try
    {
        if (!ConnectDB())
        {
            return false;
        }

        QSqlQuery query(m_db);
        QString sql = QString("REPLACE INTO %1(%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15, %16, %17, %18)"
                              "VALUES(:%2,:%3,:%4,:%5,:%6,:%7,:%8,:%9,:%10,:%11,:%12,:%13,:%14, :%15, :%16, :%17, :%18)").arg(m_strTbCollect)
                      .arg(g_tbDirCol[0]).arg(g_tbDirCol[1]).arg(g_tbDirCol[2]).arg(g_tbDirCol[3]).arg(g_tbDirCol[4])
                      .arg(g_tbDirCol[5]).arg(g_tbDirCol[6]).arg(g_tbDirCol[7]).arg(g_tbDirCol[8]).arg(g_tbDirCol[9])
                      .arg(g_tbDirCol[10]).arg(g_tbDirCol[11]).arg(g_tbDirCol[12]).arg(g_tbDirCol[13]).arg(g_tbDirCol[14])
                      .arg(g_tbDirCol[15]).arg(g_tbDirCol[16]);
        query.prepare(sql);
        query.bindValue(":"+g_tbDirCol[0], task.dirID);
        query.bindValue(":"+g_tbDirCol[1], task.dirName);
        query.bindValue(":"+g_tbDirCol[2], task.enable);
        query.bindValue(":"+g_tbDirCol[3], task.collectType);
        query.bindValue(":" + g_tbDirCol[4], task.ftp_transferMode);
        query.bindValue(":" + g_tbDirCol[5], task.ftp_connectMode);
        query.bindValue(":"+g_tbDirCol[6], task.rltvPath);
        query.bindValue(":"+g_tbDirCol[7], task.dispatch);
        query.bindValue(":"+g_tbDirCol[8], task.fileTemplate);
        query.bindValue(":"+g_tbDirCol[9], task.subdirFlag);
        query.bindValue(":"+g_tbDirCol[10], task.moveFlag);
        query.bindValue(":"+g_tbDirCol[11], task.col_timerange);
        query.bindValue(":"+g_tbDirCol[12], task.recordLatestTime);
        query.bindValue(":"+g_tbDirCol[13], task.loginUser);
        query.bindValue(":"+g_tbDirCol[14], task.loginPass);
        query.bindValue(":"+g_tbDirCol[15], task.ip);
        query.bindValue(":"+g_tbDirCol[16], task.port);

        bool res = query.exec();
        if (!res)
        {
            QSLOG_ERROR(QString("Error: %1").arg(query.lastError().text()));
        }
        else
        {
            return true;
        }
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("Error：")+QString(e.what()));
    }

    return false;
}

bool DataBase::DeltCollectTask(const QString &dirID)
{
    try
    {
        if (!ConnectDB())
        {
            return false;
        }

        QSqlQuery query(m_db);
        QString sql;
        bool res;
        // 先删除收集任务表
        sql = QString("delete from %1 where %2='%3'").arg(m_strTbCollect).arg(g_tbDirCol[0]).arg(dirID);
        res = query.exec(sql);
        if (res)
        {
            // 再删除收集用户表
            sql = QString("delete from T_COL_USER where DIRID = '%3'").arg(dirID);
            res = query.exec(sql);
            if (res)
            {
                return true;
            }
        }

        QSLOG_ERROR(QString("Error: %1").arg(query.lastError().text()));
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("Error：")+QString(e.what()));
    }

    return false;
}


bool DataBase::queryDirLatestTime(const QString &db, const QString &dir, QString &latesttime)
{
    if (!init(db))
    {
        return false;
    }

    QSqlDatabase oDataBase = m_name2db[db];

    QSqlQuery query(oDataBase);
    QString sql("SELECT LATESTTIME FROM TB_DIR_RECORD WHERE DIR = '" + dir + "'");

    bool res = query.exec(sql);
    if (res)
    {
        if (query.next())
        {
            latesttime = query.value(0).toString();
        }
        else
        {
            QSLOG_INFO(QStringLiteral("数据库中未查询到目录[%1]的最后修改时间").arg(dir));
        }
    }
    else
    {
        QSLOG_ERROR(QString("查询最后处理时间失败: %1").arg(query.lastError().text()));
        return false;
    }
    return  true;
}

bool DataBase::init(const QString &db)
{
    if (!m_name2db.contains(db))
    {
        QSqlDatabase oDataBase = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString());
        oDataBase.setDatabaseName(db);
        m_name2db.insert(db, oDataBase);
        QFileInfo fi(db);
        if (!QFileInfo::exists(fi.absolutePath()))
        {
            QDir dir(fi.absolutePath());
            dir.mkpath(fi.absolutePath());
        }

        if (!oDataBase.open())
        {
            QSLOG_ERROR(QStringLiteral("连接数据库失败：%1").arg(oDataBase.lastError().text()));
            return false;
        }
        else
        {
            // 判断表是否存在，如果表不存在，自动建表
            QSqlQuery query(oDataBase);
            QString sql("SELECT COUNT(*) FROM sqlite_master where type = 'table' and name = 'TB_DIR_RECORD'");

            bool res = query.exec(sql);
            if (res)
            {
                if (query.next())
                {
                    int iCount = query.value(0).toInt();
                    //QSLOG_INFO(QStringLiteral("%1").arg(iCount));
                    if (iCount == 0)
                    {
                        sql = "CREATE TABLE TB_DIR_RECORD ([DIR] VHARCHAR(512) NOT NULL, [LATESTTIME] VHARCHAR(14) NOT NULL,  CONSTRAINT [sqlite_autoindex_TB_DIR_RECORD] PRIMARY KEY ([DIR]))";
                        res = query.exec(sql);
                        if (!res)
                        {
                            QSLOG_ERROR(QStringLiteral("创建数据库表TB_DIR_RECORD失败，错误原因: %1").arg(query.lastError().text()));
                            return false;
                        }
                        else
                        {
                            QSLOG_INFO(QStringLiteral("创建数据库表TB_DIR_RECORD成功"));
                        }
                    }

                }
                else
                {
                    // QSLOG_INFO(QStringLiteral("数据库中未查询到目录[%1]的最后修改时间").arg(dir));
                }
            }
            else
            {
                QSLOG_ERROR(QStringLiteral("判断TB_DIR_RECORD表是否存在失败，Error: %1").arg(query.lastError().text()));
                return false;

            }
        }
    }

    return true;
}

bool DataBase::updateDirLatestTime(const QString &db, const QString &dir, const QString &latesttime)
{
    if (!init(db))
    {
        return false;
    }

    QSqlDatabase oDataBase = m_name2db[db];

    QSqlQuery query(oDataBase);
    QString sql("REPLACE INTO TB_DIR_RECORD(DIR, LATESTTIME) VALUES(:DIR, :LATESTTIME)" );

    query.prepare(sql);
    query.bindValue(":DIR", dir);
    query.bindValue(":LATESTTIME", latesttime);


    bool res = query.exec();
    if (!res)
    {
        QSLOG_ERROR(QStringLiteral("更新目录最后修改时间失败，Error: %1").arg(query.lastError().text()));
        return false;
    }
    else
    {
        QSLOG_DEBUG(QStringLiteral("更新目录【%1】最后处理时间【%2】").arg(dir).arg(latesttime));
        return true;
    }

}

bool DataBase::queryClearTask(QList<ClearTask> &tasks)
{
    if (!ConnectDB())
    {
        return false;
    }

    QSqlQuery query(m_db);

    QString sql = QString("select * from T_DIR_CLEAR");
    bool res = query.exec(sql);
    if (res)
    {
        while (query.next())
        {
            ClearTask task;
            task.taskName = query.value(0).toString();
            task.taskDir = query.value(1).toString();
            task.quartzRule = query.value(2).toString();
            task.matchRule = query.value(3).toString();
            task.clearUnit = query.value(4).toInt();
            task.clearValue = query.value(5).toInt();
            task.activeFlag = query.value(6).toInt();
            tasks.append(task);
        }

        return true;
    }
    else
    {
        QSLOG_ERROR(QStringLiteral("查询T_DIR_CLEAR失败，Error: %1").arg(query.lastError().text()));
        return false;
    }

    return true;
}

bool DataBase::queryClearTask(ClearTask &task)
{
    if (!ConnectDB())
    {
        return false;
    }

    QSqlQuery query(m_db);

    QString sql = QString("select * from T_DIR_CLEAR where name = '%1'").arg(task.taskName);
    bool res = query.exec(sql);
    if (res)
    {
        if (query.next())
        {
            task.taskName = query.value(0).toString();
            task.taskDir = query.value(1).toString();
            task.quartzRule = query.value(2).toString();
            task.matchRule = query.value(3).toString();
            task.clearUnit = query.value(4).toInt();
            task.clearValue = query.value(5).toInt();
            task.activeFlag = query.value(6).toInt();
            return true;
        }
        else
        {
            return false;
        }


    }
    else
    {
        QSLOG_ERROR(QStringLiteral("查询T_DIR_CLEAR失败，Error: %1").arg(query.lastError().text()));
        return false;
    }

    return true;
}

bool DataBase::queryClearTask(const QString &dir, QString &name)
{
    if (!ConnectDB())
    {
        return false;
    }

    QSqlQuery query(m_db);

    QString sql = QString("select NAME from T_DIR_CLEAR where DIR = '%1'").arg(dir);
    bool res = query.exec(sql);
    if (res)
    {
        if (query.next())
        {
            name = query.value(0).toString();
            return true;
        }
        else
        {
            return false;
        }


    }
    else
    {
        QSLOG_ERROR(QStringLiteral("查询T_DIR_CLEAR失败，Error: %1").arg(query.lastError().text()));
        return false;
    }

    return true;
}

bool DataBase::checkTable(const QString &table, const QString &createSql)
{
    if (!ConnectDB())
    {
        return false;
    }

    QSqlQuery query(m_db);
    QString sql(QString("SELECT COUNT(*) FROM sqlite_master where type = 'table' and name = '%1'").arg(table));

    bool res = query.exec(sql);
    if (res)
    {
        if (query.next())
        {
            int iCount = query.value(0).toInt();
            //QSLOG_INFO(QStringLiteral("%1").arg(iCount));
            if (iCount == 0)
            {
                sql = createSql;
                res = query.exec(sql);
                if (!res)
                {
                    QSLOG_ERROR(QStringLiteral("创建数据库表%1失败，错误原因: %2").arg(table).arg(query.lastError().text()));

                    return false;
                }
                else
                {
                    QSLOG_INFO(QStringLiteral("创建数据库表%1成功").arg(table));
                }
            }

        }
        else
        {
            // QSLOG_INFO(QStringLiteral("数据库中未查询到目录[%1]的最后修改时间").arg(dir));
        }
    }
    else
    {
        QSLOG_ERROR(QStringLiteral("判断%1表是否存在失败，Error: %2").arg(table).arg(query.lastError().text()));
        return false;

    }
    return true;
}

bool DataBase::insertClearTask(const ClearTask &task)
{
    try
    {
        if (!ConnectDB())
        {
            return false;
        }

        QSqlQuery query(m_db);
        QString sql = QString("REPLACE INTO T_DIR_CLEAR(NAME, DIR, QUARTZ, FILE, UNIT, VALUE, ACTIVE)"
                              "VALUES(:NAME, :DIR,:QUARTZ,:FILE,:UNIT, :VALUE, :ACTIVE)");
        query.prepare(sql);
        query.bindValue(":NAME", task.taskName);
        query.bindValue(":DIR", task.taskDir);
        query.bindValue(":QUARTZ", task.quartzRule);
        query.bindValue(":FILE", task.matchRule);
        query.bindValue(":UNIT", task.clearUnit);
        query.bindValue(":VALUE", task.clearValue);
        query.bindValue(":ACTIVE", task.activeFlag);

        bool res = query.exec();
        if (!res)
        {
            QSLOG_ERROR(QString("Error: %1").arg(query.lastError().text()));
        }
        else
        {
            return true;
        }
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("Error：")+QString(e.what()));
    }

    return false;
}

bool DataBase::deleteClearTask(const QString &name)
{
    try
    {
        if (!ConnectDB())
        {
            return false;
        }

        QSqlQuery query(m_db);
        QString sql;
        bool res;
        // 先删除收集任务表
        sql = QString("delete from T_DIR_CLEAR where name = '%1'").arg(name);
        res = query.exec(sql);
        if (res)
        {
            return true;
        }

        QSLOG_ERROR(QString("Error: %1").arg(query.lastError().text()));
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("Error：")+QString(e.what()));
    }

    return false;
}
