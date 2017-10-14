#include "curlftp.h"
#include "ctkLog.h"
#include <QThread>

#pragma warning(disable:4996)

SignalObject g_SignalObject;	///< 全局变量

/// MLSD信息结构体
struct CListInfo
{
    QString	strFileName;
    QString	strMdfyTime;
    int		nType;			// 1-文件，2-目录
    long	nFileSize;

    CListInfo()
    {
        nType = 0;
        nFileSize = 0;
    }
};

/// 进度条使用
struct myprogress
{
    double lastruntime;
    CURL *curl;
};

const uint MINIMAL_PROGRESS_INTERVAL = 1;		// 1s获取一次进度
const string TmpNameSuffix = ".~tmp";

// 获取文件到内存（大文件会卡主线程）
static size_t WriteInMemoryFun(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;		//本次回调获取的数据
    struct MemoryData *mem = (struct MemoryData *)userp;

    mem->memdata = (char *)realloc(mem->memdata, mem->size + realsize + 1);
    if (mem->memdata == NULL)
    {
        /* out of memory! */
        QSLOG_ERROR("not enough memory (realloc returned NULL).");
        return 0;
    }

    memcpy(&(mem->memdata[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memdata[mem->size] = 0;

    return realsize;
}

// 获取文件到内存（频繁申请空间，可能导致异常）
static size_t WriteInMemoryFun_New(char *contents, size_t size, size_t nmemb, string *memData)
{
    unsigned long realsize = size * nmemb;
    if (memData == NULL)
    {
        QSLOG_ERROR("memData is null.");
        return 0;
    }
    memData->append(contents, realsize);

    return realsize;
}

// 提前申请好大内存
static size_t WriteInMemoryFun_Big(void *contents, size_t size, size_t nmemb, void *dest)
{
    size_t realsize = size * nmemb;		//本次回调获取的数据
    struct MemoryData *mem = (struct MemoryData *)dest;

    if (mem->memdata == NULL)
    {
        QSLOG_ERROR("mem->memdata is null.");
        return 0;
    }

    // 避免出现获取文件大小失败的情况
    if ((mem->size + realsize) > mem->capacity)
    {
        unsigned int capacity = mem->size + realsize * 6 + 1;	// 减少realloc次数，一定要+1 !!
        mem->memdata = (char *)realloc(mem->memdata, capacity);
        if (mem->memdata == NULL)
        {
            QSLOG_DEBUG(QString("Fail to realloc: %1 %2").arg(capacity).arg(mem->size));
            capacity = mem->size + realsize + 1;
            mem->memdata = (char *)realloc(mem->memdata, capacity);
            if (mem->memdata == NULL)
            {
                QSLOG_DEBUG(QString("Fail to realloc: %1 %2").arg(capacity).arg(mem->size));
                return 0;
            }
        }
        mem->capacity = capacity;
        QSLOG_DEBUG(QString("Sucecss to realloc: %1 %2").arg(capacity).arg(mem->size));
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
        //QSLOG_ERROR("Fail to UploadFromMemFun");
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

//////////////////////////////////////////////////////////////////////////
// 下载到文件
static size_t WriteInFileFun(void *buffer, size_t size, size_t nmemb, void *stream)
{
    struct FileData *out = (struct FileData *)stream;
    if (out && !out->stream)	// 若没有打开文件
    {
        out->stream = fopen(out->filename, "wb");
        if (!out->stream)		// 打开文件失败
        {
            return -1;
        }
    }

    return fwrite(buffer, size, nmemb, out->stream);
}

// 从文件上传
static size_t ReadFromFile(void *buffer, size_t size, size_t nmemb, void *stream)
{
    size_t retcode = fread(buffer, size, nmemb, (FILE *)stream);
    return retcode;
}

//////////////////////////////////////////////////////////////////////////

static size_t throw_away(void *ptr, size_t size, size_t nmemb, void *data)
{
    (void)ptr;
    (void)data;

    return (size_t)(size * nmemb);
}

// 进度条回调函数
static int progressFun(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    struct myprogress *myp = (struct myprogress *)p;
    CURL *curl = myp->curl;
    if (curl == NULL)
    {
        return 0;
    }
    double curtime = 0;
    // 从句柄获得时间信息，单位s（同步??）
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &curtime);
    double delTime = curtime - myp->lastruntime;
    if ((delTime >= MINIMAL_PROGRESS_INTERVAL) ||
            (ulnow > 0 && ulnow ==  ultotal))
    {
        myp->lastruntime = curtime;
        g_SignalObject.emitProgress(curl, (qint64)dltotal, (qint64)dlnow, (qint64)ultotal, (qint64)ulnow, delTime);
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////

//CurlFtp *CurlFtp::m_sCurlFtp = NULL;

CurlFtp::CurlFtp()
    : m_bConnected(false)
{
    //m_sCurlFtp = this;
    m_nPgtime = MINIMAL_PROGRESS_INTERVAL;
    m_pCurl = curl_easy_init();
    if (m_pCurl == NULL)
    {
        QSLOG_ERROR("curl_easy_init error.");
        exit(-1);
    }
}

CurlFtp::~CurlFtp()
{
    curl_easy_cleanup(m_pCurl);
    m_pCurl = NULL;
}

void CurlFtp::setHostPort(const QString &strIP, const int &nPort)
{
    m_strIP = strIP.toStdString();
    m_nPort = nPort;
}

void CurlFtp::setUserPwd(const QString &strUser, const QString &strPwd)
{
    m_strUser = strUser.toStdString();
    m_strPwd = strPwd.toStdString();
}

void CurlFtp::setRootPath(const QString &strPath)
{
    m_strRoot = strPath.toStdString();
}

//int CurlFtp::downloadFile(const char *url, const char *user_pwd, MemoryData *memData)
//{
//    CURL *curl = m_pCurl;
//    if (curl == NULL)
//    {
//        QSLOG_ERROR("m_pCurl is null.");
//        return -1;
//    }
//
//    string buffer;
//    //buffer.resize()
//
//    curl_easy_reset(m_pCurl);	// 重置
//    curl_easy_setopt(curl, CURLOPT_URL, url);
//    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
//    //curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)memData);
//    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
//    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteInMemoryFun_New);
//    //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteInMemoryFun);
//    //--进度条功能--
//    struct myprogress prog;
//    prog.lastruntime = 0;
//    prog.curl = curl;
//    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressFun);
//    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
//    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
//    //------------
//    CURLcode res = curl_easy_perform(curl);
//    if (CURLE_OK != res)
//    {
//        QSLOG_ERROR(QString("downloadFile error: %1").arg(curl_easy_strerror(res)));
//        return -1;
//    }
//
//    int len = buffer.size();
//    memData->memdata = (char *)malloc(len + 1);
//    if (memData->memdata == NULL)
//    {
//        QSLOG_ERROR(QStringLiteral("申请内存失败。"));
//        return -2;
//    }
//
//    buffer.copy(memData->memdata, len, 0);
//    memData->size = len;
//
//    return 0;
//}

int CurlFtp::downloadFile(const char *url, const char *user_pwd, MemoryData *memData)
{
    //////////////////////////////////////////////////////////////////////////
    // [10/20/2016 wujun]
    double dLen = 0;
    getFileInfo(url, user_pwd, "", dLen);
    if (dLen <= 0)
    {
        QSLOG_DEBUG(QStringLiteral("getFileInfo: %1").arg(url));
        dLen = 0;
    }
    size_t nLen = (size_t)dLen + 1;
    memData->memdata = (char *)malloc(nLen);
    if (memData->memdata == NULL)
    {
        QSLOG_ERROR(QStringLiteral("申请内存失败，大小%1").arg(nLen+1));
        return -2;
    }
    memData->size = 0;
    memData->capacity = nLen;
    //////////////////////////////////////////////////////////////////////////

    CURL *curl = m_pCurl;
    if (curl == NULL)
    {
        QSLOG_ERROR("m_pCurl is null.");
        return -1;
    }

    curl_easy_reset(m_pCurl);	// 重置
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)memData);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteInMemoryFun_Big);
    //--进度条功能--
    struct myprogress prog;
    prog.lastruntime = 0;
    prog.curl = curl;
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressFun);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
    //------------
    CURLcode res = curl_easy_perform(curl);
    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("downloadFile error: %1").arg(curl_easy_strerror(res)));
        return -1;
    }
    QSLOG_DEBUG(QStringLiteral("下载成功: %1").arg(url));

    return 0;
}

