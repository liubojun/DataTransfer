#ifndef _LIBCURLFTP_H_
#define _LIBCURLFTP_H_

/************************************************************************/
/*                           类说明：封装libcurl                         */
/************************************************************************/

#include <QString>
#include <QIODevice>
#include <QDateTime>

#include <curl/curl.h>
#include "../macro.h"
#include "curlftp.h"

namespace FTP
{
	// FTP传输模式
	enum TransferMode
	{
		Active = 0,
		Passive = 1
	};

	enum TransferType
	{
		Binary = 0,
		Ascii = 1
	};

	enum FileType
	{
		FILE = 0,
		DIR = 1
	};

	class DLL_EXPORT_CLASS_DECL CFileInfo
	{
	public:
		QString name;		// 文件名
		QString path;		// 全路径
		FileType type;		// 文件类型
		QDateTime time;		// 最后修改时间
		qint64 size;		// 文件大小（单位：字节）
	};

	class DLL_EXPORT_CLASS_DECL CFtp
	{
	public:

		CFtp();

		virtual ~CFtp();

		// 进入到制定的目录
		int cd(const QString &dir);

		// 关闭当前ftp连接
		int close();

		// 连接到ftp服务器
		int connectToHost(const QString &host, quint16 port = 21);

		// 返回上一次错误结果
		QString errorString();

		// 是否启用curl内部打印信息
		int enableDebugLevel(bool flag = true);

		// 从ftp下载文件
		int get(const QString &file, const QString &dir, TransferType type = Binary);

		// 获取文件目录列表
		QList<CFileInfo> list(const QString &dir = QString());

		// 登陆到ftp服务器
		int login(const QString &user = QString(), const QString &password = QString());

		// 创建目录
		int mkdir(const QString &dir);

		// 上传到目标文件
		int	put(QIODevice * dev, const QString & file, TransferType type = Binary);

		// 上传到目标文件
		int	put(const QByteArray & data, const QString & file, TransferType type = Binary);

		// 重命名
		int rename(const QString &oldname, const QString &newname);

		// 删除文件
		int remove(const QString &file);

		// 删除目录
		int rmdir(const QString &dir);

		// 设置ftp传输模式（主被动）
		int setTransferMode(TransferMode mode);

	private:
		// 根据主机ip及端口组装url
		QString makeUrl(const QString &host, quint16 port);

		// 调用CD命名进入的新目录
		QString makeUrl(const QString &dir);

		// 获取当前所在的相对目录
		QString getCurrentUrl();

		// easy_perform准备工作
		void prepare();

		void parseMlsdInfo(const QString &rootPath, const QString &info, QList<CFileInfo> &fileList);
		void process4Ftp(CListInfo &oneInfo, const QStringList &lstParts);
		void process9Ftp(CListInfo &oneInfo, const QStringList &lstParts);

	private:
		// 内部维护curl句柄
		CURL *m_pCurlHandler;

		MemoryData listInfo;

		// 返回码
		CURLcode m_iRetCode;

		// 访问的ftp服务器ip
		QString m_strHost;

		// 访问的ftp服务器端口
		qint16 m_iPort;

		QList<QString> m_urlBody;
	};
}
#endif