/**
  * @file   msg.h
  * @brief  消息中心消息结构序列化定义.
  *		消息ID类型
  *  	case 10000:bRet = push(m);break;
  *		case 11024:bRet = pop(m);break;
  * @author NRIET.
  * @par   Copyright (c):
  *          NRIET..
  *         All Rights Reserved
  * @date   2016:04:27
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef _H_H_MSG_H_H_
#define _H_H_MSG_H_H_

#include "macro.h"
#include "czmq.h"
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <ctime>
#include <QMap>
#include <QList>
#include <QSharedPointer>
#include <QDateTime>
#include <QMutex>
using namespace std;

#pragma warning(disable:4251)
#pragma warning(disable:4018)

enum MSGIDTYPE
{
    POSTMESSAGE = 10000,
    // 出队消息：11024~12047
    GETMESSAGE=11024,			///< 取消息
    QUERYBYSTATIONNO = 11025,
    QUERYBYQUEUENM = 11026,
    QUERYQUEUEINFO = 11027,
    CLEARMESSAGE = 12047,		///< 清空消息
    HEARTBEAT = 12068,
    QUERYHEARTBEAT = 12069
};

/**
  * @class <msgheader>
  * @brief  消息头结构类定义.
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL msgheader
{
public:
    msgheader();
    msgheader(unsigned int i_queueid, std::string i_queuename, unsigned int i_msgid, unsigned long i_msgtime);
    msgheader(const msgheader& header);
    msgheader& operator=(const msgheader& header);
    ~msgheader();
public:
    unsigned int  m_queueid;		///< 队列id
    std::string	  m_queuename;		///< 队列名称
    unsigned int  m_msgid;			///< 消息id
    unsigned long long m_msgtime;	///< 消息产生的时间（0-表示无效消息）
    bool		  m_bRemove;		///< 只针对获取消息，是否从消息队列中移除消息.
};

/**
  * @class <msgbody>
  * @brief  消息体结构类定义.
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL msgbody
{
public:
    msgbody();
    msgbody(const char *i_msgdata);
    msgbody(std::string i_msgdata);
    msgbody(const msgbody& body);
    msgbody& operator=(const msgbody& body);
    ~msgbody();
public:
    std::string	  m_msgdata;		///< 消息体数据
};

/**
  * @class <msg>
  * @brief  消息中心消息结构序列化类定义.
  *		用于进程间通信，收发消息
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL msg
{
public:
    msg(void);
    msg(unsigned int i_queueid, std::string i_queuename, unsigned int i_msgid, unsigned long i_msgtime, std::string i_msgdata);
    msg(const msg& m);
    msg& operator=(const msg& m);
    ~msg(void);
public:
    msgheader head;
    msgbody	  body;
};

/**
  * @class <msgs>
  * @brief  消息集合声明类.
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL msgs
{
public:
    msgs(void);
    msgs(const msgs& ms);
    msgs& operator=(const msgs& ms);
    ~msgs();
public:
    std::vector<msg> m_array;
};

/**
  * @class <msgQuery>
  * @brief  组网消息查询指定时间段序列化.
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL msgQuery
{
public:
    msgQuery(void);
    msgQuery(msg m, long lPreTime, long lAftTime, unsigned long lPreDay, unsigned long lAftDay, std::string sStationNo, unsigned short iMatchingTime, std::string sQueueName);
    msgQuery(const msgQuery& mQ);
    msgQuery& operator=(const msgQuery& mQ);
    ~msgQuery();
public:
    msg m_m;
    long m_lPreTime;
    long m_lAftTime;
    unsigned long m_lPreDay;
    unsigned long m_lAftDay;
    std::string m_sStationNo;
    unsigned short m_iMatchingTime;
    std::string m_sQueueName;
};

///< TYPEDEF...
typedef msg MSGSTRUCT;

///< TYPEDEF...
typedef msg* LPMSGSTRUCT;

///< 消息队列元素集合类型...
typedef std::queue<MSGSTRUCT> MsgQueueArray;

typedef std::vector<MSGSTRUCT> MsgVectorArray;

// typedef std::list<MSGSTRUCT> MsgListArray;
typedef QList<MSGSTRUCT> MsgListArray;

typedef std::pair<QSharedPointer<QMutex>, MsgListArray> MUTEX_MSG_LIST_Array;

///< 消息队列与消息元素集合的映射
typedef std::map<std::string, MsgQueueArray> S_MSG_QUEUE_Array;

typedef std::map<std::string, MsgVectorArray> S_MSG_VECTOR_Array;

typedef std::map<std::string, MsgListArray> S_MSG_LIST_Array;

typedef std::map<std::string, MUTEX_MSG_LIST_Array> S_MSG_MUTEX_LIST_ARRAY;

// typedef QMap<std::string, MsgListArray> S_MSG_LIST_Array;

typedef std::map<std::string, std::vector<zframe_t *> > Q_CONSUMER_Array;

///< 消息队列与消息元素集合的映射
typedef std::map<unsigned int, MsgQueueArray> I_MSG_QUEUE_Array;

/**
  * @class <strmsgqueuearray>
  * @brief  指定队列的消息集合查询.
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL strmsgqueuearray
{
public:
    strmsgqueuearray(void);
    strmsgqueuearray(const strmsgqueuearray& sQA);
    strmsgqueuearray& operator=(const strmsgqueuearray& sQA);
    ~strmsgqueuearray();
public:
    std::string m_queuename;
    msgs m_msgs;
    msg	 m_m;
};


/**
  * @class <QUEUEINFOSTRUCT>
  * @brief  消息队列结构体
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
typedef struct QueueInfo
{
    unsigned int	queueID;				///< 消息队列ID
    std::string		queueName;				///< 消息队列名称
    unsigned int	level;					///< 消息队列等级
    unsigned int	type;					///< 消息队列类型
    unsigned long	curMsgSize;				///< 未处理的消息元素个数
    unsigned long	preMsgSize;				///< 已处理的消息元素个数(计数)
    unsigned int	curCustomers;			///< 当前消费者个数
    unsigned int	preCustomers;			///< 历史消息者个数(计数)
    std::vector<std::string> arrayConnID;	///< 当前连接数组集合（连接客户端的个数）

    QueueInfo()
    {
        this->queueID	= 1000;
        this->queueName	= "";
        this->level		= 0;
        this->type		= 0;
        this->curMsgSize= 0;
        this->preMsgSize= 0;
        this->curCustomers=0;
        this->preCustomers=0;
    }

    QueueInfo(unsigned int queueID, std::string queueName, unsigned int level, unsigned int type, unsigned long curMsgSize,
              unsigned long preMsgSize, unsigned int curCustomers, unsigned int preCustomers )
    {
        this->queueID	= queueID;
        this->queueName	= queueName;
        this->level		= level;
        this->type		= type;
        this->curMsgSize= curMsgSize;
        this->preMsgSize= preMsgSize;
        this->curCustomers=curCustomers;
        this->preCustomers=preCustomers;
    }

    QueueInfo(const QueueInfo& qI)
    {
        this->queueID	= qI.queueID;
        this->queueName	= qI.queueName;
        this->level		= qI.level;
        this->type		= qI.type;
        this->curMsgSize= qI.curMsgSize;
        this->preMsgSize= qI.preMsgSize;
        this->curCustomers = qI.curCustomers;
        this->preCustomers = qI.preCustomers;
        this->arrayConnID  = qI.arrayConnID;
    }

    QueueInfo& operator=(const QueueInfo& qI)
    {
        if ( this != &qI )
        {
            this->queueID	= qI.queueID;
            this->queueName	= qI.queueName;
            this->level		= qI.level;
            this->type		= qI.type;
            this->curMsgSize= qI.curMsgSize;
            this->preMsgSize= qI.preMsgSize;
            this->curCustomers = qI.curCustomers;
            this->preCustomers = qI.preCustomers;
            this->arrayConnID  = qI.arrayConnID;
        }

        return *this;
    }

} QUEUEINFOSTRUCT, *LPQUEUEINFOSTRUCT;

///< 消息队列集合
typedef std::vector<QUEUEINFOSTRUCT> QueueInfoArray;

/**
  * @class <queueQuery>
  * @brief  消息队列基本信息查询.
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL queueQuery
{
public:
    queueQuery(void);
    queueQuery(std::string i_queuename, const unsigned i_queueId);
    queueQuery(const queueQuery& qQ);
    queueQuery& operator=(const queueQuery& qQ);
    ~queueQuery();
public:
    unsigned int m_queueId;
    std::string m_queuename;
    QUEUEINFOSTRUCT m_qIS;
    QueueInfoArray m_qIArray;
    msg	 m_m;
};
/**
  * @class <CONNECTINFO>
  * @brief  客户端连接结构数据存储
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL ConnectInfo
{
public:
    ConnectInfo();
    ~ConnectInfo();
    ConnectInfo(bool bs, std::string s, std::string Id, time_t t_T);
    ConnectInfo(const ConnectInfo& cI);
    ConnectInfo& operator=(const ConnectInfo& cI);

public:
    bool state;				///< 网络状态
    std::string station;	///< 站号
    std::string connId;		///< 心跳标识
    time_t	connTime;		///< 心跳时间
};

///< 客户端连接集合
typedef std::vector<ConnectInfo> ConnectInfoArray;
typedef std::map<std::string, time_t> ConnectInfoMap;

class DLL_EXPORT_CLASS_DECL ConnectArray
{
public:
    ConnectArray();
    ~ConnectArray();
    ConnectArray(const ConnectArray& cTA);
    ConnectArray& operator=(const ConnectArray& cTA);
public:
    ConnectInfoArray m_cIA;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///< 通信模块结构体定义

enum PUBTYPE { INVALID = -1, SI_RADAR = 0, MO_RADAR, PUP_RADAR, SKY_RADAR, REQ_ARES, REQ_HIS, REP_HIS};

struct filedata
{
    size_t size;
    std::string name;
    std::string path;
    std::string buffer;

    filedata():name(""),path(""),buffer(""),size(0) {}

    /*~filedata() {}

    filedata(const std::string& nm, const std::string& p,const std::string& buf, size_t sz):name(nm),path(p),buffer(buf),size(sz) {}

    filedata(const filedata& fd):name(fd.name),path(fd.path),buffer(fd.buffer),size(fd.size) {}

    filedata& operator=(const filedata& fd)
    {
        if(&fd == this)
            return *this;

        name	= fd.name;
        path	= fd.path;
        buffer	= fd.buffer;
        size	= fd.size;

        return *this;
    }*/
};
///查询历史文件
struct QUERYHISFILE
{
    std::string m_areaCode;				///<区域
    std::string m_productID;				//产品ID
    time_t	m_startDateTime;			///<开始时间
    time_t	m_endDateTime;				///<结束时间
    std::vector<filedata> arrayFileList;///<文件列表
};
struct REQPRODUCTINFO
{
    PUBTYPE type;			//广播类型
    std::string area;		//区域
    time_t tm;				//日期，时间
    std::string productid;	//产品ID
    std::string productname;//产品英文名称
    filedata file;			//文件信息

