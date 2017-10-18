/**
  * @file   shareddirCollector.h
  * @brief  共享(本地)目录收集器.
  *
  *  这里是详细说明。
  * @author nriet
  * @par   Copyright (c):
  *          nriet.
  *         All Rights Reserved
  * @date   2016:04:07
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef shareddirCollector_H_
#define shareddirCollector_H_


#include "filewatcher.h"
#include "CollectorBase.h"
#include "commontypedef.h"
#ifdef WITH_QT5
#include <QtConcurrent/QtConcurrent>
#else
#include <QFuture>
#include <QtCore>
using namespace QtConcurrent;
#endif
#include <QList>

class CltDispatch;
class IFileWatcher;
class CurlFtp;

class SharedDirCollector : public CollectorBase
{
    Q_OBJECT
public:
    /**
     * @brief  默认构造函数
     */
    SharedDirCollector(CollectManager *pManager, QWaitCondition &in_oCond, QMutex &in_oLocker, int &in_iLogsize);

    /**
     * @brief  默认析构函数
     */
    virtual ~SharedDirCollector();

    /**
     * @brief  启动共享目录收集
     * @return int：启动成功返回0，失败返回-1
     */
    virtual int start();

    virtual int reStart();

public slots:
    virtual int stop();
    void onCheckDir(QSet<QString>);
    //void fileCreated(const QString &strFilePath);
public:
    /**
     * @brief  重写父类虚函数，被定时调用，查找新文件
     * @return void：无
     */
    virtual void getNewFiles();

    virtual void getAllFiles(FileInfoList &fileList, QString strPath);

    virtual bool testCollection();

    void syncTransfer();
    void getSynclessFiles(QString strDir, bool bSubdir);

    // 相同返回true，不同返回false
    bool compareWithDest(CurlFtp &oCurlFtp, const QFileInfo &fi, TransTask &tTask);

    /**
     * @brief  数据传输检索新文件
     * @param  QString strDir：目录路径
     * @param  FileInfoList & fileList：新文件列表
     * @param  bool bRoot：是否为根目录
     * @return void：无
     */
    void getNewFiles(QString strDir, FileInfoList &fileList, bool bRoot=false);

    /**
     * @brief  获取修改时间变化了的所有目录
     * @param  QString strDir：父目录
     * @param  QStringList & lstDir：子目录
     * @return void：无
     */
    void getNewDirs(QString strDir, QStringList &lstDir);

    /**
     * @brief  遍历lstDir，查找新文件
     * @param  const QStringList & lstDir：目录列表
     * @param  FileInfoList & fileList：文件列表
     * @return void：无
     */
    void getNewFilesFromDir(const QStringList &lstDir, FileInfoList &fileList);

    void getNewDirs2(QString strDir, QStringList &lstDir);
    void getNewFilesFromDir2(const QStringList &lstDir, FileInfoList &fileList);

    /**
     * @brief  数据传输系统收集数据
     * @return void：无
     */
    void GetNewFilesToTransfer();

    /**
     * @brief  通用数据收集
     * @return void：无
     */
    void CollectNewFiles();

    /**
     * @brief  连接共享目录
     * @param  const QString & strURL：共享目录路径
     * @param  const QString & strUser：用户名
     * @param  const QString & strPwd：密码
     * @return bool：连接成功返回true，否则返回false
     */
    //bool connectToWinRemote(QString strURL,const QString& strUser,const QString& strPwd);

    virtual void taskDone(bool bFlag, const FileInfo &file);

    bool m_bConnected;

    bool ProcessFile(const QString &strFileFullPath);

private:
    QSharedPointer<QObject> m_pTimerObject;
    QSharedPointer<CltDispatch> m_oProcessor;

    /// 实时监控对象，共享目录方式独有
    QSharedPointer<IFileWatcher> m_pWatcher;

    void Init();

    QMutex m_oMutex;
    QMutex m_threadNumMutex;

    //static void makeVtbFun(SharedDirCollector *pParent, FileInfo fileinfo);

    QList<QFuture<void> > m_threadFun;	///< 记录当前运行的标准化线程

    //QSharedPointer<FileWatcher> m_pFileWatcher;		///< 实时监控
};

#endif
