#ifndef _LIBCURLSFTP_H_
#define _LIBCURLSFTP_H_

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

class DLL_EXPORT_CLASS_DECL SFtp : public FtpBase
{
public:

    SFtp();

    virtual ~SFtp();

    // 从ftp下载文件
    virtual int get(const QString &sourceFile, const QString &downloadFile, TransferType type = Binary);

    // 从ftp下载文件(支持文件不落地，直接走内存)
    virtual int get(const QString &file, QIODevice *dev, TransferType type = Binary);


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

    QString makeUrl(const QString &host, quint16 port);
};

#endif