int CurlFtp::downloadFile(const char *url, const char *user_pwd, FileData *fileData)
{
    CURL *curl = m_pCurl;
    if (curl == NULL)
    {
        QSLOG_ERROR("m_pCurl is null");
        return -1;
    }

    // 重置
    curl_easy_reset(m_pCurl);
    // 指定全路径url
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
    // 设置下载文件的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteInFileFun);
    // 设置传递给回调函数的对象指针
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fileData);
    //--进度条功能--
    //struct myprogress prog;
    //prog.lastruntime = 0;
    //prog.curl = curl;
    //curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressFun);
    //curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
    //curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
    //------------
    CURLcode res = curl_easy_perform(curl);
    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("downloadFile error: %1").arg(curl_easy_strerror(res)));
        return -1;
    }

    // 下载完成，关闭本地文件
    if (fileData->stream)
    {
        fclose(fileData->stream);
    }

    return 0;
}

int CurlFtp::uploadFile(const char *url, const char *user_pwd, MemoryData *memData, const string &filename)
{
    if (!connectToHost(url, user_pwd))
    {
        return -1;
    }

    CURL *curl = m_pCurl;
    if (curl == NULL)
    {
        return -1;
    }
    curl_easy_reset(m_pCurl);	// 重置

    struct curl_slist *headerlist = NULL;
    CURLcode res = CURLE_OK;
    // 删除已有文件
    deleteFile(url, user_pwd, filename);

    char * const pStart = memData->memdata;	//记录起始位置
    size_t orgSize = memData->size;			//记录原始大小
    string cmdTmpName = "RNFR " + filename + TmpNameSuffix;
    string cmdOrgName = "RNTO " + filename;
    string strUrl = url;
    strUrl += filename + TmpNameSuffix;

    headerlist = curl_slist_append(headerlist, cmdTmpName.c_str());
    headerlist = curl_slist_append(headerlist, cmdOrgName.c_str());
    curl_easy_setopt(curl, CURLOPT_UPLOAD, TRUE);
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
    curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, TRUE);
    curl_easy_setopt(curl, CURLOPT_READDATA, memData);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, UploadFromMemFun);
    /* pass in that last of FTP commands to run after the transfer */
    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE, (curl_off_t)memData->size);
    //--进度条功能--
    struct myprogress prog;
    prog.lastruntime = 0;
    prog.curl = curl;
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressFun);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
    //------------
    res = curl_easy_perform(curl);
    curl_slist_free_all(headerlist);
    //curl_easy_cleanup(curl);
    memData->memdata = pStart;		//还原起始位置
    memData->size = orgSize;
    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("uploadFile error: %1").arg(curl_easy_strerror(res)));
        return -1;
    }

    return 0;
}

