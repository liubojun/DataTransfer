#include "DataBase.h"
#include "ctkLog.h"
#include <QCoreApplication>
#include <QFile>
#include <QSqlError>
#include <QFileInfo>
#include <QDir>
#include <QUuid>
#include <QMutex>
#include <QUuid>

#ifdef WITH_MEM_CHECK
#include "vld.h"
#endif

DataBase *DataBase::s_db = NULL;
QMutex DataBase::m_oMutex;

#define MAGIC_NUM 20180124

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
    //m_bDB = false;
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

    //m_strTbCollect = "T_DIR_COL";
    // m_strTbColUser = "T_COL_USER";
    //m_strTbSendUser = "T_SEND_USER";

    // 判断清理表是否存在，如果不存在则自动创建
    checkTable("T_DIR_CLEAR", "CREATE TABLE[T_DIR_CLEAR]("
               "[ID] VARCHAR2(20) NOT NULL,"
               "[NAME] VHARCHAR(64) NOT NULL,"
               "[TYPE] INT(1) NOT NULL DEFAULT(0),"
               "[IP] VARCHAR2(50),"
               "[PORT] INT(5),"
               "[USER] VARCHAR2(20),"
               "[PASSWORD] VARCHAR2(20),"
               "[TRANSFERMODE] INT(1) DEFAULT(0),"
               "[DIR] VHARCHAR(256) NOT NULL,"
               "[QUARTZ] VHARCHAR(64) NOT NULL,"
               "[FILE] VHARCHAR(64) NOT NULL,"
               "[UNIT] INT NOT NULL,"
               "[VALUE] INT NOT NULL,"
               "[ACTIVE] INT NOT NULL,"
               "CONSTRAINT[] PRIMARY KEY([ID]))");


    checkTable("T_COL_USER",
               "CREATE TABLE[T_COL_USER]("
			   "[CONNECTID] VARCHAR2(40) NOT NULL, "
               "[USERID] VARCHAR(20),"
               "[RLTVPATH] VARCHAR(100),"
               "[RENAME_RULE] VARCHAR(20),"
			   "[TIMERULE] INT(1),"
			   "[KEEPDIR] BOOL,"
               "CONSTRAINT[sqlite_autoindex_T_COL_USER_1] PRIMARY KEY([CONNECTID]) ON CONFLICT FAIL)"
              );

    checkTable("T_DIR_COL", "CREATE TABLE [T_DIR_COL] ("
               "[DIRID] VARCHAR(20) NOT NULL,"
               "[DIRNAME] VARCHAR(20) NOT NULL,"
               "[ENABLE] INT,"
               "[COLLECTTYPE] INT,"
               "[FTPTRANSFERMODE] INT,"
               "[FTPCONNECTMODE] INT,"
               "[RLTVPATH] VARCHAR(100),"
               "[DISPATCH] VARCHAR(50),"
               "[FILETEMPLATE] VARCHAR(50),"
               "[SUBDIRTEMPLATE] VARCHAR2(20),"
               "[SUBDIRCHECK] INT,"
               "[MOVEFLAG] INT,"
               "[COLTIMERANGE] INT,"
               "[RECORDLATESTTIME] BOOL,"
               "[COMPARE_CONTENT] BOOL DEFAULT 0,"
               "[LOGINUSER] VARCHAR(20),"
               "[LOGINPASS] VARCHAR(20),"
               "[IP] VARCHAR(20),"
               "[PORT] INT,"
			   "[CONNECTID] VARCHAR2(40),"
               "CONSTRAINT [sqlite_autoindex_T_DIR_COL_1] PRIMARY KEY ([DIRID]))");

    checkTable("T_SEND_USER", "CREATE TABLE [T_SEND_USER] ("
               "[USERID] VARCHAR(20) NOT NULL, "
               "[USERNAME] VARCHAR(20) NOT NULL, "
               "[SENDTYPE] INT, "
               "[FTPTRANSFERTYPE] INT(1) NOT NULL DEFAULT 0,"
               "[FTPTRANSFERMODE] INT(1) NOT NULL DEFAULT 0, "
               "[SENDSUFFIX] VARCHAR(20) NOT NULL, "
               "[RLTVPATH] VARCHAR(100), "
               "[LOGINUSER] VARCHAR(20), "
               "[LOGINPASS] VARCHAR(20), "
               "[IP] VARCHAR(20), "
               "[PORT] INT, "
               "[CONPUT] INT, "
               "CONSTRAINT [sqlite_autoindex_T_SEND_USER_1] PRIMARY KEY ([USERID]))");

    checkTable("T_GLOBALINFO", "CREATE TABLE [T_GLOBALINFO] ("
               "[PROGRAMID] INT,"
               "[THREADNUM] INT,"
               "[LOGPORT] INT,"
               "[ENABLELOG] BOOL,"
               "[BROADCASTPORT] INT DEFAULT 50002,"
               "[ENABLEBROADCAST] BOOL,"
               "CONSTRAINT [] PRIMARY KEY ([PROGRAMID]) ON CONFLICT FAIL)");

}

