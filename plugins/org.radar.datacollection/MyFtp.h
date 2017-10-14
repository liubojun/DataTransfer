#include "qftp.h"
#include <QFile>
#include <QStringList>
#include <string>
using namespace std;
#include "fileMsg.h"

class CollectorBase;

class MyFtp : public QObject
{
	Q_OBJECT
public:
	MyFtp(QObject *parent=0);
	MyFtp(CollectorBase *parent);
	~MyFtp();

	void setHostPort(const QString &strIP, const int &nPort);
	void setUserPwd(const QString &strUser, const QString &strPwd);
	void setDataPath(const QString &strPath);
	void getNewFile(FileInfoList &fileList);
	void connectToHost();
	bool m_bConnected;		///< 是否登录ftp服务器
signals:
	void done();	///< ftp收集完成后发射此信号
private slots:
	void ftpCmdStart(int);
	void ftpCmdFinish(int, bool);
	void ftpListInfo(const QUrlInfo &);
private:
	QString m_strIP;
	int m_nPort;
	QString m_strUser;
	QString m_strPwd;
	

	QString m_strPath;	//用于遍历目录

	QFtp *m_pFtp;
	QFile *m_pFile;

	FileInfoList *m_pFileList;

	CollectorBase *m_pParent;

	/**
	 * @brief  获取所有新文件
	 * @param  const QString & dirPath：数据目录
	 * @param  FileInfoList & fileList：新文件链表
	 * @return void：无
	 */
	//void getFiles(const QString &dirPath, FileInfoList &fileList);

	QString m_strCurDir;
	QStringList m_lstDirs;
	/**
	 * @brief  list下一个文件夹的所有文件
	 * @return void：无
	 */
	void scanNextDir();

	//TransCollectTime *m_pDirTimes;
// 	string m_strCurDirLastTime;
// 	string m_strCurDirNewTime;
	uint m_strCurDirLastTime;
	uint m_strCurDirNewTime;
};