int CurlFtp::conputFile(const char *url, const char *user_pwd, MemoryData *memData, const string &filename)
{
    if (!connectToHost(url, user_pwd))
    {
        return -1;
    }

    double dHasSend = 0.0;
    string strTmpName = filename + TmpNameSuffix;
    if (getFileInfo(url, user_pwd, strTmpName, dHasSend))
    {
        CURL *curl = m_pCurl;
        if (curl == NULL)
        {
            QSLOG_ERROR("m_pCurl is null.");
            return -1;
        }
        curl_easy_reset(m_pCurl);	// 重置

        // 删除已有文件
        deleteFile(url, user_pwd, filename);

        char * const pStart = memData->memdata;	//记录起始位置
        size_t orgSize = memData->size;			//记录原始大小
        string cmdTmpName = "RNFR " + filename + TmpNameSuffix;
        string cmdOrgName = "RNTO " + filename;
        string strUrl = url;
        strUrl += filename + TmpNameSuffix;
        struct curl_slist *headerlist = NULL;
        headerlist = curl_slist_append(headerlist, cmdTmpName.c_str());
        headerlist = curl_slist_append(headerlist, cmdOrgName.c_str());
        curl_easy_setopt(curl, CURLOPT_UPLOAD, TRUE);
        //////////////////////////////////////////////////////////////////////////
        size_t tHasSend = (size_t)dHasSend;
        if (tHasSend > memData->size)
        {
            tHasSend = memData->size;
        }
        curl_easy_setopt(curl, CURLOPT_APPEND, TRUE);
        //curl_easy_setopt(curl, CURLOPT_RESUME_FROM, tHasSend);
        memData->memdata += tHasSend;
        memData->size -= tHasSend;
        //////////////////////////////////////////////////////////////////////////
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
        curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, TRUE);
        curl_easy_setopt(curl, CURLOPT_READDATA, memData);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, UploadFromMemFun);
        /* pass in that last of FTP commands to run after the transfer */
        curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
        curl_easy_setopt(curl, CURLOPT_INFILESIZE, (curl_off_t)memData->size);
        //--进度条功能--
        struct myprogress prog;
        prog.lastruntime = 0;
        prog.curl = curl;
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressFun);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
        //------------
        CURLcode res = curl_easy_perform(curl);
        curl_slist_free_all(headerlist);
        //curl_easy_cleanup(curl);
        // 还原起始位置
        memData->memdata = pStart;
        memData->size = orgSize;
        if (CURLE_OK != res)
        {
            QSLOG_ERROR(QString("uploadFile error: %1").arg(curl_easy_strerror(res)));
            return -1;
        }
    }
    else
    {
        return uploadFile(url, user_pwd, memData, filename);
    }

    return 0;
}