DataBase::~DataBase()
{
    //m_db.close();
    //QSLOG_INFO("~DataBase()");
}

bool DataBase::ConnectDB(QSqlDatabase &io_db)
{
    //if (m_bDB)
    //{
    //    return true;
    //}

    try
    {
        //m_db = QSqlDatabase::addDatabase("QSQLITE", m_dbName);
        //m_db.setDatabaseName(m_dbPath);
        //if (!m_db.open())
        //{
        //    QSLOG_ERROR(QStringLiteral("连接数据库失败：%1").arg(m_db.lastError().text()));
        //    return false;
        //}
        io_db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString());
        io_db.setDatabaseName(m_dbPath);
        if (!io_db.open())
        {
            QSLOG_ERROR(QStringLiteral("连接数据库失败：%1").arg(io_db.lastError().text()));
            return false;
        }
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("连接数据库异常：") + QString(e.what()));
        return false;
    }

    //m_bDB = true;
    return true;
}

bool DataBase::InitDB()
{
    return true;
}

void DataBase::QueryCollectTask(QList<CollectTask> &colTasks)
{
    MyDataBase t_oDb;

    try
    {
        if (!t_oDb.connect())
        {
            return;
        }
        QSqlQuery &query(t_oDb.sqlquery());
        QString sql = "SELECT DIRID, DIRNAME, ENABLE, COLLECTTYPE, FTPTRANSFERMODE, FTPCONNECTMODE, RLTVPATH, DISPATCH,"
                      "FILETEMPLATE, SUBDIRCHECK, MOVEFLAG, COLTIMERANGE, RECORDLATESTTIME, COMPARE_CONTENT, LOGINUSER, LOGINPASS,"
                      "IP, PORT, CONNECTID FROM T_DIR_COL ORDER BY DIRNAME";
        bool res = query.exec(sql);
        if (!res)
        {
            QSLOG_ERROR("Error: " + query.lastError().text());
            //t_oDb.close();
            return;
        }
        while (query.next())
        {
            CollectTask cTask;
            int index = 0;
            cTask.dirID = query.value(index++).toString();
            cTask.dirName = query.value(index++).toString();
            cTask.enable = query.value(index++).toInt();
            cTask.collectType = query.value(index++).toInt();
            cTask.ftp_transferMode = query.value(index++).toInt();
            cTask.ftp_connectMode = query.value(index++).toInt();
            cTask.rltvPath = query.value(index++).toString();
            cTask.dispatch = query.value(index++).toString();
            cTask.fileTemplate = query.value(index++).toString();
            cTask.subdirFlag = query.value(index++).toInt();
            cTask.moveFlag = query.value(index++).toInt();
            cTask.col_timerange = query.value(index++).toInt();
            cTask.recordLatestTime = query.value(index++).toBool();
            cTask.compareContent = query.value(index++).toBool();
            cTask.loginUser = query.value(index++).toString();
            cTask.loginPass = query.value(index++).toString();
            cTask.ip = query.value(index++).toString();
            cTask.port = query.value(index++).toInt();
			cTask.connectId = query.value(index++).toString();
            colTasks.append(cTask);
        }

    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("查询数据库失败：")+QString(e.what()));
    }
}

