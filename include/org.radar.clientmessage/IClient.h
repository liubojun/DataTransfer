/**
  * @file   IClient.h
  * @brief  消息总线客户端二次封装.
  *
  *  这里是详细说明。
  * @author WEIKANGC
  * @par   Copyright (c):
  *          WEIKANGC.
  *         All Rights Reserved
  * @date   2016:04:11
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef _H_H_ICLIENT_H_H_
#define _H_H_ICLIENT_H_H_

#include <QObject>
#include <QString>

#include "msg.h"
#include "ILogPrint.h"

class IClient : public ILogPrint
{
    Q_OBJECT
public:
    virtual ~IClient() {}
public:
    /**
     * @brief:该接口主要是为各业务插件提供向服务器连接的请求，并返回连接成功与否标识。
              原则上各业务插件不允许向服务器发起连接，均要通过Client端向外提供的接口来实现相应的功能.
     * @param: const std::string sIP：IP地址
     * @param: const unsigned int nPort：端口号
     * @return: bool: 连接成功返回true，失败返回false
    */
    virtual bool connect( const std::string& sIP, unsigned int nPort ) = 0;

	/**
	 * @brief: 断开通信连接
	 * @return: bool: 描述返回值
	*/
	virtual bool disconnect() = 0 ;

    /**
     * @brief: 该接口主要是为各业务插件提供向消息总线服务端消息入队的功能.
     * @param: 【in】const LPMSGSTRUCT lpMsgStruct：消息结构体
     * @return: bool: 入队成功返回true，失败返回false
    */
    virtual bool pushMsg( const LPMSGSTRUCT lpMsgStruct ) = 0;

    /**
     * @brief: 该接口主要是为各业务插件提供向消息总线服务端消息入队的功能.
     * @param: 【in】const MSGSTRUCT & msgStruct
     * @return: bool: 描述返回值
    */
    virtual bool pushMsg( MSGSTRUCT& msgStruct ) = 0;

    /**
     * @brief: 该接口主要是为各业务插件提供向消息总线服务端获取消息的功能.
     * @param: 【in|out】LPMSGSTRUCT lpMsgStruct
     * @return: bool: 描述返回值
    */
    virtual bool popMsg( LPMSGSTRUCT lpMsgStruct ) = 0;
	
    /**
     * @brief: 该接口主要是为各业务插件提供向消息总线服务端获取消息的功能.
     * @param: 【in|out】MSGSTRUCT & msgStruct
     * @return: bool: 描述返回值
    */
    virtual bool popMsg( MSGSTRUCT & msgStruct ) = 0;
	
    /**
     * @brief: 该接口主要是为各业务插件提供向消息总线服务端查询消息队列信息的功能.
     * @param: 【in|out】std::vector<QUEUEINFOSTRUCT> * lpArrayMsgQueues
     * @return: bool: 描述返回值
    */
    virtual bool queryQueues ( std::vector<QUEUEINFOSTRUCT>* lpArrayMsgQueues ) = 0;

    /**
     * @brief: 该接口主要是为各业务插件提供向消息总线服务端查询消息队列信息的功能.
     * @param: 【in|out】std::vector<QUEUEINFOSTRUCT> & arrayMsgQueues
     * @return: bool: 描述返回值
    */
    virtual bool queryQueues ( std::vector<QUEUEINFOSTRUCT>& arrayMsgQueues ) = 0;
	
    /**
     * @brief: 该接口主要是为各业务插件提供通过消息队列名称向消息总线服务端查询指定队列的消息集
     * @param: 【in】 const std::string sQueueName
     * @param: 【in|out】 LPQUEUEINFOSTRUCT lpMsgQueuesInfo
     * @return: bool: 描述返回值
    */
    virtual bool queryMsgs ( const std::string& sQueueName, LPQUEUEINFOSTRUCT lpMsgQueuesInfo ) = 0;

    /**
     * @brief: 该接口主要是为各业务插件提供通过消息队列名称向消息总线服务端查询指定队列的消息集
     * @param: 【in】 const std::string sQueueName
     * @param: 【in|out】QUEUEINFOSTRUCT & msgQueuesInfo
     * @return: bool: 描述返回值
    */
    virtual bool queryMsgs ( const std::string& sQueueName, QUEUEINFOSTRUCT& msgQueuesInfo ) =0;

    /**
     * @brief: 该接口主要是为各业务插件提供通过消息队列名称向消息总线服务端查询指定队列的消息集
     * @param: 【in】 const unsigned int nQueueID
     * @param: 【in|out】QUEUEINFOSTRUCT & msgQueuesInfo
     * @return: bool: 描述返回值
    */
    virtual bool queryMsgs ( const unsigned int nQueueID, QUEUEINFOSTRUCT& msgQueuesInfo ) = 0;

    /**
     * @brief: 该接口主要是为各业务插件提供通过消息队列名称向消息总线服务端查询指定队列的消息集
     * @param: const unsigned int nQueueID
     * @param: LPQUEUEINFOSTRUCT lpMsgQueuesInfo
     * @return: bool: 描述返回值
    */
    virtual bool queryMsgs ( const unsigned int nQueueID, LPQUEUEINFOSTRUCT lpMsgQueuesInfo )  = 0;

    /**
     * @brief: 通过队列名称查询队列中消息的集合.
     * @param: const std::string & sQueueName
     * @param: std::vector<MSGSTRUCT> & ArrayMsgs
     * @return: bool: 成功返回true, 失败返回false.
    */
    virtual bool queryMsgs(const std::string& sQueueName, std::vector<MSGSTRUCT> &ArrayMsgs) = 0 ;

    /**
     * @brief: 通过队列名称查询队列中消息的集合
     * @param: const std::string & sQueueName
     * @param: std::vector<MSGSTRUCT> * LPArrayMsgs
     * @return: bool: 成功返回true, 失败返回false.
    */
    virtual bool queryMsgs(const std::string& sQueueName, std::vector<MSGSTRUCT> *lPArrayMsgs) = 0 ;

    /**
     * @brief: 发送心跳包消息
     * @param: const MSGSTRUCT & msgStruct
     * @return: bool: 成功返回true, 失败返回false.
    */
    virtual bool heartbeat(const MSGSTRUCT& msgStruct) = 0 ;

    /**
     * @brief  查询心跳包
     * @param  ConnectInfoArray & msgStruct：消息结构体
     * @return bool：查询成功返回true，否则返回false
     */
    virtual bool queryhearbeat(ConnectInfoArray& msgStruct) = 0 ;

signals:
    /**
     * @brief  接收到消息的信号
     * @param  const MSGSTRUCT & msgStruct：消息结构体
     * @return void：无
     */
    void msgIncoming(const MSGSTRUCT& msgStruct);

};


#endif // _H_H_ICLIENT_H_H_
