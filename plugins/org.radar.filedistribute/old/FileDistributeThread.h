#ifndef  __FILEDISTRIBUTETHREAD_H_20160518_
#define  __FILEDISTRIBUTETHREAD_H_20160518_

#include <QtCore/QtCore>
#include <QThread>
#include "qftp.h"
#include "qurlinfo.h"
#include <QtCore/QMutex>
#include <vector>
#include <QBuffer>
#include <QMap>
#include <QList>
#include "transFileInfo.h"
#include "IFileCrypt.h"

class TransFileInfo;

using namespace std;

/**
  * @class <SendAcctInfo>
  * @brief  分发用户信息类
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class SendAcctInfo
{
public:
    SendAcctInfo():m_bDirConnected(false),m_iSendTryTimes(0)
    {
        m_bConSend = true;
    }

    SendAcctInfo(const SendAcctInfo& rhs)
    {
        m_strSendID 		= rhs.m_strSendID 		;
        m_strSendHost 		= rhs.m_strSendHost 	;
        m_strSendUserName	= rhs.m_strSendUserName ;
        m_strSendPasswd 	= rhs.m_strSendPasswd 	;
        m_strRltvPath 		= rhs.m_strRltvPath 	;
        m_iSendPort 		= rhs.m_iSendPort 		;
        m_iSendWay 			= rhs.m_iSendWay 		;
        m_bEncrypt 			= rhs.m_bEncrypt 		;
        m_bCompress 		= rhs.m_bCompress 		;
        m_bKeepDir 			= rhs.m_bKeepDir 		;
        m_strCurrentDir     = rhs.m_strCurrentDir   ;
        m_keyiv				= rhs.m_keyiv			;
        m_bConSend			= rhs.m_bConSend		;
        m_mPathFileList     = rhs.m_mPathFileList   ;
    }
    SendAcctInfo& operator=(const SendAcctInfo& rhs)
    {
        m_strSendID 		= rhs.m_strSendID 		;
        m_strSendHost 		= rhs.m_strSendHost 	;
        m_strSendUserName	= rhs.m_strSendUserName ;
        m_strSendPasswd 	= rhs.m_strSendPasswd 	;
        m_strRltvPath 		= rhs.m_strRltvPath 	;
        m_iSendPort 		= rhs.m_iSendPort 		;
        m_iSendWay 			= rhs.m_iSendWay 		;
        m_bEncrypt 			= rhs.m_bEncrypt 		;
        m_bCompress 		= rhs.m_bCompress 		;
        m_bKeepDir 			= rhs.m_bKeepDir 		;
        m_strCurrentDir     = rhs.m_strCurrentDir   ;
        m_keyiv				= rhs.m_keyiv			;
        m_bConSend			= rhs.m_bConSend		;
        m_mPathFileList     = rhs.m_mPathFileList   ;

        return *this;
    }
    ~SendAcctInfo() {}
    QString m_strSendID;
    QString m_strSendHost;
    QString m_strSendUserName;
    QString m_strSendPasswd;
    int     m_iSendPort;
    int     m_iSendWay;		//分发方式（0-目录，1-FTP）
    QString m_strRltvPath;	//收集路径
    bool m_bDirConnected;
    int m_iSendTryTimes;
    bool m_bEncrypt;		//true-加密
    bool m_bCompress;		//true-压缩
    bool m_bKeepDir;		//true-创建对应的目录结构
    KeyIv	m_keyiv;		///< 加密使用的key和iv
    bool m_bConSend;		///< 是否断点续传
    QString m_strCurrentDir;
    QMap<QString, QMap<QString,QUrlInfo> > m_mPathFileList;	///< 目录对应的文件:目录、文件名、文件信息
};

class FileDistributeThread : public QThread
{
    Q_OBJECT

public:
    //FileDistributeThread(QWidget *parent = 0);
    FileDistributeThread(void *parent = 0);
    ~FileDistributeThread();
    FileDistributeThread(const FileDistributeThread &);
    FileDistributeThread& operator=(const FileDistributeThread &);
    bool hasTask()
    {
        return m_hasTask;
    }
    void stop();
    //void setParent(QWidget *parent,bool sendMsg);
    void setParent(void *parent,bool sendMsg);
    void setFileCrypt(IFileCrypt *pFileCrypt);
    void setSendMsg(bool r_bSend);

    void setCollectAcctInfo( const QString& r_id
                             ,const QString& r_host
                             ,const QString& r_user
                             ,const QString& r_passwd
                             ,int r_port
                             ,const QString& r_rltvPath)
    {
        m_strCollectID = r_id;
        m_strCollectHost = r_host;
        m_strCollectUserName = r_user;
        m_strCollectPasswd = r_passwd;
        m_iCollectPort = r_port;
        m_strRltvPath = r_rltvPath;

    }

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
    void addTransFileInfo(const vector<TransFileInfo>& r_fileList);
    void getTransFileInfo(vector<TransFileInfo>& r_fileList);
signals:
    void doTask();
    void transProgress(const TransFileInfo r_transInfo);
protected:
    void run();

private slots:
    /**
     * @brief  分发数据
     * @param  int cmdID
     * @param  bool hasError
     * @return void：无
     */
    void sendFile(int cmdID, bool hasError);
    void sendDone(int cmdID, bool hasError);
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
    bool loadFileToBuffer(const QString& r_fileName, QSharedPointer<QBuffer> &r_pBuffer, QString& r_errorInfo);
    bool writeFileFromBuffer(const QString& r_fileName, QSharedPointer<QBuffer> & r_pBuffer, QString& r_errorInfo);
    bool writeKeyIv(const QString &fileName, KeyIv &keyiv);

    bool collectFromDir();
    /**
     * @brief  共享目录方式分发数据
     * @return void：无
     */
    void sendToDir();

    bool connectToWinRemote(const QString& r_resource,const QString& r_user,const QString& r_passwd);
    bool loginToSendRemote();
    bool loginToCollectRemote();
    void dealUnsupportCollectWay();
    void dealUnsupportSendWay();
    void sendTransProgress(const TransFileInfo);
    QString getHostFromFileName(const QString& r_strFullPathFileName);
    void ftpCmdFinishCollect(int cmdID, bool err);
    void ftpCmdFinishSend(int cmdID, bool err);

    /**
     * @brief  转为ftp识别的编码
     * @param  const QString & InputStr：原始字符串
     * @return QT_NAMESPACE::QString：转换后的字符串
     */
    QString _ToSpecialEncoding(const QString &InputStr);

    /**
     * @brief  转为可识别编码
     * @param  const QString & InputStr：原始字符串
     * @return QT_NAMESPACE::QString：转换后的字符串
     */
    QString _FromSpecialEncoding(const QString &InputStr);


    QString getCollectSubPath();
    QString getCurrentPath();
    QString getFtpCdPath();
    void dealCollectError(const QString& r_errorInfo);
    void dealSendError(const QString& r_errorInfo);
    void testFTPDirExists(const QString& r_path);
    void testDirCmdFinish(int cmdID, bool err);
    QString getRightCutPath(const QString& r_path);
    void dealRawCommandReply(int replyCode, const QString & detail);
    void collectDataTransferProgress(qint64 done, qint64 total);
    void sendDataTransferProgress(qint64 done, qint64 total);
    QString getCurrentSendID()
    {
        return m_transFileList[m_currentIndex].m_strSendID;
    }
    void cryptAndPutFile();