int CurlFtp::deleteFile(const char *url, const char *user_pwd, const string &filename)
{
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        QSLOG_ERROR("curl_easy_init error.");
        return -1;
    }

    string cmdDelt = "DELE " + filename;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, cmdDelt.c_str());
    curl_easy_perform(curl);	//不用管结果

    curl_easy_cleanup(curl);
    return 0;
}

// 有时获取大小失败！
bool CurlFtp::getFileInfo(const char *url, const char *user_pwd, const string &filename, double &fileSize)
{
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        QSLOG_ERROR("curl_easy_init error.");
        return false;
    }
    fileSize = 0.0;
    string strUrl = url;
    strUrl += filename;

    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
    curl_easy_setopt(curl, CURLOPT_NOBODY, TRUE);
    //curl_easy_setopt(curl, CURLOPT_HEADER, FALSE);
    //---File模式必须这样写---
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, throw_away);
    curl_easy_setopt(curl, CURLOPT_HEADER, TRUE);
    //------
    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK)
    {
        res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &fileSize);
        if((CURLE_OK == res) && (fileSize > 0.0))
        {
            curl_easy_cleanup(curl);
            return true;
        }
    }

    curl_easy_cleanup(curl);
    return false;
}

// void CurlFtp::emitProgress(qint64 dltotal, qint64 dlnow, qint64 ultotal, qint64 ulnow)
// {
//     emit progress(dltotal, dlnow, ultotal, ulnow);
// }