bool DataBase::QueryCollectTask(CollectTask &task)
{
    MyDataBase t_oDb;

    try
    {
        if (!t_oDb.connect())
        {
            return false;
        }
        QSqlQuery &query(t_oDb.sqlquery());
        QString sql = QString("SELECT DIRID, DIRNAME, ENABLE, COLLECTTYPE, FTPTRANSFERMODE, FTPCONNECTMODE, RLTVPATH, DISPATCH,"
                              "FILETEMPLATE, SUBDIRTEMPLATE, SUBDIRCHECK, MOVEFLAG, COLTIMERANGE, RECORDLATESTTIME, COMPARE_CONTENT, LOGINUSER, LOGINPASS,"
                              "IP, PORT, CONNECTID from T_DIR_COL where DIRID = '%1'").arg(task.dirID);
        bool res = query.exec(sql);
        if (!res)
        {
            QSLOG_ERROR("Error: " + query.lastError().text());
            return false;
        }
        if (query.next())
        {
            int index = 0;
            task.dirID = query.value(index++).toString();
            task.dirName = query.value(index++).toString();
            task.enable = query.value(index++).toInt();
            task.collectType = query.value(index++).toInt();
            task.ftp_transferMode = query.value(index++).toInt();
            task.ftp_connectMode = query.value(index++).toInt();
            task.rltvPath = query.value(index++).toString();
            task.dispatch = query.value(index++).toString();
            task.fileTemplate = query.value(index++).toString();
            task.subDirTemplate = query.value(index++).toString();
            task.subdirFlag = query.value(index++).toInt();
            task.moveFlag = query.value(index++).toInt();
            task.col_timerange = query.value(index++).toInt();
            task.recordLatestTime = query.value(index++).toBool();
            task.compareContent = query.value(index++).toBool();
            task.loginUser = query.value(index++).toString();
            task.loginPass = query.value(index++).toString();
            task.ip = query.value(index++).toString();
            task.port = query.value(index++).toInt();
			task.connectId = query.value(index++).toString();
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
    //user.lstUser.clear();
    MyDataBase t_oDb;

    try
    {
        if (!t_oDb.connect())
        {
            return false;
        }
        QSqlQuery &query(t_oDb.sqlquery());
        // 先查收集用户表
		QString sql = QString("SELECT T1.USERID, T1.RLTVPATH, T1.RENAME_RULE, T1.TIMERULE, T1.KEEPDIR FROM T_COL_USER T1, T_DIR_COL T2 WHERE T1.CONNECTID = T2.CONNECTID AND T2.DIRID = '%1'").arg(user.taskID);

        bool res = query.exec(sql);
        if (!res)
        {
            QSLOG_ERROR("Error: " + query.lastError().text());
            return false;
        }

        while (query.next())
        {
            //CollectUser cUser;
			user.sendUser.user.userID = query.value(0).toString();
			user.sendUser.rltvPath = query.value(1).toString();
			user.sendUser.rename_rule = query.value(2).toString();
			user.sendUser.iTimeRule = query.value(3).toInt();
			user.sendUser.bKeepDir = query.value(3).toBool();
            //user.lstUser.append(cUser);
        }

        // 再查分发用户表
        //for (int i=0; i<user.lstUser.size(); ++i)
        {
			UserInfo &uInfo = user.sendUser.user;
            sql = QString("SELECT USERID, USERNAME, SENDTYPE, FTPTRANSFERTYPE, FTPTRANSFERMODE, SENDSUFFIX, RLTVPATH,  LOGINUSER, LOGINPASS, IP, PORT, CONPUT FROM T_SEND_USER WHERE USERID = '%1'").arg(uInfo.userID);
            res = query.exec(sql);
            if (!res)
            {
                QSLOG_ERROR("Error: " + query.lastError().text());
                //continue;
            }

            if (query.next())
            {
                int index = 1;
                uInfo.userName = query.value(index++).toString();
                uInfo.sendType = query.value(index++).toInt();

                uInfo.ftpTransferType = query.value(index++).toInt();
                uInfo.ftpTransferMode = query.value(index++).toInt();
                uInfo.sendSuffix = query.value(index++).toString();
                uInfo.rootPath = query.value(index++).toString();
                //uInfo.timebaserule = query.value(index++).toInt();
                uInfo.lgUser = query.value(index++).toString();
                uInfo.lgPass = query.value(index++).toString();
                uInfo.ip = query.value(index++).toString();
                uInfo.port = query.value(index++).toInt();
                //uInfo.keepDir = query.value(index++).toInt();
                //uInfo.compress = query.value(index++).toInt();
                //uInfo.encrypt = query.value(index++).toInt();
                uInfo.conput = query.value(index++).toInt();
                //uInfo.tryCount = query.value(index++).toInt();
            }
        }

        return true;

    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("查询数据库失败：")+QString(e.what()));
    }
    return false;
}

bool DataBase::QueryUserInfo(QList<UserInfo> &lstUser)
{
    MyDataBase t_oDb;

    try
    {
        if (!t_oDb.connect())
        {
            return false;
        }
        QSqlQuery &query(t_oDb.sqlquery());
        // 先查收集用户表
        QString sql = QString("SELECT USERID, USERNAME, SENDTYPE, FTPTRANSFERTYPE, FTPTRANSFERMODE, SENDSUFFIX, RLTVPATH,  LOGINUSER, LOGINPASS, IP, PORT,CONPUT FROM T_SEND_USER");
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
                user.ftpTransferType = query.value(index++).toInt();
                user.ftpTransferMode = query.value(index++).toInt();
                user.sendSuffix = query.value(index++).toString();
                user.rootPath = query.value(index++).toString();
                //user.timebaserule = query.value(index++).toInt();
                user.lgUser = query.value(index++).toString();
                user.lgPass = query.value(index++).toString();
                user.ip = query.value(index++).toString();
                user.port = query.value(index++).toInt();
                //user.keepDir = query.value(index++).toInt();
                //user.compress = query.value(index++).toInt();
                //user.encrypt = query.value(index++).toInt();
                user.conput = query.value(index++).toInt();
                //user.tryCount = query.value(index++).toInt();
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
    MyDataBase t_oDb;

    try
    {
        if (!t_oDb.connect())
        {
            return false;
        }
        QSqlQuery &query(t_oDb.sqlquery());
        // 先查收集用户表
        QString sql = QString("REPLACE INTO T_SEND_USER(USERID, USERNAME, SENDTYPE, FTPTRANSFERTYPE, FTPTRANSFERMODE, SENDSUFFIX, RLTVPATH,  LOGINUSER, LOGINPASS, IP, PORT, CONPUT)"
                              "VALUES(:USERID, :USERNAME, :SENDTYPE, :FTPTRANSFERTYPE, :FTPTRANSFERMODE, :SENDSUFFIX, :RLTVPATH, :LOGINUSER, :LOGINPASS, :IP, :PORT, :CONPUT)");
        query.prepare(sql);
        query.bindValue(":USERID", user.userID);
        query.bindValue(":USERNAME", user.userName);
        query.bindValue(":SENDTYPE", user.sendType);
        query.bindValue(":FTPTRANSFERTYPE", user.ftpTransferType);
        query.bindValue(":FTPTRANSFERMODE", user.ftpTransferMode);
        query.bindValue(":SENDSUFFIX", user.sendSuffix);
        query.bindValue(":RLTVPATH", user.rootPath);
        //query.bindValue(":TIMEBASEDRULE", user.timebaserule);
        query.bindValue(":LOGINUSER", user.lgUser);
        query.bindValue(":LOGINPASS", user.lgPass);
        query.bindValue(":IP", user.ip);
        query.bindValue(":PORT", user.port);
        //query.bindValue(":KEEPDIR", user.keepDir);
        //query.bindValue(":COMPRESS", user.compress);
        //query.bindValue(":ENCRYPT", user.encrypt);
        query.bindValue(":CONPUT", user.conput);
        //query.bindValue(":MAXTRYCOUNS", user.tryCount);
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
    MyDataBase t_oDb;

    try
    {
        if (!t_oDb.connect())
        {
            return false;
        }
        QSqlQuery &query(t_oDb.sqlquery());
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
    MyDataBase t_oDb;

    try
    {
        if (!t_oDb.connect())
        {
            return false;
        }
        QSqlQuery &query(t_oDb.sqlquery());
        //QString sql;
        //for (int i=0; i<user.lstUser.size(); i++)
        {
            QString sql = QString("REPLACE INTO T_COL_USER (CONNECTID, USERID, RLTVPATH, RENAME_RULE, TIMERULE, KEEPDIR) "
                                  "VALUES(:CONNECTID, :USERID, :RLTVPATH, :RENAME_RULE, :TIMERULE, :KEEPDIR)");
            query.prepare(sql);
            query.bindValue(":CONNECTID", user.connectId);
            query.bindValue(":USERID", user.sendUser.user.userID);
			query.bindValue(":RLTVPATH", user.sendUser.rltvPath);
			query.bindValue(":RENAME_RULE", user.sendUser.rename_rule);
			query.bindValue(":TIMERULE", user.sendUser.iTimeRule);
			query.bindValue(":KEEPDIR", user.sendUser.bKeepDir);
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
    MyDataBase t_oDb;

    try
    {
        if (!t_oDb.connect())
        {
            return false;
        }
        QSqlQuery &query(t_oDb.sqlquery());
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
    MyDataBase t_oDb;

    try
    {
        if (!t_oDb.connect())
        {
            return false;
        }
        QSqlQuery &query(t_oDb.sqlquery());
        QString sql = QString("REPLACE INTO T_DIR_COL(DIRID, DIRNAME, ENABLE, COLLECTTYPE, FTPTRANSFERMODE, FTPCONNECTMODE, RLTVPATH, DISPATCH,"
                              "FILETEMPLATE, SUBDIRTEMPLATE, SUBDIRCHECK, MOVEFLAG, COLTIMERANGE, RECORDLATESTTIME, COMPARE_CONTENT, LOGINUSER, LOGINPASS,"
                              "IP, PORT, CONNECTID)"
                              "VALUES(:DIRID, :DIRNAME, :ENABLE, :COLLECTTYPE, :FTPTRANSFERMODE, :FTPCONNECTMODE, :RLTVPATH, :DISPATCH,"
                              ":FILETEMPLATE, :SUBDIRTEMPLATE ,:SUBDIRCHECK, :MOVEFLAG, :COLTIMERANGE, :RECORDLATESTTIME, :COMPARE_CONTENT, :LOGINUSER, :LOGINPASS,"
                              ":IP, :PORT, :CONNECTID)");
        query.prepare(sql);
        query.bindValue(":DIRID", task.dirID);
        query.bindValue(":DIRNAME", task.dirName);
        query.bindValue(":ENABLE", task.enable);
        query.bindValue(":COLLECTTYPE", task.collectType);
        query.bindValue(":FTPTRANSFERMODE", task.ftp_transferMode);
        query.bindValue(":FTPCONNECTMODE", task.ftp_connectMode);
        query.bindValue(":RLTVPATH", task.rltvPath);
        query.bindValue(":DISPATCH", task.dispatch);
        query.bindValue(":FILETEMPLATE", task.fileTemplate);
        query.bindValue(":SUBDIRTEMPLATE", task.subDirTemplate);
        query.bindValue(":SUBDIRCHECK", task.subdirFlag);
        query.bindValue(":MOVEFLAG", task.moveFlag);
        query.bindValue(":COLTIMERANGE", task.col_timerange);
        query.bindValue(":RECORDLATESTTIME", task.recordLatestTime);
        query.bindValue(":COMPARE_CONTENT", task.compareContent);
        query.bindValue(":LOGINUSER", task.loginUser);
        query.bindValue(":LOGINPASS", task.loginPass);
        query.bindValue(":IP", task.ip);
        query.bindValue(":PORT", task.port);
		query.bindValue(":CONNECTID", task.connectId);

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
    MyDataBase t_oDb;

    try
    {
        if (!t_oDb.connect())
        {
            return false;
        }
        QSqlQuery &query(t_oDb.sqlquery());
        QString sql;
        bool res;

		sql = QString("SELECT CONNECTID FROM T_DIR_COL WHERE DIRID = '%1'").arg(dirID);
		res = query.exec(sql);
		QString strConnectId;
		if (res)
		{
			if (query.next())
			{
				strConnectId = query.value(0).toString();
			}	
		}

        // 先删除收集任务表
		sql = QString("delete from T_DIR_COL where DIRID = '%1'").arg(dirID);
        res = query.exec(sql);
        if (res)
        {
            // 再删除收集用户表
			sql = QString("delete from T_COL_USER where CONNECTID = '%3'").arg(strConnectId);
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
            //QSharedPointer<QSqlDatabase> autoclose(&oDataBase, &QSqlDatabase::close);
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
    MyDataBase t_oDb;

    if (!t_oDb.connect())
    {
        return false;
    }
    QSqlQuery &query(t_oDb.sqlquery());

    QString sql = QString("SELECT ID, NAME,TYPE,IP,PORT,USER,PASSWORD,TRANSFERMODE,DIR,QUARTZ,FILE,UNIT,VALUE,ACTIVE FROM T_DIR_CLEAR");
    bool res = query.exec(sql);
    if (res)
    {
        while (query.next())
        {
            ClearTask task;
            int index = 0;
            task.taskId = query.value(index++).toString();
            task.taskName = query.value(index++).toString();
            task.taskType = query.value(index++).toInt();
            task.ip = query.value(index++).toString();
            task.port = query.value(index++).toInt();
            task.user = query.value(index++).toString();
            task.password = query.value(index++).toString();
            task.transfermode = query.value(index++).toInt();
            task.taskDir = query.value(index++).toString();
            task.quartzRule = query.value(index++).toString();
            task.matchRule = query.value(index++).toString();
            task.clearUnit = query.value(index++).toInt();
            task.clearValue = query.value(index++).toInt();
            task.activeFlag = query.value(index++).toInt();
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
    MyDataBase t_oDb;

    if (!t_oDb.connect())
    {
        return false;
    }
    QSqlQuery &query(t_oDb.sqlquery());

    QString sql = QString("SELECT ID, NAME,TYPE,IP,PORT,USER,PASSWORD,TRANSFERMODE,DIR,QUARTZ,FILE,UNIT,VALUE,ACTIVE FROM T_DIR_CLEAR WHERE NAME = '%1'").arg(task.taskName);
    bool res = query.exec(sql);
    if (res)
    {
        if (query.next())
        {
            int index = 0;
            task.taskId = query.value(index++).toString();
            task.taskName = query.value(index++).toString();
            task.taskType = query.value(index++).toInt();
            task.ip = query.value(index++).toString();
            task.port = query.value(index++).toInt();
            task.user = query.value(index++).toString();
            task.password = query.value(index++).toString();
            task.transfermode = query.value(index++).toInt();
            task.taskDir = query.value(index++).toString();
            task.quartzRule = query.value(index++).toString();
            task.matchRule = query.value(index++).toString();
            task.clearUnit = query.value(index++).toInt();
            task.clearValue = query.value(index++).toInt();
            task.activeFlag = query.value(index++).toInt();

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
    MyDataBase t_oDb;

    if (!t_oDb.connect())
    {
        return false;
    }
    QSqlQuery &query(t_oDb.sqlquery());

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
    MyDataBase t_oDb;

    if (!t_oDb.connect())
    {
        return false;
    }
    QSqlQuery &query(t_oDb.sqlquery());
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
    MyDataBase t_oDb;
    try
    {
        if (!t_oDb.connect())
        {
            return false;
        }
        QSqlQuery &query(t_oDb.sqlquery());
        QString sql = QString("REPLACE INTO T_DIR_CLEAR(ID, NAME,TYPE,IP,PORT,USER,PASSWORD,TRANSFERMODE,DIR,QUARTZ,FILE,UNIT,VALUE,ACTIVE)"
                              "VALUES(:ID, :NAME,:TYPE,:IP,:PORT,:USER,:PASSWORD,:TRANSFERMODE,:DIR,:QUARTZ,:FILE,:UNIT,:VALUE,:ACTIVE)");
        query.prepare(sql);
        query.bindValue(":ID", task.taskId);
        query.bindValue(":NAME", task.taskName);
        query.bindValue(":TYPE", task.taskType);
        query.bindValue(":IP", task.ip);
        query.bindValue(":PORT", task.port);
        query.bindValue(":USER", task.user);
        query.bindValue(":PASSWORD", task.password);
        query.bindValue(":TRANSFERMODE", task.transfermode);
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
    MyDataBase t_oDb;
    try
    {
        if (!t_oDb.connect())
        {
            return false;
        }
        QSqlQuery &query(t_oDb.sqlquery());
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

bool DataBase::queryBaseInfo(GlobalConfig &io_globalConfig)
{
    // QSqlDatabase t_oDb;
    MyDataBase t_oDb;
    try
    {
        if (!t_oDb.connect())
        {
            return false;
        }
        QSqlQuery &query(t_oDb.sqlquery());
        //if (!ConnectDB(t_oDb))
        //{
        //    return false;
        //}
        //QSharedPointer<QSqlDatabase> autoclose(&t_oDb, &QSqlDatabase::close);
        //QSqlQuery query(t_oDb);
        QString sql;
        bool res;
        // 先删除收集任务表
        // sql = QString("SELECT THREADNUM, LOGPORT FROM T_GLOBALINFO WHERE PROGRAMID = %1").arg(MAGIC_NUM);
        sql = QString("SELECT THREADNUM, LOGPORT, ENABLELOG, BROADCASTPORT, ENABLEBROADCAST, ENABLEBROADCAST FROM T_GLOBALINFO WHERE PROGRAMID = :PROGRAMID");
        query.prepare(sql);
        query.bindValue(":PROGRAMID", MAGIC_NUM);
        res = query.exec();
        if (res)
        {
            if (query.next())
            {
                io_globalConfig.nThreadNum = query.value(0).toInt();
                io_globalConfig.nLogPort = query.value(1).toInt();
                io_globalConfig.bEnableLog = query.value(2).toBool();
                io_globalConfig.nBroadcastPort = query.value(3).toInt();
                io_globalConfig.bEnableBroadcast = query.value(4).toBool();
                m_oGConfig = io_globalConfig;
                return true;
            }
            else
            {
                io_globalConfig.nThreadNum = 4;
                io_globalConfig.nLogPort = 50001;
                io_globalConfig.nBroadcastPort = 50002;
                m_oGConfig = io_globalConfig;
                return false;
            }

            QSLOG_ERROR(QString("Error: %1").arg(query.lastError().text()));
        }
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("Error：") + QString(e.what()));
    }

    return false;
}


bool DataBase::updateBaseInfo(GlobalConfig &io_globalConfig)
{
    // QSqlDatabase t_oDb;
    MyDataBase t_oDb;
    try
    {
        if (!t_oDb.connect())
        {
            return false;
        }

        //if (!ConnectDB(t_oDb))
        //{
        //    return false;
        //}
        //QSharedPointer<QSqlDatabase> autoclose(&t_oDb, &QSqlDatabase::close);
        //QSqlQuery query(t_oDb);
        QSqlQuery &query(t_oDb.sqlquery());
        QString sql;
        bool res;

        // 先删除收集任务表
        // sql = QString("REPLACE INTO T_GLOBALINFO(PROGRAMID,THREADNUM, LOGPORT) VALUES(%1, %2)").arg(MAGIC_NUM).arg(threadNum);
        sql = QString("REPLACE INTO T_GLOBALINFO(PROGRAMID,THREADNUM, LOGPORT, ENABLELOG, BROADCASTPORT, ENABLEBROADCAST ) VALUES(:PROGRAMID,:THREADNUM, :LOGPORT, :ENABLELOG, :BROADCASTPORT, :ENABLEBROADCAST)");
        if (!query.prepare(sql))
        {
            QSLOG_ERROR(QString("Error: %1").arg(query.lastError().text()));
            return false;
        }

        query.bindValue(":PROGRAMID", MAGIC_NUM);
        query.bindValue(":THREADNUM", io_globalConfig.nThreadNum);
        query.bindValue(":LOGPORT", io_globalConfig.nLogPort);
        query.bindValue(":ENABLELOG", io_globalConfig.bEnableLog);
        query.bindValue(":BROADCASTPORT", io_globalConfig.nBroadcastPort);
        query.bindValue(":ENABLEBROADCAST", io_globalConfig.bEnableBroadcast);
        res = query.exec();
        if (res)
        {
            m_oGConfig = io_globalConfig;
            return true;
        }

        QSLOG_ERROR(QString("Error: %1").arg(query.lastError().text()));
    }
    catch (std::exception &e)
    {
        QSLOG_ERROR(QStringLiteral("Error：") + QString(e.what()));
    }
    return false;
}

MyDataBase::MyDataBase()
{
    m_pQuery = NULL;
    m_pDataBase = NULL;

    m_pDataBase = new QSqlDatabase();
    m_strConName = QUuid::createUuid().toString();
    *m_pDataBase = QSqlDatabase::addDatabase("QSQLITE", m_strConName);
    QString strDbPath = qApp->applicationDirPath() + "/config/Transfer.db";
    m_pDataBase->setDatabaseName(strDbPath);
}


bool MyDataBase::connect()
{
    if (!m_pDataBase->open())
    {
        QSLOG_ERROR(QStringLiteral("连接数据库失败：%1").arg(m_pDataBase->lastError().text()));
        return false;
    }
    else
    {
        m_pQuery = new QSqlQuery(*m_pDataBase);
        return true;
    }
}


MyDataBase::~MyDataBase()
{
    if (NULL != m_pQuery)
    {
        delete m_pQuery;
        m_pQuery = NULL;
    }

    if (NULL != m_pDataBase)
    {
        if (m_pDataBase->isOpen())
        {
            m_pDataBase->close();
        }
        delete m_pDataBase;
        m_pDataBase = NULL;
    }

    QSqlDatabase::removeDatabase(m_strConName);
}