private:

    QMutex* m_mutex;
    QFtp* m_ftpCollect;			///< 收集ftp
    QFtp* m_ftpSend;			///< 分发ftp
    volatile bool m_stoped;
    volatile bool m_hasTask;
    volatile bool m_bDirConnected;
    volatile int  m_iCollectTryTimes;
    volatile bool m_bSendSignalToMainWindow;

    //QWidget *m_parent;
    void *m_parent;
    QString m_strCollectID;
    QString m_strRltvPath;
    QString m_strCollectHost;
    QString m_strCollectUserName;
    QString m_strCollectPasswd;
    int     m_iCollectPort;
    QString m_strLastSendID;//最近一次send 帐号
    QString m_strLastCdPath;///< 上次cd的路径

    QMap<QString,SendAcctInfo> m_sendUserAcct;
    QList<TransFileInfo> m_transFileList;
    long m_currentIndex;	///< 当前处理的文件在m_transFileList中的序号
    QMutex* m_mutexData;
    QMap<int,QString> m_sendCmdIdMap;///<发送指令序列
    QMap<int,QString> m_collectCmdIdMap;///<收集指令序列
    QMap<int,QString> m_testDirCmdIdMap;
    QList<QString>    m_lstNeedCreate;

    IFileCrypt	*m_pFileCrypt;		//压缩加密服务
    bool CryptCurFile(void);		//加密压缩文件

//////////////////////////////////////////////////////////////////////////
// 断点续传功能
private slots:
    /**
     * @brief  依次返回当前文件夹的文件信息
     * @param  const QUrlInfo & info：文件信息
     * @return void：无
     */
    void ftpSendListInfo(const QUrlInfo &info);
//////////////////////////////////////////////////////////////////////////
};

#endif
