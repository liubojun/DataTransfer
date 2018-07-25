/**
  * @file   ftpCollector.h
  * @brief  ftp方式收集数据.
  *
  *  这里是详细说明。
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:05:24
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef sftpCollector_H_
#define sftpCollector_H_

#include "ftpCollector.h"
#include "LibCurlSFtp.h"


class DLL_EXPORT_CLASS_DECL SFtpCollector : public FtpCollector
{
    Q_OBJECT
public:
    SFtpCollector(QWaitCondition &in_oCond, QMutex &in_oLocker, int &in_iLogsize);

    ~SFtpCollector();

public:
    virtual void getNewFiles(const CollectTask &in_oTask);

	virtual void resetFtpInstance();
};

#endif