/// 未使用的函数
int CurlFtp::renameFile(const char *url, const char *user_pwd, const string &oldname, const string &newname)
{
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        QSLOG_ERROR("curl_easy_init error.");
        return -1;
    }

    string cmdTmpName = "RNFR " + oldname;
    string cmdOrgName = "RNTO " + newname;
    string strUrl = url;
    struct curl_slist *headerlist = NULL;
    headerlist = curl_slist_append(headerlist, cmdTmpName.c_str());
    headerlist = curl_slist_append(headerlist, cmdOrgName.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);

    CURLcode res = curl_easy_perform(curl);	//不用管结果

    curl_easy_cleanup(curl);
    return 0;
}

bool CurlFtp::connectToHost(const char *url, const char *user_pwd)
{
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        QSLOG_ERROR("curl_easy_init error.");
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, TRUE);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, TRUE);
    curl_easy_setopt(curl, CURLOPT_NOBODY, TRUE);
    curl_easy_setopt(curl, CURLOPT_HEADER, FALSE);

    CURLcode res = curl_easy_perform(curl);
    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("connectToHost error: %1").arg(curl_easy_strerror(res)));
        return false;
    }

    return true;
}

int CurlFtp::uploadFileToDir(const char *url, const char *user_pwd, MemoryData *memData, const string &filename)
{
    if (m_pCurl == NULL)
    {
        QSLOG_ERROR("m_pCurl is null.");
        return -1;
    }
    curl_easy_reset(m_pCurl);	// 重置

    CURLcode res = CURLE_OK;
    // 删除已有文件
    deleteFile(url, user_pwd, filename);

    char * const pStart = memData->memdata;	//记录起始位置
    size_t orgSize = memData->size;			//记录原始大小
    // file协议下重命名无效
//     string cmdTmpName = "RNFR " + filename + TmpNameSuffix;
//     string cmdOrgName = "RNTO " + filename;
    string strUrl = url;
    strUrl += filename + TmpNameSuffix;

//     headerlist = curl_slist_append(headerlist, cmdTmpName.c_str());
//     headerlist = curl_slist_append(headerlist, cmdOrgName.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_UPLOAD, TRUE);
    curl_easy_setopt(m_pCurl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, user_pwd);
    //curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, TRUE);
    curl_easy_setopt(m_pCurl, CURLOPT_READDATA, memData);
    curl_easy_setopt(m_pCurl, CURLOPT_READFUNCTION, UploadFromMemFun);
    /* pass in that last of FTP commands to run after the transfer */
    //curl_easy_setopt(m_pCurl, CURLOPT_POSTQUOTE, headerlist);
    curl_easy_setopt(m_pCurl, CURLOPT_INFILESIZE, (curl_off_t)memData->size);
    //--进度条功能--
    struct myprogress prog;
    prog.lastruntime = 0;
    prog.curl = m_pCurl;
    curl_easy_setopt(m_pCurl, CURLOPT_XFERINFOFUNCTION, progressFun);
    curl_easy_setopt(m_pCurl, CURLOPT_XFERINFODATA, &prog);
    curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, FALSE);
    //------------
    res = curl_easy_perform(m_pCurl);
    //curl_slist_free_all(headerlist);
    memData->memdata = pStart;		//还原起始位置
    memData->size = orgSize;
    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("uploadFile error: %1").arg(curl_easy_strerror(res)));
        return -1;
    }

    return 0;
}

