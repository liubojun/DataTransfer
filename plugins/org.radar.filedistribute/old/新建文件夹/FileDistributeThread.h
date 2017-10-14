#ifndef FileDistributeThread_H_
#define FileDistributeThread_H_

#include "IDistributeManager.h"

class DistributeTaskThread;

/**
  * @class <FileDistributeThread>
  * @brief  数据分发线程
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
 */
class FileDistributeThread : public IDistributeManager
{
    Q_OBJECT
public:
    FileDistributeThread(void *parent = 0);
    ~FileDistributeThread();
    FileDistributeThread(const FileDistributeThread &);
    FileDistributeThread& operator=(const FileDistributeThread &);
    bool hasTask()
    {
        return m_hasTask;
    }
    void stop();
    virtual void SetParent(void *parent,bool sendMsg);
    void setFileCrypt(IFileCrypt *pFileCrypt);
    void setSendMsg(bool r_bSend);

    void setCollectAcctInfo( const QString& r_id
                             ,const QString& r_host
                             ,const QString& r_user
                             ,const QString& r_passwd
                             ,int r_port
                             ,const QString& r_rltvPath);

    /**
     * @brief  添加分发用户信息
     * @param  const vector<SendAcctInfo> & r_vSendAcctInfo：分发用户信息
     * @return void：无
     */
    void setSendAcctInfo(const vector<SendAcctInfo>& r_vSendAcctInfo);
    /**
     * @brief  添加新的分发任务
     * @param  const vector<TransFileInfo> & r_fileList：分发任务列表
     * @return void：无
     */
    //void addTransFileInfo(const vector<TransFileInfo>& r_fileList);
    void getTransFileInfo(vector<TransFileInfo>& r_fileList);

    //////////////////////////////////////////////////////////////////////////
public slots:
    virtual void AddSendTask(const TransferSet & tSet);

    /**
    * @brief  停止一个分发任务
    * @param  const string & dirId：收集目录ID
    * @return void：无
    */
    //virtual void StopSendTask(const string &dirId);
    //////////////////////////////////////////////////////////////////////////

signals:
    void doTask();
    void transProgress(const TransFileInfo r_transInfo);

protected:
    void run();

private slots:
    /**
     * @brief  当前文件处理完成
     * @param  bool hasError：成功为true，失败为false
     * @param  const QString & errInfo：失败信息
     * @return void：无
     */
    void sendDone(bool hasError=false, QString errInfo="");
    /**
     * @brief  处理分发
     * @return void：描述返回值
     */
    void processData();

    /**
     * @brief  共享目录方式读取文件
     * @param  const QString & r_fileName：文件全路径
     * @param  QSharedPointer<QBuffer> & r_pBuffer：内容缓冲区
     * @param  QString & r_errorInfo
     * @return bool：描述返回值
     */
    //bool loadFileToBuffer(const QString& r_fileName, QSharedPointer<QBuffer> &r_pBuffer, QString& r_errorInfo);
    //bool writeFileFromBuffer(const QString& r_fileName, QSharedPointer<QBuffer> & r_pBuffer, QString& r_errorInfo);
    bool writeKeyIv(const QString &fileName, KeyIv &keyiv);

    //bool collectFromDir();
    /**
     * @brief  共享目录方式分发数据
     * @return void：无
     */
    //void sendToDir();

    /**
     * @brief  从共享目录收集数据
     * @param  MemoryData & mData：文件内容
     * @return bool：成功返回true，否则false
     */
    bool collectFromDir(MemoryData &mData);

    /**
     * @brief  发送数据到共享目录
     * @param  MemoryData & mData：文件内容
     * @return void：无
     */
    bool sendToDir(MemoryData &mData);

    /**
     * @brief  将内存数据写入文件
     * @param  const QString & fileName：文件全路径
     * @param  const MemoryData & mData：内存数据
     * @return bool：成功返回true，否则返回false
     */
    bool writeFileFromBuffer(const QString &fileName, const MemoryData &mData);

    /**
     * @brief  分发数据到FTP
     * @param  MemoryData & mData：文件内容
     * @return void：无
     */
    bool sendToFtp(MemoryData &mData);

