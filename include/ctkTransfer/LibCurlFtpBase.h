#ifndef _LIBCURLFTPBASE_H_
#define _LIBCURLFTPBASE_H_

/************************************************************************/
/*                           类说明：封装libcurl                         */
/************************************************************************/

#include <QString>
#include <QIODevice>
#include <QDateTime>

#include <curl/curl.h>
#include "../macro.h"
#include "curlftp.h"


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
    FTP_FILE = 0,
    FTP_DIR = 1
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

class DLL_EXPORT_CLASS_DECL FtpBase
{
public:

    FtpBase();

    virtual ~FtpBase();

    // 进入到制定的目录
    int cd(const QString &dir);

    // 关闭当前ftp连接
    int close();

    // 连接到ftp服务器
    int connectToHost(const QString &host, quint16 port = 21);

    int connectToHost(const QString &host, quint16 port /*= 21*/, const QString &user /*= QString()*/, const QString &pwd /*= QString()*/, int timeout  = 30);

    // 返回上一次错误结果
    QString errorString();

    // 是否启用curl内部打印信息
    int enableDebugLevel(bool flag = true);

    // 从ftp下载文件
    virtual int get(const QString &file, const QString &dir, TransferType type = Binary) = 0;

    // 从ftp下载文件(支持文件不落地，直接走内存)
    virtual int get(const QString &file, QIODevice *dev, TransferType type = Binary) = 0;

    // 获取ftp文件大小
    double getFileSize(const QString &file);

    // 获取文件目录列表
    QList<CFileInfo> list(const QString &dir = QString());

    // 获取文件目录列表（递归遍历子目录）
    QList<CFileInfo> listRecursion(const QString &dir = QString());

    // 登陆到ftp服务器(30秒)
    int login(const QString &user = QString(), const QString &password = QString(), int timeout = 30);

    // 创建目录
    virtual int mkdir(const QString &dir) = 0;

    // 上传到目标文件
    virtual int	put(const QString & localFile, const QString & remoteFile, const QString &suffix = QString(".tmp"), TransferType type = Binary) = 0;

    // 上传到目标文件
    virtual int	put(const QByteArray & data, const QString & file, TransferType type = Binary) = 0;

    // 重命名
    virtual int rename(const QString &oldname, const QString &newname) = 0;

    // 删除文件
    virtual int remove(const QString &file) = 0;

    // 删除目录
    virtual int rmdir(const QString &dir) = 0;

    // 设置ftp传输模式（主被动）
    int setTransferMode(TransferMode mode);

    // 根据主机ip及端口组装url
    virtual QString makeUrl(const QString &host, quint16 port) = 0;

    // 调用CD命名进入的新目录
    QString makeUrl2(const QString &dir);

    // 获取当前所在的相对目录
    QString getCurrentUrl();

    // easy_perform准备工作
    void prepare();


    // 解析ftp LIST命令返回的字符串
    void parseMlsdInfo(const QString &rootPath, const QString &info, QList<CFileInfo> &fileList);

    // 解析4段分隔符的ftp
    void process4Ftp(CListInfo &oneInfo, const QStringList &lstParts);

    // 解析9段分隔符的ftp
    void process9Ftp(CListInfo &oneInfo, const QStringList &lstParts);

protected:
    // 内部维护curl句柄
    CURL *m_pCurlHandler;

    // 辅助curl句柄，主要用在获取文件属性等
    CURL *m_pSecCurlHandler;

    // 辅助curl句柄，主要用在获取文件属性等
    CURL *m_pThridCurlHandler;

    MemoryData listInfo;

    // 返回码
    CURLcode m_iRetCode;

    // 访问的ftp服务器ip
    QString m_strHost;

    // 访问的ftp服务器端口
    qint16 m_iPort;

    QList<QString> m_urlBody;
};

#endif