int CurlFtp::uploadFileToDir(const char *url, const char *user_pwd, const string &filename, const char *localPath)
{
    if (m_pCurl == NULL)
    {
        QSLOG_ERROR("m_pCurl is null.");
        return -1;
    }
    curl_easy_reset(m_pCurl);
    // 删除已有文件
    deleteFile(url, user_pwd, filename);

    FILE *pfile = fopen(localPath, "rb");
    if (pfile == NULL)
    {
        QSLOG_ERROR(QStringLiteral("打开文件失败：%1").arg(localPath));
        return -1;
    }
    // 获取文件大小
    fseek(pfile, 0, SEEK_END);
    long fsize = ftell(pfile);
    fseek(pfile, 0, SEEK_SET);

    CURLcode res = CURLE_OK;
    string strUrl = url;
    strUrl += filename + TmpNameSuffix;
    curl_easy_setopt(m_pCurl, CURLOPT_UPLOAD, TRUE);
    curl_easy_setopt(m_pCurl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, user_pwd);
    curl_easy_setopt(m_pCurl, CURLOPT_READDATA, pfile);
    curl_easy_setopt(m_pCurl, CURLOPT_READFUNCTION, ReadFromFile);
    curl_easy_setopt(m_pCurl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)fsize);
    //--进度条功能--
    struct myprogress prog;
    prog.lastruntime = 0;
    prog.curl = m_pCurl;
    curl_easy_setopt(m_pCurl, CURLOPT_XFERINFOFUNCTION, progressFun);
    curl_easy_setopt(m_pCurl, CURLOPT_XFERINFODATA, &prog);
    curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, FALSE);
    //------------
    res = curl_easy_perform(m_pCurl);

    fclose(pfile);

    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("uploadFile error: %1").arg(curl_easy_strerror(res)));
        return -1;
    }

    return 0;
}

int CurlFtp::uploadFileToFtp(const char *url, const char *user_pwd, const string &filename, const char *localPath)
{
    // 连接ftp服务器
    if (!connectToHost(url, user_pwd))
    {
        return -1;
    }

    // 删除已有文件
    deleteFile(url, user_pwd, filename);

    // 打开本地文件，并获取文件大小
    FILE *pfile = fopen(localPath, "rb");
    if (pfile == NULL)
    {
        QSLOG_ERROR(QStringLiteral("打开文件失败：%1").arg(localPath));
        return -1;
    }
    fseek(pfile, 0, SEEK_END);
    long fsize = ftell(pfile);
    fseek(pfile, 0, SEEK_SET);

    CURL *curl = m_pCurl;
    if (curl == NULL)
    {
        return -1;
    }
    curl_easy_reset(m_pCurl);

    struct curl_slist *headerlist = NULL;
    CURLcode res = CURLE_OK;

    string cmdTmpName = "RNFR " + filename + TmpNameSuffix;
    string cmdOrgName = "RNTO " + filename;
    string strUrl = url;
    strUrl += filename + TmpNameSuffix;

    headerlist = curl_slist_append(headerlist, cmdTmpName.c_str());
    headerlist = curl_slist_append(headerlist, cmdOrgName.c_str());
    curl_easy_setopt(curl, CURLOPT_UPLOAD, TRUE);
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
    curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, TRUE);
    curl_easy_setopt(curl, CURLOPT_READDATA, pfile);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadFromFile);
    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)fsize);
    //--进度条功能--
    struct myprogress prog;
    prog.lastruntime = 0;
    prog.curl = curl;
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressFun);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
    //------------
    res = curl_easy_perform(curl);
    curl_slist_free_all(headerlist);
    fclose(pfile);

    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("uploadFile error: %1").arg(curl_easy_strerror(res)));
        return -1;
    }

    return 0;
}