    std::string data;		//扩展使用，可用于历史文件查询等 (体结构).

    REQPRODUCTINFO():file(),type(MO_RADAR),area(""),tm(time(NULL)),productid(""),productname(""),data("") {}

    /*~REQPRODUCTINFO()
    {
        area.clear();
        productid.clear();
        productname.clear();
    }

    REQPRODUCTINFO(const REQPRODUCTINFO& rPI):type(rPI.type),area(rPI.area),tm(rPI.tm),productid(rPI.productid),productname(rPI.productname),file(rPI.file),data(rPI.data) {}

    REQPRODUCTINFO& operator=(const REQPRODUCTINFO& rPI)
    {
        if ( this == &rPI )
            return *this;

        type = rPI.type;
        area = rPI.area;
        tm	 = rPI.tm;
        productid = rPI.productid;
        productname = rPI.productname;
        file = rPI.file;
        data = rPI.data;
        return *this;
    }*/
} ;

typedef struct AREACODEDEF
{
    vector<std::string> areaCode;
    vector<std::string> areaName;
} AreaCodeDef;

/**
 * @brief: 序列化msg消息结构 msg->std::string
 * @param: std::string & sMsg
 * @param: const msg & m
 * @return: DLL_EXPORT_C_DECL void: void
*/
DLL_EXPORT_DECL bool SerializeMsg2StdString(std::string& sMsg, const msg& m);

