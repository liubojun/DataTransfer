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
#include "LibCurlFtpBase.h"

class DLL_EXPORT_CLASS_DECL CFtp : public FtpBase
{
public:

    CFtp();

    virtual ~CFtp();

    // 从ftp下载文件
    virtual int get(const QString &file, const QString &dir, TransferType type = Binary);

    // 从ftp下载文件(支持文件不落地，直接走内存)
    virtual int get(const QString &file, QIODevice *dev, TransferType type = Binary);

    // 获取文件目录列表
    //virtual QList<CFileInfo> list(const QString &dir = QString());

    // 获取文件目录列表（递归遍历子目录）
    //virtual QList<CFileInfo> listRecursion(const QString &dir = QString());

    // 创建目录
    virtual int mkdir(const QString &dir);

    // 上传到目标文件
    virtual int	put(const QString & localFile, const QString & remoteFile, const QString &suffix = QString(".tmp"), TransferType type = Binary);

    // 上传到目标文件
    virtual int	put(const QByteArray & data, const QString & file, TransferType type = Binary);

    // 重命名
    virtual int rename(const QString &oldname, const QString &newname);

    // 删除文件
    virtual int remove(const QString &file);

    // 删除目录
    virtual int rmdir(const QString &dir);

    // 根据主机ip及端口组装url
    virtual QString makeUrl(const QString &host, quint16 port);
};

#endif