int CurlFtp::conputFileToDir(const char *url, const char *user_pwd, MemoryData *memData, const string &filename)
{
    double dHasSend = 0.0;
    string strTmpName = filename + TmpNameSuffix;
    if (getFileInfo(url, user_pwd, strTmpName, dHasSend))
    {
        if (m_pCurl == NULL)
        {
            QSLOG_ERROR("m_pCurl is null.");
            return -1;
        }
        curl_easy_reset(m_pCurl);	// 重置

        char * const pStart = memData->memdata;	//记录起始位置
        size_t orgSize = memData->size;			//记录原始大小
        string strUrl = url;
        strUrl += filename + TmpNameSuffix;
        curl_easy_setopt(m_pCurl, CURLOPT_UPLOAD, TRUE);
        //////////////////////////////////////////////////////////////////////////
        // 和FTP断点续传有区别，原因不明
        size_t tHasSend = (size_t)dHasSend;
        if (tHasSend > memData->size)
        {
            tHasSend = memData->size;
        }
        curl_easy_setopt(m_pCurl, CURLOPT_APPEND, TRUE);
        curl_easy_setopt(m_pCurl, CURLOPT_RESUME_FROM, tHasSend);
        //////////////////////////////////////////////////////////////////////////
        curl_easy_setopt(m_pCurl, CURLOPT_URL, strUrl.c_str());
        curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, user_pwd);
        curl_easy_setopt(m_pCurl, CURLOPT_READDATA, memData);
        curl_easy_setopt(m_pCurl, CURLOPT_READFUNCTION, UploadFromMemFun);
        curl_easy_setopt(m_pCurl, CURLOPT_INFILESIZE, (curl_off_t)memData->size);
        //--进度条功能--
        struct myprogress prog;
        prog.lastruntime = 0;
        prog.curl = m_pCurl;
        curl_easy_setopt(m_pCurl, CURLOPT_XFERINFOFUNCTION, progressFun);
        curl_easy_setopt(m_pCurl, CURLOPT_XFERINFODATA, &prog);
        curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, FALSE);
        //------------
        CURLcode res = curl_easy_perform(m_pCurl);
        // 还原原始状态
        memData->memdata = pStart;
        memData->size = orgSize;
        if (CURLE_OK != res)
        {
            QSLOG_ERROR(QString("uploadFile error: %1").arg(curl_easy_strerror(res)));
            return -1;
        }
    }
    else
    {
        return uploadFileToDir(url, user_pwd, memData, filename);
    }

    return 0;
}

int CurlFtp::conputFileToDir(const char *url, const char *user_pwd, const string &filename, const char *localPath)
{
    double dHasSend = 0.0;
    string strTmpName = filename + TmpNameSuffix;
    if (getFileSize(url, user_pwd, strTmpName, dHasSend))
    {
        if (m_pCurl == NULL)
        {
            QSLOG_ERROR("m_pCurl is null.");
            return -1;
        }
        curl_easy_reset(m_pCurl);

        string strUrl = url;
        strUrl += filename + TmpNameSuffix;
        curl_easy_setopt(m_pCurl, CURLOPT_UPLOAD, TRUE);
        //////////////////////////////////////////////////////////////////////////
        // 断点续传（另一个方式）
        FILE *pfile = fopen(localPath, "rb");
        if (pfile == NULL)
        {
            QSLOG_ERROR(QStringLiteral("打开文件失败：%1").arg(localPath));
            return -1;
        }
        fseek(pfile, 0, SEEK_END);
        long fsize = ftell(pfile);
        fseek(pfile, 0, SEEK_SET);

        size_t tHasSend = (size_t)dHasSend;
        if (tHasSend > fsize)
        {
            tHasSend = fsize;
        }

        //fseek(pfile, tHasSend, SEEK_SET);
        curl_easy_setopt(m_pCurl, CURLOPT_APPEND, TRUE);
        curl_easy_setopt(m_pCurl, CURLOPT_RESUME_FROM, tHasSend);
        //////////////////////////////////////////////////////////////////////////
        curl_easy_setopt(m_pCurl, CURLOPT_URL, strUrl.c_str());
        curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, user_pwd);
        curl_easy_setopt(m_pCurl, CURLOPT_READDATA, pfile);
        curl_easy_setopt(m_pCurl, CURLOPT_READFUNCTION, ReadFromFile);
        curl_easy_setopt(m_pCurl, CURLOPT_INFILESIZE, (curl_off_t)fsize);
        //--进度条功能--
        struct myprogress prog;
        prog.lastruntime = 0;
        prog.curl = m_pCurl;
        curl_easy_setopt(m_pCurl, CURLOPT_XFERINFOFUNCTION, progressFun);
        curl_easy_setopt(m_pCurl, CURLOPT_XFERINFODATA, &prog);
        curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, FALSE);
        //------------
        CURLcode res = curl_easy_perform(m_pCurl);

        if (CURLE_OK != res)
        {
            QSLOG_ERROR(QString("uploadFile error: %1").arg(curl_easy_strerror(res)));
            return -1;
        }
    }
    else
    {
        return uploadFileToDir(url, user_pwd, filename, localPath);
    }

    return 0;
}

