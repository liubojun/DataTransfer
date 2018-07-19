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