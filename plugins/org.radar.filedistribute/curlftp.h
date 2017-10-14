/**
  * @file   curlftp.h
  * @brief  基于libcurl实现ftp相关功能
  *
  *  下载、上传
  * @author wjun
  * @par   Copyright (c):
  *          wjun.
  *         All Rights Reserved
  * @date   2016:06:29
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef curlftp_H_
#define curlftp_H_

#include "fileMsg.h"
#include "curl/curl.h"
#include <QObject>
#include <iostream>
using namespace std;

/// 在内存中缓存文件内容（加密压缩需要）
struct MemoryData
{
    char *memdata;		///< 起始地址
    size_t size;		///< 有效大小
    size_t capacity;	///< 申请的空间大小

    MemoryData()
    {
        memdata = NULL;
        size = 0;
        capacity = 0;
    }

    ~MemoryData()
    {
        free(memdata);
    }
};

// 存储文件
struct FileData
{
    const char *filename;
    FILE *stream;

    FileData()
    {
        filename = NULL;
        stream = NULL;
    }
};

class SignalObject : public QObject
{
    Q_OBJECT
public:
    virtual ~SignalObject() {}
signals:
    void progress(int id, qint64 dltotal, qint64 dlnow, qint64 ultotal, qint64 ulnow, double dTime);
public:
    void emitProgress(CURL *curl, qint64 dltotal, qint64 dlnow, qint64 ultotal, qint64 ulnow, double dTime);
};


class CurlFtp : public QObject
{
    Q_OBJECT
public:
    CurlFtp();
    ~CurlFtp();

    void setHostPort(const QString &strIP, const int &nPort);
    void setUserPwd(const QString &strUser, const QString &strPwd);
    void setRootPath(const QString &strPath);
    //void getNewFiles(FileInfoList &fileList);

    /**
     * @brief  从ftp下载文件
     * @param  const char * url
     * @param  const char * user_pwd
     * @param  MemoryData * memData
     * @return int：无
     */
    int downloadFile(const char *url, const char *user_pwd, MemoryData *memData);

    /**
     * @brief  从ftp或file下载到文件
     * @param  const char * url
     * @param  const char * user_pwd
     * @param  FileData * fileData
     * @return int：无
     */
    int downloadFile(const char *url, const char *user_pwd, FileData *fileData);

    /**
     * @brief  上传文件到ftp
     * @param  const char * url
     * @param  const char * user_pwd
     * @param  MemoryData * memData
     * @param  const string & filename
     * @return int：无
     */
    int uploadFile(const char *url, const char *user_pwd, MemoryData *memData, const string &filename);
    /**
     * @brief  续传文件
     * @param  const char * url：包含相对根路径的url
     * @param  const char * user_pwd：用户名、密码
     * @param  MemoryData * memData：文件内容
     * @param  const string & filename：最终文件名
     * @return int：成功返回0
     */
    int conputFile(const char *url, const char *user_pwd, MemoryData *memData, const string &filename);
    /**
     * @brief  删除ftp服务器上一个文件
     * @param  const char * url：包含文件路径的url
     * @param  const char * user_pwd：用户名、密码
     * @param  const string & filename：文件名
     * @return int：返回0
     */
    int deleteFile(const char *url, const char *user_pwd, const string &filename);

    //int downloadFileFromDir(const char *url, MemoryData *memData);

    int conputFileToDir(const char *url, const char *user_pwd, MemoryData *memData, const string &filename);
    int uploadFileToDir(const char *url, const char *user_pwd, MemoryData *memData, const string &filename);

    int uploadFileToDir(const char *url, const char *user_pwd, const string &filename, const char *localPath);
    int conputFileToDir(const char *url, const char *user_pwd, const string &filename, const char *localPath);

    int uploadFileToFtp(const char *url, const char *user_pwd, const string &filename, const char *localPath);
    int conputFileToFtp(const char *url, const char *user_pwd, const string &filename, const char *localPath);

    int renameFile(const char *url, const char *user_pwd, const string &oldname, const string &newname);

    /**
     * @brief  获取远端文件是否存在和大小
     * @param  const char * url：包含文件全路径的url
     * @param  const char * user_pwd：用户名、密码
     * @param  double & fileSize：文件大小
     * @return bool：true表示存在，false表示不存在
     */
    bool getFileInfo(const char *url, const char *user_pwd, const string &filename, double &fileSize);

    bool getFileSize(const char *url, const char *user_pwd, const string &filename, double &fileSize);

    bool connectToHost(const char *url, const char *user_pwd);

signals:
    void done();	///< ftp收集完成后发射此信号
    //void progress(qint64 dltotal, qint64 dlnow, qint64 ultotal, qint64 ulnow);

private:
    string m_strIP;
    int m_nPort;
    string m_strUser;
    string m_strPwd;
    string m_strRoot;

    //bool connectToHost();
    bool m_bConnected;			///< 是否登录ftp服务器

    QStringList m_lstDirs;		///< 等待遍历的目录
    QString m_strCurDir;		///< 当前ftp目录
    string m_strCurDirLastTime;	///< 当前目录记录的最后修改时间
    string m_strCurDirNewTime;	///< 当前目录现在的最后修改时间

public:
    //static CurlFtp *m_sCurlFtp;		/// 感觉有问题??
    //void emitProgress(qint64 dltotal, qint64 dlnow, qint64 ultotal, qint64 ulnow);
    uint m_nPgtime;				///< 进度条间隔
    CURL	*m_pCurl;
};

#endif