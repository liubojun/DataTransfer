/**
  * @file   IClientMessage.h
  * @brief  消息总线客户端公开接口声明.
  *
  *  这里是详细说明。
  * @author WEIKANGC
  * @par   Copyright (c):
  *        WEIKANGC.
  *        All Rights Reserved
  * @date   2016:04:11
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef _H_H_ICLIENTMESSAGE_H_H_
#define _H_H_ICLIENTMESSAGE_H_H_

#include "IBaseInterface.h"
#include "IClient.h"
#include <QSharedPointer>

/**
  * @class <IClientMessage>
  * @brief  消息总线客户端公开接口描述.
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */

class IClientMessage : public IBaseInterface
{
    Q_OBJECT
    Q_INTERFACES(IBaseInterface)
public:
    /**
     * @brief: 虚析构函数
     * @return: : 无
    */
    virtual ~IClientMessage() {}


    /**
     * @brief: 创建客户端
     * @return: QSharedPointer<IClient>: 实例指针
    */
    virtual QSharedPointer<IClient> createClient() = 0;

    /**
     * @brief:该接口主要是为各业务插件提供向服务器连接的请求，并返回连接成功与否标识。
              原则上各业务插件不允许向服务器发起连接，均要通过Client端向外提供的接口来实现相应的功能.
     * @param: int nType
     * @param: const std::string sIP
     * @param: const unsigned int nPort
     * @return: bool: 描述返回值
    */
    //virtual bool connect( int nType, const std::string sIP, const unsigned int nPort ) = 0;

    /**
     * @brief: 该接口主要是为各业务插件提供向消息总线服务端消息入队的功能.
     * @param: 【in】const LPMSGSTRUCT lpMsgStruct
     * @return: bool: 描述返回值
    */
    //virtual bool pushMsg( const LPMSGSTRUCT lpMsgStruct ) = 0;

    /**
     * @brief: 该接口主要是为各业务插件提供向消息总线服务端消息入队的功能.
     * @param: 【in】const MSGSTRUCT & msgStruct
     * @return: bool: 描述返回值
    */
    //virtual bool pushMsg( const MSGSTRUCT& msgStruct ) = 0;

    /**
     * @brief: 该接口主要是为各业务插件提供向消息总线服务端获取消息的功能.
     * @param: 【in|out】LPMSGSTRUCT lpMsgStruct
     * @return: bool: 描述返回值
    */
    //virtual bool popMsg( LPMSGSTRUCT lpMsgStruct ) = 0;


    /**
     * @brief: 该接口主要是为各业务插件提供向消息总线服务端获取消息的功能.
     * @param: 【in|out】MSGSTRUCT & msgStruct
     * @return: bool: 描述返回值
    */
    //virtual bool popMsg( MSGSTRUCT & msgStruct ) = 0;


    /**
     * @brief: 该接口主要是为各业务插件提供向消息总线服务端查询消息队列信息的功能.
     * @param: 【in|out】std::vector<QUEUEINFOSTRUCT> * lpArrayMsgQueues
     * @return: bool: 描述返回值
    */
    //virtual bool queryQueues ( std::vector<QUEUEINFOSTRUCT>* lpArrayMsgQueues ) = 0;

    /**
     * @brief: 该接口主要是为各业务插件提供向消息总线服务端查询消息队列信息的功能.
     * @param: 【in|out】std::vector<QUEUEINFOSTRUCT> & arrayMsgQueues
     * @return: bool: 描述返回值
    */
    //virtual bool queryQueues ( std::vector<QUEUEINFOSTRUCT>& arrayMsgQueues ) = 0;


    /**
     * @brief: 该接口主要是为各业务插件提供通过消息队列名称向消息总线服务端查询指定队列的消息集
     * @param: 【in】 const std::string sQueueName
     * @param: 【in|out】 LPQUEUEINFOSTRUCT lpMsgQueuesInfo
     * @return: bool: 描述返回值
    */
    //virtual bool queryMsgs ( const std::string sQueueName, LPQUEUEINFOSTRUCT lpMsgQueuesInfo ) = 0;

    /**
     * @brief: 该接口主要是为各业务插件提供通过消息队列名称向消息总线服务端查询指定队列的消息集
     * @param: 【in】 const std::string sQueueName
     * @param: 【in|out】QUEUEINFOSTRUCT & msgQueuesInfo
     * @return: bool: 描述返回值
    */
    //virtual bool queryMsgs ( const std::string sQueueName, QUEUEINFOSTRUCT& msgQueuesInfo ) =0;

    /**
     * @brief: 该接口主要是为各业务插件提供通过消息队列名称向消息总线服务端查询指定队列的消息集
     * @param: 【in】 const unsigned int nQueueID
     * @param: 【in|out】QUEUEINFOSTRUCT & msgQueuesInfo
     * @return: bool: 描述返回值
    */
    //virtual bool queryMsgs ( const unsigned int nQueueID, QUEUEINFOSTRUCT& msgQueuesInfo ) = 0;

    /**
     * @brief: 该接口主要是为各业务插件提供通过消息队列名称向消息总线服务端查询指定队列的消息集
     * @param: const unsigned int nQueueID
     * @param: LPQUEUEINFOSTRUCT lpMsgQueuesInfo
     * @return: bool: 描述返回值
    */
    //virtual bool queryMsgs ( const unsigned int nQueueID, LPQUEUEINFOSTRUCT lpMsgQueuesInfo )  = 0;

};

Q_DECLARE_INTERFACE(IClientMessage, "org.radar.clientmessage.IClientMessage")

#endif //_H_H_ICLIENTMESSAGE_H_H_