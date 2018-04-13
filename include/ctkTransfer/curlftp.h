/**
  * @file   curlftp.h
  * @brief  基于libcurl实现ftp收集数据功能.
  *
  *  这里是详细说明。
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

#include "CollectorBase.h"
#include "fileMsg.h"
#include "curl/curl.h"
#include "subdirfilter.h"

class CollectorBase;
class CDirRecord;
//////////////////////////////////////////////////////////////////////////
struct CListInfo
{
    QString	strFileName;
    QString	strMdfyTime;
    int		nType;			// 1-文件，2-目录
    qint64	nFileSize;

    CListInfo()
    {
        nType = 0;
        nFileSize = 0;
    }
};

// 存储文件
struct FileData
{
    const char *filename;
    FILE *stream;
    //QSharedPointer<FILE> autoclose;
    FileData()
    {
        filename = NULL;
        stream = NULL;
    }
    ~FileData()
    {

    }
};

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
        if (NULL != memdata)
        {
            free(memdata);
        }

    }
};
// int get_file_size(FILE *file)
// {
//     int size = 0;
//     fseek(file, 0L, SEEK_END);
//     size = ftell(file);
//     fseek(file, 0L, SEEK_SET);
//     return size;
// }
//
// void curl_set_upload_opt(CURL *curl, const char *url, const char *user_key, FILE *file)
// {
//     curl_easy_setopt(curl, CURLOPT_URL, url);
//     curl_easy_setopt(curl, CURLOPT_USERPWD, user_key);
//     curl_easy_setopt(curl, CURLOPT_READDATA, file);
//     curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);
//     curl_easy_setopt(curl, CURLOPT_INFILESIZE, get_file_size(file));
//     curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
//     //	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
// }

// void curl_set_download_opt(CURL *curl, const char *url, const char *user_key, FILE *file)
// {
//     curl_easy_setopt(curl, CURLOPT_URL, url);
//     curl_easy_setopt(curl, CURLOPT_USERPWD, user_key);
//     curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
//     //	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
// }
static size_t WriteInMemoryFun(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;		//本次回调获取的数据
    struct MemoryData *mem = (struct MemoryData *)userp;

    mem->memdata = (char *)realloc(mem->memdata, mem->size + realsize + 1);
    if (mem->memdata == NULL)
    {
        /* out of memory! */
        QSLOG_ERROR("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memdata[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memdata[mem->size] = 0;

    return realsize;
}

static size_t UploadFromMemFun(void *ptr, size_t size, size_t nmemb, void *stream)
{
    struct MemoryData *mem = (struct MemoryData *)stream;
    size_t tocopy = size * nmemb;

    if (tocopy < 1 || mem->size == 0)
    {
        return 0;
    }

    if (mem->size < tocopy)
    {
        tocopy = mem->size;
    }

    memcpy(ptr, mem->memdata, tocopy);
    mem->memdata += tocopy;
    mem->size -= tocopy;

    return tocopy;
}

// 下载到文件
static size_t WriteInFileFun(void *buffer, size_t size, size_t nmemb, void *stream)
{
    struct FileData *out = (struct FileData *)stream;
    if (out && !out->stream)	// 若没有打开文件
    {
        out->stream = fopen(out->filename, "wb");
        if (NULL == out->stream)		// 打开文件失败
        {
            QSLOG_ERROR(QStringLiteral("打开文件：%1失败").arg(out->filename));
            return -1;
        }
        // 设置自动释放
        //out->autoclose = QSharedPointer<FILE>(out->stream, fclose);
    }

    return fwrite(buffer, size, nmemb, out->stream);
}

// 从文件上传
static size_t ReadFromFile(void *buffer, size_t size, size_t nmemb, void *stream)
{
    curl_off_t nread;
    /* in real-world cases, this would probably get this data differently
    as this fread() stuff is exactly what the library already would do
    by default internally */
    size_t retcode = fread(buffer, size, nmemb, (FILE *)stream);

    nread = (curl_off_t)retcode;

    //QSLOG_DEBUG(QString("*** We read %1 bytes from file").arg(nread));
    return retcode;

    //size_t retcode = fread(buffer, size, nmemb, (FILE *)stream);
    //return retcode;
}

static size_t throw_away(void *ptr, size_t size, size_t nmemb, void *data)
{
    (void)ptr;
    (void)data;

    return (size_t)(size * nmemb);
}


static size_t getDataFun(void *ptr, size_t size, size_t nmemb, void *stream)
{
//     char *out = (char *)(*stream);
//     size_t nLen = size * nmemb + 1;
//     out = new char[nLen](0);
//     memcpy(out, ptr, nLen-1);
//     return nLen-1;

    return 0;
}



typedef struct
{
    QString dir;	// 目录
    int level;		// 所属子目录层级，根目录为0，子目录依次递增
} DIRLEVEL;

//////////////////////////////////////////////////////////////////////////

class CurlFtp : public QObject
{
    Q_OBJECT
public:
    CurlFtp(CollectorBase *pBase=0);
    ~CurlFtp();

