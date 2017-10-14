/**
  * @file   transferSet.h
  * @brief  数据传输设置序列化.
  *
  *  这里是详细说明。
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:05:16
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef transferSet_H_
#define transferSet_H_

#include <string>
#include <map>
#include <vector>
#include <list>
#include <sstream>
#include <fstream>
using namespace std;
#include "../macro.h"
#include "fileMsg.h"


// 分发设置
class SendSet
{
public:
    SendSet()
    {
        bEncrypt=false;
        bCompress=false;
        bKeepDir=true;
        bConSend = false;
        nSendWay = 0;
        nPort = 21;
    }

    string strUsrname;		//用户标识（要求唯一）
    string strPath;			//监控目录或URL
    string strRltvPath;		//相对路径
    vector<string> vecRegs;	//匹配规则
    bool bEncrypt;			//true表示加密，false表示不加密
    bool bCompress;			//true表示压缩，false表示不压缩
    bool bKeepDir;			//true表示保留原始目录结构，false表示不保留
    int nSendWay;			//分发方式（0-目录，1-FTP）
    bool bConSend;			//true表示断点续传

    string strUsr;			//用户名（共享目录方式可能也需要）
    string strPwd;			//密码
    int	   nPort;			//端口号
    string strIP;			//IP
    //--TCP--（待定）
};

class SendSetList
{
public:
    list<SendSet> lsts;
};

DLL_EXPORT_C_DECL bool SendSetListToXml(const string &strPath, const SendSetList &info);
DLL_EXPORT_C_DECL bool XmlToSendSetList(const string &strPath, SendSetList &info);

/**
  * @class <TransferSet>
  * @brief  数据传输设置序列化
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class TransferSet
{
public:
    string strName;				//目录标识（要求唯一）
    //收集
    string strPath;				//监控目录或URL
    int nCollectWay;			//收集方式（0-目录，1-FTP）
    vector<string> vecRegs;		//匹配规则
    int nWatchWay;				//目录监控方式（0-定时，1-实时）
    string strDispatch;			//调度规则
    //--FTP--
    string strUsr;				//用户名
    string strPwd;				//密码
    int	   nPort;				//端口号
    string strIP;				//IP
    vector<string> vecRecvID;	//用户ID
    bool	bEnable;			//是否启用
    //////////////////////////////////////////////////////////////////////////
    // 以下不被序列化
    vector<SendSet> vecRcvers;	//分发用户
    FileInfoList fileList;		//需要传输的文件列表
    int  sendFlag;				//传输标识
    //----

    TransferSet()
    {
        sendFlag = 0;
        bEnable = true;
    }
};

class TransferSetList
{
public:
    list<TransferSet> lsSets;
};

DLL_EXPORT_DECL bool TransferSetToXml(const string &strPath, const TransferSetList &info);
DLL_EXPORT_DECL bool XmlToTransferSet(const string &strPath, TransferSetList &info);


//////////////////////////////////////////////////////////////////////////
/**
  * @class <TransCollectTime>
  * @brief  数据传输收集时间
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class TransCollectTime
{
public:
    std::string strDirID;		//目录ID（唯一）
    std::map<std::string, uint> mapDirTime;	//路径-最后修改时间
    //std::map<std::string, std::string> mapDirTime;

    //重载==，便于容器直接remove
    bool operator==(const TransCollectTime& time)
    {
        if ( this->strDirID == time.strDirID )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

class TransCollectTimeList
{
public:
    TransCollectTimeList()
    {
        lsts.clear();
    }
    std::list<TransCollectTime> lsts;
};

DLL_EXPORT_C_DECL bool TransCollectTimeToXml(const std::string &strPath, const TransCollectTimeList &lst);
DLL_EXPORT_C_DECL bool XmlToTransCollectTime(const std::string &strPath, TransCollectTimeList &lst);

#endif