    bool connectToWinRemote(const QString& r_resource,const QString& r_user,const QString& r_passwd);
    bool loginToSendRemote();
    bool loginToCollectRemote();
    void dealUnsupportCollectWay();
    void dealUnsupportSendWay();

    QString getHostFromFileName(const QString& r_strFullPathFileName);

    /**
     * @brief  获取收集相对路径
     * @return QT_NAMESPACE::QString：返回相对路径
     */
    QString getCollectSubPath();
    QString getCurrentPath();
    /**
     * @brief  获取当前文件的上传路径
     * @note   根据是否保持原始目录结构返回路径
     * @return QT_NAMESPACE::QString：上传路径
     */
    QString getFtpCdPath();
    void dealSendError(const QString& r_errorInfo);
    QString getRightCutPath(const QString& r_path);
    //void ftpGetDataProgress(qint64 done, qint64 total);
    //void ftpPutDataProgress(qint64 done, qint64 total);
    void ftpTransProgress(qint64 dltotal, qint64 dlnow, qint64 ultotal, qint64 ulnow);
    void onFtpProgress(int id, qint64 dltotal, qint64 dlnow, qint64 ultotal, qint64 ulnow);
    QString getCurrentSendID()
    {
        return m_transFileList[m_currentIndex].m_strSendID;
    }
    void cryptAndPutFile();

public slots:
    void onTaskFinished(int id, int threadId);

    //////////////////////////////////////////////////////////////////////////
public:
    void dealError(TransFileInfo &fileInfo, QString &errorInfo);
    //  bool CryptCurFile(TransFileInfo &fileInfo, int &nIndex, KeyIv &keyiv);
    bool getUserInfo(const QString userID, SendAcctInfo &userInfo);
    /**
    * @brief  发送进度条
    * @param  const TransFileInfo：当前文件进度
    * @return void：无
    */
    void sendTransProgress(TransFileInfo &tsInfo);

public:
    volatile bool m_bRun;
    //////////////////////////////////////////////////////////////////////////

private:
    QSharedPointer<QMutex> m_mutex;
    //QFtp* m_ftpCollect;			///< 收集ftp
    //QFtp* m_ftpSend;			///< 分发ftp
    CurlFtp *m_pFtp;

    volatile bool m_stoped;		///< true表示停止运行
    volatile bool m_hasTask;
    volatile bool m_bDirConnected;
    volatile int  m_iCollectTryTimes;
    volatile bool m_bSendSignalToMainWindow;

    //QWidget *m_parent;
    void *m_parent;
    QString m_strCollectID;
    QString m_strRltvPath;			///< 当前任务文件的收集根路径
    QString m_strCollectHost;
    QString m_strCollectUserName;
    QString m_strCollectPasswd;
    int     m_iCollectPort;
    QString m_strLastSendID;//最近一次send 帐号
    QString m_strLastCdPath;///< 上次cd的路径

    QMap<QString, SendAcctInfo> m_sendUserAcct;		///< 记录所有用户信息
    QList<TransFileInfo> m_transFileList;
    long m_currentIndex;	///< 当前处理的文件在m_transFileList中的序号
    QSharedPointer<QMutex> m_mutexData;
    QMap<int,QString> m_sendCmdIdMap;///<发送指令序列
    QMap<int,QString> m_collectCmdIdMap;///<收集指令序列
    QMap<int,QString> m_testDirCmdIdMap;
    QList<QString>    m_lstNeedCreate;

    IFileCrypt	*m_pFileCrypt;		//压缩加密服务
    //bool CryptCurFile(void);		//加密压缩文件

    bool CryptCurFile(MemoryData &mData);	//加密压缩文件

    QMutex m_taskMutex;
    QThread m_oThread;
    int m_nCurThreadCount;
    int m_nMaxThreadCount;
    QMap<int, QSharedPointer<DistributeTaskThread>> m_mapIdThread;

    QVector<QSharedPointer<QThread>> m_vecThread;		///< 多线程分发
    QVector<bool>	m_vecWorkState;		///< 各线程工作状态，true-工作，false-休眠
};

#endif
