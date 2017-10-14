#ifndef IDistributeManager_H_
#define IDistributeManager_H_


#include <QtCore/QtCore>
#include <QThread>
#include <QtCore/QMutex>
#include <vector>
#include <QBuffer>
#include <QMap>
#include <QList>
#include "transFileInfo.h"
#include "IFileCrypt.h"
#include "transferSet.h"
// #include "qftp.h"
// #include "qurlinfo.h"
using namespace std;

class TransFileInfo;
class CurlFtp;
struct MemoryData;


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
        //m_mPathFileList     = rhs.m_mPathFileList   ;
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
        //m_mPathFileList     = rhs.m_mPathFileList   ;

        return *this;
    }
    ~SendAcctInfo() {}
    QString m_strSendID;
    QString m_strSendHost;			///<
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
    //QMap<QString, QMap<QString,QUrlInfo> > m_mPathFileList;	///< 目录对应的文件:目录、文件名、文件信息
};


class IDistributeManager : public QObject
{
    Q_OBJECT
public:
    virtual ~IDistributeManager() {};

signals:
    void tranState(bool bTrans);

public slots:
    virtual void AddSendTask(const TransferSet & tSet) = 0;
    virtual void deltUser(QStringList lstUser) = 0;

public:
    /**
    * @brief  设置父窗的指针，以便发送信号
    * @param  void * parent：父窗口指针
    * @param  bool sendMsg：是否向父窗口发送实时处理信息
    * @return void：无
    */
    virtual void SetParent(void *parent,bool sendMsg) = 0;

    virtual bool IsTranferring() = 0;
};


#endif
