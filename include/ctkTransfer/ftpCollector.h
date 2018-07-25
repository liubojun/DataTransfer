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

#ifndef ftpCollector_H_
#define ftpCollector_H_

#include "CollectorBase.h"
#include "LibCurlFtp.h"



class CDirRecord;

class DLL_EXPORT_CLASS_DECL FtpCollector : public CollectorBase
{
    Q_OBJECT
public:
    FtpCollector(QWaitCondition &in_oCond, QMutex &in_oLocker, int &in_iLogsize);
    ~FtpCollector();

    virtual int start();

    virtual int reStart();

public slots:
    virtual int stop();

    /**
     * @brief  删除收集实例
     * @return void
     */
    virtual void deleteSelf();

    // 接收到子进程退出消息
    void stoprcv(int exitcode, QProcess::ExitStatus status);
public:
    virtual void getNewFiles();

    virtual void getNewFiles(const CollectTask &in_oTask);

	void getNewFiles(const CollectTask &in_oTask, QSharedPointer<FtpBase> pFtpPtr);

    virtual bool testCollection();

    virtual void taskDone(bool bFlag, const FileInfo &file);

    // 相同返回true，不同返回false
    bool compareWithDest(QSharedPointer<FtpBase> &oCurlFtp, const CFileInfo &fi, TransTask &tTask);

    void setProcess(QProcess *in_pro);

	virtual void resetFtpInstance();

public slots:
    void ftpDone(const QList<CFileInfo> &files, CDirRecord &io_record);

    void readOutput();

public:
    QSharedPointer<QObject> m_pTimerObj;

    bool m_bChildProcessRunning;	// 子进程运行标识

    QProcess *m_pro;

    bool m_bBeingDeleted;   // 设置删除标识

	QSharedPointer<FtpBase> m_pFtp;
};

#endif