/**
 * @brief: 反序列化msg消息结构 std::string->msg
 * @param: const std::string & sMsg
 * @param: msg & m
 * @return: DLL_EXPORT_C_DECL void: void
*/
DLL_EXPORT_DECL bool DeserializeStdString2Msg(const std::string& sMsg, msg& m);

/**
 * @brief: 序列化msg消息结构 msg->*.xml
 * @param: std::string & sXmPath
 * @param: const msg & m
 * @return: DLL_EXPORT_C_DECL void: 描述返回值
*/
DLL_EXPORT_DECL bool SerializeMsg2XmlFile(std::string& sXmPath, const msg& m);

/**
 * @brief: 反序列化msg消息结构 *.xml->msg
 * @param: const std::string & sXmlPath
 * @param: msg & m
 * @return: DLL_EXPORT_C_DECL void: 描述返回值
*/
DLL_EXPORT_DECL bool DeserializeXmlFile2Msg(const std::string& sXmlPath, msg& m);

/**
 * @brief: 序列化msg消息结构 msg->*.*
 * @param: std::string & sFilePath
 * @param: const msg & m
 * @return: DLL_EXPORT_C_DECL void: 描述返回值
*/
DLL_EXPORT_DECL bool SerializeMsg2File(std::string& sFilePath, const msg& m);

/**
 * @brief: 反序列化msg消息结构 *.*->
 * @param: const std::string & sFilePath
 * @param: msg & m
 * @return: DLL_EXPORT_C_DECL void: 描述返回值
*/
DLL_EXPORT_DECL bool DeserializeFile2Msg(const std::string& sFilePath, msg& m);