    void setHostPort(const QString &strIP, const int &nPort);
    void setUserPwd(const QString &strUser, const QString &strPwd);
    void setRootPath(const QString &strPath);

    // 设置ftp传输模式，0为binary，1为ascii
    void setFtpTransferMode(int mode);

    // 设置ftp连接模式，0为被动，1为主动
    void setFtpConnectMode(int mode);

    // 是否需要递归遍历子目录标识
    void setSubDirFlag(bool flag);

    /**
     * @brief: 设置子目录模板ID
     * @param: const QString & templateId 模板ID
     * @return:void 无
     */
    void setSubDirTemplateId(const QString &templateId);

    int getNewFiles(FileInfoList &fileList, CDirRecord &in_record);

    void listFiles(const string &strDir, FileInfoList &fileList, CDirRecord &in_oRecord);

    QString parseMlsdInfo(const QString &rootPath, const QString &info, FileInfoList &fileList, QList<DIRLEVEL> &dirList, CDirRecord &in_record, bool bFtpSupportMSDL);

    /**
     * @brief 连接远程目录
     * @param  const char * url 远程地址
     * @param  const char * user_pwd 访问的用户名密码
     * @param  const char * timeout 超时时间，默认使用系统自带超时
     * @return bool：连接成功返回true,失败返回false
     */
    bool connectToHost(const char *url, const char *user_pwd, int timeout = -1);

    bool TestConnection(const char *url, const char *user_pwd);

    /**
     * @brief 初始化curl，将原有构造函数部分的内容放到此函数中，便于每次都进行curl的连接与断开
     * @return void：无
     */
    void initcurl();

    /**
    * @brief 释放curl，将原有析构函数部分的内容放到此函数中，便于每次都进行curl的连接与断开
    * @return void：无
    */
    void uninitcurl();
signals:
    void done();	///< ftp收集完成后发射此信号

    // 发送日志信号 infoType：1-文件发送成功，2-失败
    void emitLog(const QString &info, int infoType);

private:
    int setCommOpt();

    string m_strIP;
    int m_nPort;
    string m_strUser;
    string m_strPwd;
    string m_strRoot;
    bool m_subDirFlag;	// 是否需要递归遍历子目录

    QString m_strSubDirTemplate;	// 子目录匹配模板ID

    // bool m_bConnected;			///< 是否登录ftp服务器

    QList<DIRLEVEL> m_lstDirs;  ///< 等待遍历的目录
    //QStringList m_lstDirs;		///< 等待遍历的目录
    DIRLEVEL m_strCurDir;		///< 当前ftp目录

    CSubDirFilter m_oSubDirFilter;	// 子目录过滤器
    // string m_strCurDirLastTime;	///< 当前目录记录的最后修改时间
    //string m_strCurDirNewTime;	///< 当前目录现在的最后修改时间

    CollectorBase *m_pCoBase;

    int ftp_transfermode;
    int ftp_connectmode;

    //////////////////////////////////////////////////////////////////////////
public:
    CURL *m_pCurl;

    // 创建收集目录的libcurl对象，便于反复复用
    //CURL *createSourceCurl();

    CURL *m_pSourceCurl;

    // 创建收集目录的libcurl对象，便于反复复用
    //CURL *createDestCurl();

    CURL *m_pDestCurl;

    // 删除远程文件lincurl对象
    CURL *m_pRemoveDestCurl;
    /**
    * @brief  从ftp或file下载到文件
    * @param  const char * url
    * @param  const char * user_pwd
    * @param  FileData * fileData
    * @return int：无
    */
    int downloadFile(const char *url, const char *user_pwd, FileData *fileData);


    /**
    * @brief  从内存上传到ftp
    * @param  const char * url
    * @param  const char * user_pwd
    * @param  MemoryData * memData
    * @param  const string & filename
    * @return int：无
    */
    int uploadFile(const char *url, const char *user_pwd, MemoryData *memData, const string &filename, const char *sendsuffix);

    int uploadFileToDir(const char *url, const char *user_pwd, const string &filename, const char *localPath, const char *sendsuffix);
    int conputFileToDir(const char *url, const char *user_pwd, const string &filename, const char *localPath, const char *sendsuffix);
    // 0被动模式，1主动模式
    int uploadFileToFtp(const char *url, const char *user_pwd, const string &filename, const char *localPath, const char *sendsuffix, int transferMode);
    int conputFileToFtp(const char *url, const char *user_pwd, const string &filename, const char *localPath, const char *sendsuffix);

    /**
    * @brief  删除ftp服务器上一个文件
    * @param  const char * url：包含文件路径的url
    * @param  const char * user_pwd：用户名、密码
    * @param  const string & filename：文件名
    * @return int：返回0
    */
    int deleteFtpFile(const char *url, const char *user_pwd, const string &filename);

    int deleteLocalFile(const char *url, const char *user_pwd, const string &filename);

    bool getFileSize(const char *url, const char *user_pwd, const string &filename, double &fileSize);

    //////////////////////////////////////////////////////////////////////////

    void process4Ftp(CListInfo &oneInfo, const QStringList &lstParts);

    void process9Ftp(CListInfo &oneInfo, const QStringList &lstParts);
};


#endif