int CurlFtp::conputFileToFtp(const char *url, const char *user_pwd, const string &filename, const char *localPath)
{
    if (!connectToHost(url, user_pwd))
    {
        return -1;
    }

    double dHasSend = 0.0;
    string strTmpName = filename + TmpNameSuffix;
    if (getFileSize(url, user_pwd, strTmpName, dHasSend))
    {
        CURL *curl = m_pCurl;
        if (curl == NULL)
        {
            QSLOG_ERROR("m_pCurl is null.");
            return -1;
        }
        curl_easy_reset(m_pCurl);

        // 删除已有文件
        deleteFile(url, user_pwd, filename);

        string cmdTmpName = "RNFR " + filename + TmpNameSuffix;
        string cmdOrgName = "RNTO " + filename;
        string strUrl = url;
        strUrl += filename + TmpNameSuffix;
        struct curl_slist *headerlist = NULL;
        headerlist = curl_slist_append(headerlist, cmdTmpName.c_str());
        headerlist = curl_slist_append(headerlist, cmdOrgName.c_str());
        curl_easy_setopt(curl, CURLOPT_UPLOAD, TRUE);
        //////////////////////////////////////////////////////////////////////////
        // 断点续传
        FILE *pfile = fopen(localPath, "rb");
        if (pfile == NULL)
        {
            QSLOG_ERROR(QStringLiteral("打开文件失败：%1").arg(localPath));
            return -1;
        }
        fseek(pfile, 0, SEEK_END);
        long fsize = ftell(pfile);

        size_t tHasSend = (size_t)dHasSend;
        if (tHasSend > fsize)
        {
            tHasSend = fsize;
        }

        fseek(pfile, tHasSend, SEEK_SET);

        curl_easy_setopt(curl, CURLOPT_APPEND, TRUE);
        //curl_easy_setopt(curl, CURLOPT_RESUME_FROM, tHasSend);
        //////////////////////////////////////////////////////////////////////////
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
        curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, TRUE);
        curl_easy_setopt(curl, CURLOPT_READDATA, pfile);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadFromFile);
        curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
        //curl_easy_setopt(curl, CURLOPT_INFILESIZE, (curl_off_t)memData->size);
        //--进度条功能--
        struct myprogress prog;
        prog.lastruntime = 0;
        prog.curl = curl;
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressFun);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
        //------------
        CURLcode res = curl_easy_perform(curl);
        curl_slist_free_all(headerlist);

        if (CURLE_OK != res)
        {
            QSLOG_ERROR(QString("uploadFile error: %1").arg(curl_easy_strerror(res)));
            return -1;
        }
    }
    else
    {
        return uploadFileToFtp(url, user_pwd, filename, localPath);
    }

    return 0;
}

bool CurlFtp::getFileSize(const char *url, const char *user_pwd, const string &filename, double &fileSize)
{
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        QSLOG_ERROR("curl_easy_init error.");
        return false;
    }
    fileSize = 0.0;
    string strUrl = url;
    strUrl += filename;

    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_NOBODY, TRUE);
    curl_easy_setopt(curl, CURLOPT_HEADER, FALSE);
    //---File模式必须这样写---
    //curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, throw_away);
    //curl_easy_setopt(curl, CURLOPT_HEADER, TRUE);
    //------
    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK)
    {
        res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &fileSize);
        if((CURLE_OK == res) && (fileSize > 0.0))
        {
            curl_easy_cleanup(curl);
            return true;
        }
    }

    curl_easy_cleanup(curl);
    return false;
}

void SignalObject::emitProgress(CURL *curl, qint64 dltotal, qint64 dlnow, qint64 ultotal, qint64 ulnow, double dTime)
{
    emit progress((int)curl, dltotal, dlnow, ultotal, ulnow, dTime);
}