/**
 * @brief: 序列化MSGQUEUEARRAY->*.xml
 * @param: std::string & sXmlPath
 * @param: const MSGQUEUEARRAY & QMS
 * @return: DLL_EXPORT_C_DECL void: 描述返回值
*/
DLL_EXPORT_DECL bool SerializeArrayMsg2XmlFile(std::string& sXmlPath, const msgs& QMS);

/**
 * @brief: 反序列化MSGQUEUEARRAY *.xml->MSGQUEUEARRAY
 * @param: const std::string & sXmlPath
 * @param: MSGQUEUEARRAY & QMS
 * @return: DLL_EXPORT_C_DECL void: 描述返回值
*/
DLL_EXPORT_DECL bool DeserializeXmlFile2ArrayMsg(const std::string& sXmlPath, msgs& QMS);

/**
 * @brief: 序列化MsgQuery对象到标准字符串中
 * @param: std::string & sMsg
 * @param: const msgQuery & m
 * @return: DLL_EXPORT_DECL bool: 成功返回true, 失败返回false.
*/
DLL_EXPORT_DECL bool SerializeMsgQuery2StdString(std::string& sMsg, const msgQuery& m);

/**
 * @brief: 从标准字符串反序列化MsgQuery对象
 * @param: const std::string & sMsg
 * @param: msgQuery & m
 * @return: DLL_EXPORT_DECL bool: 成功返回true, 失败返回false.
*/
DLL_EXPORT_DECL bool DeserializeStdString2MsgQuery(const std::string& sMsg, msgQuery& m);

/**
 * @brief: 序列化strmsgqueuearray对象至字符串
 * @param: std::string & sMsg
 * @param: const strmsgqueuearray & m
 * @return: DLL_EXPORT_DECL bool: 成功执行返回true, 失败返回false.
*/
DLL_EXPORT_DECL bool Serializestrmsgqueuearray2StdString(std::string& sMsg, const strmsgqueuearray& m);

/**
 * @brief: 反序列化至strmsgqueuearray对象
 * @param: const std::string & sMsg
 * @param: strmsgqueuearray & m
 * @return: DLL_EXPORT_DECL bool: 成功执行返回true, 失败返回false.
*/
DLL_EXPORT_DECL bool DeserializeStdString2strmsgqueuearray(const std::string& sMsg, strmsgqueuearray& m);

/**
 * @brief: 序列化queueQuery对象至字符串中
 * @param: std::string & sMsg
 * @param: const queueQuery & m
 * @return: DLL_EXPORT_DECL bool: 成功执行返回true, 失败返回false.
*/
DLL_EXPORT_DECL bool SerializequeueQuery2StdString(std::string& sMsg, const queueQuery& m);

/**
 * @brief: 序列化字符串至queueQuery对象
 * @param: const std::string & sMsg
 * @param: queueQuery & m
 * @return: DLL_EXPORT_DECL bool: 成功执行返回true, 失败返回false.
*/
DLL_EXPORT_DECL bool DeserializeStdString2queueQuery(const std::string& sMsg, queueQuery& m);

/**
 * @brief: 简述函数功能
 * @param: std::string & sMsg
 * @param: const ConnectInfo & cI
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool SerializeConnectionInfo2String(std::string& sMsg, const ConnectInfo& cI);

/**
 * @brief: 简述函数功能
 * @param: const std::string & sMsg
 * @param: ConnectInfo & cI
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool DeserializeStdString2ConnectionInfo(const std::string& sMsg, ConnectInfo& cI);

/**
 * @brief: 简述函数功能
 * @param: std::string & sMsg
 * @param: const ConnectArray & cI
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool SerializeConnectArray2String(std::string& sMsg, const ConnectArray& cI);

/**
 * @brief: 简述函数功能
 * @param: const std::string & sMsg
 * @param: ConnectArray & cI
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool DeserializeStdString2ConnectArray(const std::string& sMsg, ConnectArray& cI);

/**
 * @brief: 简述函数功能
 * @param: std::string & sMsg
 * @param: const REQPRODUCTINFO & reqPI
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool SerializeProductInfo2String(std::string& sMsg, const REQPRODUCTINFO& reqPI);

/**
 * @brief: 简述函数功能
 * @param: const std::string & sMsg
 * @param: REQPRODUCTINFO & reqPI
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool DeserialzeString2ProductInfo(const std::string& sMsg, REQPRODUCTINFO& reqPI);

/**
 * @brief: 简述函数功能
 * @param: std::string & sMsg
 * @param: const QUERYHISFILE & qHF
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool SerializeQueryHisFile2String(std::string& sMsg, const QUERYHISFILE& qHF);

/**
 * @brief: 简述函数功能
 * @param: const std::string & sMsg
 * @param: QUERYHISFILE & qHF
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool DeserializeString2QueryHisFile(const std::string& sMsg, QUERYHISFILE& qHF);

#endif//_H_H_MSG_H_H_


