/**
  * @file   CollectorBase.h
  * @brief  收集器基类.
  *
  *  包含了三种不同类型的收集器，基于共享目录的目录收集器，基于FTP的远程目录收集器，基于TCP消息流的流收集器
  * @author 刘伯俊
  * @par   Copyright (c):
  *          刘伯俊.
  *         All Rights Reserved
  * @date   2016:04:07
  * @note   这里是注意事项。
  * @version 1.0
  */

#ifndef CollectorBase_h__
#define CollectorBase_h__

#include <string>
using namespace std;
#include <QString>
#include <QVector>
#include <QMap>
#include <QDir>
#include <QThread>
#include <QThreadPool>
#include <QMutex>
#include <QProcess>
#include <QWaitCondition>
#include <QUuid>
#include "commontypedef.h"
#include "ctkLog.h"
#include "../ctkProtocol/fileMsg.h"
#include "lateFileTime.h"
#include "transferSet.h"
#include "lastCollectTime.h"
#include "logprotocol.h"
#include "macro.h"

class IRadarBaseDataParse;

/**
  * @class <CollectorBase>
  * @brief  收集器基类.
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL CollectorBase : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief  默认构造函数
     */
    CollectorBase(QWaitCondition &in_oCond, QMutex &in_oLocker, int &in_iLogsize, QObject *pParent=NULL);

    /**
     * @brief  默认析构函数
     */
    virtual ~CollectorBase();

    /**
     * @brief  初始化
     */
    static void init();
    /**
    * @brief  释放资源
    */
    static void unit();

private:
    CollectorBase();

public:
    void setCollectTask(const CollectTask &task)
    {
        m_collectSet = task;
        m_bRun = (task.enable==1) ? true : false;
    }

    const CollectTask &getCollectTask() const
    {
        return m_collectSet;
    }

    QString getIdentify()
    {
        return m_oId.toString();
    }

    /**
     * @brief  设置收集的路径
     * @param  const QString & url 收集路径
     * @return void：无
     */
    void setUrl(const QString &url)
    {
        m_strUrl = url.trimmed();
        m_strUrl.replace("\\", "/");
    }


    /**
     * @brief  设置用户名密码
     * @param  const QString & user 用户名
     * @param  const QString & pwd 密码
     * @return void：无
     */
    void setUserAndPwd(const QString &user, const QString &pwd)
    {
        m_strUser = user.trimmed();
        m_strPassword = pwd.trimmed();
    }

    void setIP(const QString &strIP)
    {
        m_strIP = strIP.trimmed();
    }

    /**
     * @brief  启动监听的端口号
     * @param  uint port 端口号
     * @return void：无
     */
    void setListenPort(uint port)
    {
        m_iPort = port;
    }

    void setStation(QString strStation)
    {
        m_strStation = strStation;
    }

    /**
     * @brief  设置各项信息的提取规则
     * @param  const QString & capReg 信息提取规则
     * @return void：无
     */
    void setCapReg(const QString &capReg)
    {
        m_strCapRegular = capReg;
    }

    /**
     * @brief  设置站点信息提取规则索引
     * @param  int index 索引下标，从1开始
     * @return void：无
     */
    void setStationCapIndex(int index)
    {
        m_nStationCapIndex = index;
    }

    /**
     * @brief  设置文件时间提取规则索引
     * @param  int index 索引下标，从1开始
     * @return void：无
     */
    void setTimeCapIndex(int index)
    {
        m_nTimeCapIndex = index;
    }

    void setFileTypeCapIndex(int index)
    {
        m_nTypeCapIndex = index;
    }

    void setLastTimeFilePath(const QString &path)
    {
        m_strTctimePath = path;
    }


    /**
     * @brief  启动收集
     * @return int：启动成功返回true,失败返回false
     */
    virtual int start() = 0;

    virtual int reStart() = 0;

signals:
    // 打印日志
    //void showLog(const QString &dirName, const QString &info);
    void showLog(const CollectTask &task, const QString &info, int infoType);
    // 通知前台链路状态
    void taskState(const CollectTask&, int, int);

    void startGif(const QString&, bool);

    void begin();

    void showIdentify(QString id);

public slots:
    /**
     * @brief  停止收集
     * @return int：停止成功返回0，失败返回-1
     */
    virtual int stop() = 0;

    void onBegined();

    // 发送日志信号 infoType：1-文件发送成功，2-失败
    void emitLog(const QString &info, int infoType);

public:
    void doWork();
    //virtual void setEnable(bool bFlag) = 0;

    //void emitLog(const QString &dirName, const QString &info);


    /**
     * @brief  遍历监控目录，找出新文件，发送到消息队列中。
     *         用于定时调度的收集方式。
     * @return void：描述返回值
     */
    virtual void getNewFiles() = 0;

    virtual void getNewFiles(const CollectTask &in_oTask) = 0;

    virtual bool testCollection() = 0;

    virtual void taskDone(bool bFlag, const FileInfo &file) = 0;

    CollectTask m_collectSet;

    TaskUser m_tUser;	///< 需要分发的用户

    void showMsg(const QString &str);

    // 从数据库读取配置文件
    bool readSet();

signals:
    /**
     * @brief  通过信号的方式通知其他插件有新文件
     * @param  const QString & filefullpath
     * @return void：描述返回值
     */
    void newfile(const FileInfo &fileinfo);

    void newTransFile(const TransferSet &tset);

    void print(const QString &);

