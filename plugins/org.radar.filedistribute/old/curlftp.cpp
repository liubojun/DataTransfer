#include "curlftp.h"
#include "ctkLog.h"

#pragma warning(disable:4996)

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

static size_t WriteInMemoryFun(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;		//本次回调获取的数据
    struct MemoryData *mem = (struct MemoryData *)userp;

    mem->memdata = (char *)realloc(mem->memdata, mem->size + realsize + 1);
    if (mem->memdata == NULL)
    {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
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

// 进度条回调函数
static int progressFun(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    struct myprogress *myp = (struct myprogress *)p;
    CURL *curl = myp->curl;
    double curtime = 0;
    // 从句柄获得时间信息，单位s（同步??）
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &curtime);
    if (curtime - myp->lastruntime >= MINIMAL_PROGRESS_INTERVAL)
    {
        myp->lastruntime = curtime;
        if (CurlFtp::m_sCurlFtp != NULL)
        {
            CurlFtp::m_sCurlFtp->emitProgress((qint64)dltotal, (qint64)dlnow, (qint64)ultotal, (qint64)ulnow);
        }
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////

CurlFtp *CurlFtp::m_sCurlFtp = NULL;

CurlFtp::CurlFtp()
    : m_bConnected(false)
{
    m_sCurlFtp = this;
    m_nPgtime = MINIMAL_PROGRESS_INTERVAL;
}

CurlFtp::~CurlFtp()
{

}

int CurlFtp::setCommOpt()
{
    return 0;
}

void CurlFtp::getNewFiles(FileInfoList &fileList)
{
    m_bConnected = connectToHost();
    if (!m_bConnected)
    {
        emit done();
        return;
    }

    m_lstDirs.clear();
    m_lstDirs.append(QString::fromStdString(m_strRoot));
    while (!m_lstDirs.isEmpty())
    {
        m_strCurDir = m_lstDirs.takeFirst();
        listFiles(m_strCurDir.toStdString(), fileList);
    }

    emit done();
}

bool CurlFtp::connectToHost()
{
    return true;
}

void CurlFtp::listFiles(const string &strDir, FileInfoList &fileList)
{

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

int CurlFtp::downloadFile(const char *url, const char *user_pwd, MemoryData *memData)
{
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        QSLOG_ERROR("curl_easy_init error.");
        return -1;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)memData);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteInMemoryFun);
    //--进度条功能--
    struct myprogress prog;
    prog.lastruntime = 0;
    prog.curl = curl;
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressFun);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
    //------------
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("downloadFile error: %1").arg(curl_easy_strerror(res)));
        return -1;
    }

    return 0;
}

int CurlFtp::uploadFile(const char *url, const char *user_pwd, MemoryData *memData, const string &filename)
{
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        QSLOG_ERROR("curl_easy_init error.");
        return -1;
    }

    struct curl_slist *headerlist = NULL;
    CURLcode res = CURLE_OK;
    // 删除已有文件
    deleteFile(url, user_pwd, filename);

    char * const pStart = memData->memdata;	//记录起始位置
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
    curl_easy_cleanup(curl);
    memData->memdata = pStart;		//还原起始位置
    if (CURLE_OK != res)
    {
        QSLOG_ERROR(QString("uploadFile error: %1").arg(curl_easy_strerror(res)));
        return -1;
    }

    return 0;
}

int CurlFtp::conputFile(const char *url, const char *user_pwd, MemoryData *memData, const string &filename)
{
    double dHasSend = 0.0;
    string strTmpName = filename + TmpNameSuffix;
    if (getFileInfo(url, user_pwd, strTmpName, dHasSend))
    {
        CURL *curl = curl_easy_init();
        if (curl == NULL)
        {
            QSLOG_ERROR("curl_easy_init error.");
            return -1;
        }

        // 删除已有文件
        deleteFile(url, user_pwd, filename);

        char * const pStart = memData->memdata;	//记录起始位置
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
        curl_easy_cleanup(curl);
        memData->memdata = pStart;		//还原起始位置
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
    curl_easy_setopt(curl, CURLOPT_HEADER, FALSE);
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

void CurlFtp::emitProgress(qint64 dltotal, qint64 dlnow, qint64 ultotal, qint64 ulnow)
{
    emit progress(dltotal, dlnow, ultotal, ulnow);
}

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
    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
    CURLcode res = curl_easy_perform(curl);	//不用管结果

    curl_easy_cleanup(curl);
    return 0;
}
