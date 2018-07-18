#ifndef DistributeFile_H_
#define DistributeFile_H_


#include <QObject>
#include "curlftp.h"
#include "commontypedef.h"
#include "CollectorBase.h"
#include "LibCurlSFtp.h"


class DistributeFile : public QObject
{
    Q_OBJECT
public:
    DistributeFile(CollectorBase *pBase);
    virtual ~DistributeFile();

	/**
	 * @brief  文件分发
	 * @param  TransTask &task：文件信息
	 * @return bool：成功返回true
	 */
	bool transfer(TransTask &task, CurlFtp &oCurlFtp);

	/**
	 * @brief  文件分发
	 * @param  TransTask &task：文件信息
	 * @return bool：成功返回true
	 */
	bool transfer(TransTask &task, SFtp &oSFtpSource, SFtp &oSFtpDest);

    int getCurlAddr();
    TransTask m_fileInfo;

    //SignalSender m_signalObj;
signals:
    void emitLog(const QString &info, int infoType);

    void emitBroadCast(const QString &srcfile, const QString &dstfile);
protected:
    /**
     * @brief  发送文件到共享目录
     * @param  const char * fullPath：源文件全路径
     * @param  KeyIv & keyiv：加密压缩信息
     * @param  SendAcctInfo & userInfo：分发用户信息
     * @return bool：成功返回true
     */
    bool sendToDir(const char *fullPath, TransTask &task);
    bool sendToFtp(const char *fullPath, TransTask &userInfo);
    //bool sendToDir(const char *fullPath, TransTask &task, int userIndex);
    //bool sendToFtp(const char *fullPath, TransTask &userInfo, int userIndex);

    //bool writeKeyIv(const QString &fileName, KeyIv &keyiv);

    //bool CrypCurtFile(bool bComp, bool bCrypt, int &nIndex);

    bool CrypCurtFile(const QString &srcFilePath, bool bComp, bool bCrypt, int &nIndex, QString &dstFilePath);

    bool loginToSendRemote(UserInfo &userInfo);

    QString getHostFromFileName(const QString& r_strFullPathFileName);

    QString getCurrentPath(const UserInfo &userInfo);

    QString getCollectSubPath();

    void taskFinish();

private:
    CurlFtp *m_pCurlFtp;
    // QSharedPointer<CurlFtp>	m_pFtp;
    CollectorBase	*m_pBase;
    //IFileCrypt	*m_pFileCrypt;
    //KeyIv		m_keyiv[4];		///< 加密使用的key和iv

    QString		m_downloadPath;
};

#endif