protected:
    /// 该收集路径文件的站名
    QString m_strStation;

    /// 需要收集的路径
    QString m_strUrl;

    /// 用户名
    QString m_strUser;
    /// 密码
    QString m_strPassword;
    /// IP地址
    QString m_strIP;
    /// 端口号
    uint m_iPort;

    //std::map<std::string, std::vector<CDataInfo> > m_queueInfo;

    //QString m_strID;	///< 目录ID（唯一），数据传输用

    //int m_nCollectType;	///< 0-收集后处理，1-收集后分发

    ///< 文件名信息提取规则
    QString m_strCapRegular;

    ///< 站点信息提取规则索引
    int m_nStationCapIndex;

    ///< 文件时间提取规则索引
    int m_nTimeCapIndex;

    ///< 文件类型提取规则索引
    int m_nTypeCapIndex;

public:
    //////////////////////////////////////////////////////////////////////////
    TransCollectTime *m_pTsctTime;		//当前目录ID对应的收集时间

    CollectTime	*m_pLastTime;			///< 当前任务对应的收集时间

public:
    /**
     * @brief  共享目录文件名过滤
     * @param  const QString & strFilePath 全路径
     * @param  FileInfo & fileInfo 组装后的文件类型
     * @return bool：匹配返回true,不匹配返回false
     */
    bool filterFileName(const QFileInfo &fi, const QString &strFilePath, FileInfo &fileInfo);

    // 根据源文件全路径，获取对应的目标文件路径
    QString getDestFilePath(const QString &srcFileFullPath, const QString &fileName, const CollectUser &cUser, const QDateTime &dt, bool enableTimeRule);

    /**
     * @brief  简述函数功能
     * @param  QString strName：文件名
     * @return bool：匹配返回true
     */
    bool filterFileName(FileInfo &fileInfo);

    bool filterFileName(const QString &fileName);

    /**
     * @brief  判断是否为新文件
     * @param  FileInfo & fileInfo
     * @return bool：描述返回值
     */
    bool IsNewFile(FileInfo &fileInfo);

    /**
     * @brief  查询目录的最后处理时间
     * @param  const QString & dburl 记录文件所在的地址
     * @param  const QString & dir 目录
     * @param  QString &latesttime 最后处理时间
     * @return void：无
     */
    void recordLatestTime(const QString &dburl, const QString &dir, QString &latesttime);

    /**
     * @brief  查询目录的上一次处理的文件的列表以及文件
     *         考虑到远程的ftp服务器的版本差异，获取远程的文件的时间不太靠谱
     *		   只能通过大小来进行比对，如果本次获取的大小与上一次的发生了差异，则可以认为文件被更新了
     * @param  const QString &url 需要查询的目录
     * @return QMap<QString, int>：文件列表以及对应的文件大小
     */
    QMap<QString, int> queryLatestFileSize(const QString &url);

    /**
     * @brief  更新目录的上一次处理的文件的列表以及文件
     *         考虑到远程的ftp服务器的版本差异，获取远程的文件的时间不太靠谱
     *		   只能通过大小来进行比对，如果本次获取的大小与上一次的发生了差异，则可以认为文件被更新了
     * @param  const QString &url 需要查询的目录
     * @param  QMap<QString, int>：文件列表以及对应的文件大小
     * @return bool：更新成功，返回true，失败返回false
     */
    bool updateLatestFileSize(const QString &url, const QMap<QString, int> &oFileSizeInfo);
    /**
     * @brief  记录目录的最后处理时间
     * @param  const QString & dburl 记录文件所在的地址
     * @param  const QString & dir 目录
     * @param  const QString & latesttime 最后处理时间
     * @return void：描述返回值
     */
    void updateLatestTime(const QString &dburl, const QString &dir, const QString & latesttime);

    /// 每个收集都在独立子线程中运行
    QThread m_oThread;

    bool m_bFinish;		///< true表示空闲，false表示工作中

public:
    /**
     * @brief  读取上次收集时间
     * @return bool：读取成功返回true
     */
    bool ReadLastTime();
    volatile bool m_bRun;								///< 运行标识


    bool testFileConnection(QString strUrl);

    bool testFtpConnection(const QString &ip, int port, const QString & lgUser, const QString & lgPass, int transfermode, int connectmode);

    /**
     * @brief  获取指定目录的最新目录列表
     * @return list<string>：文件列表
     */
    list<string> initilizeLatestFileLists(const QString &dir);

    /**
     * @brief  判断当前文件是否在最新时间文件列表中
     * @return list<string>：文件列表
     */
    bool containsFile(list<string> &files, const QString &file);

    /**
    * @brief  更新记录的时间列表
    * @return list<string>：文件列表
    */
    void updateLatestFileLists(const QString &dir, const list<string> &files);

    /**
    * @brief  检查当前子进程是否已经结束
    * @return list<string>：文件列表
    */
    bool checkProcessFinished(const QString &dirId);

    QString getTaskLockFilePath(const QString &dirId);


protected:
    static QMutex m_mutexTime;
    static bool m_bReadTime;
    //static StationTypeTimeList m_lstSTtime;		///< 记录各站各类型数据的上次收集时间
    static TransCollectTimeList m_lstTCtime;	///< 所有的目录收集时间，序列化用
    QString m_strTctimePath;					///< 传输收集时间配置文件路径
    static QString m_strSttimePath;				///< 单站收集时间配置文件路径
    //int m_nVtbCount;							///< Vtb标准化运行线程个数

    int m_nLineState;							///< 链路状态，0-正常，1-不正常

    QWaitCondition &m_oCond;
    QMutex &m_oLocker;
    int &m_iLogsize;

    RCF::RcfInitDeinit m_oRcfInit;
    RcfClient<I_LogPrint> *m_oRcfClient;

    // 当前收集器的唯一标识
    QUuid m_oId;

    //QSharedPointer<QProcess> m_oDataTransferPro;
    QMutex m_oTaskLocker;

};

#endif // CollectorBase_h__