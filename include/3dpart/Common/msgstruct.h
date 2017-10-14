/**
  * @file   msgstruct.h
  * @brief  消息结构体描述.
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

#ifndef _H_H_MSGSTRUCT_H_H_
#define _H_H_MSGSTRUCT_H_H_

#include <queue>
#include <map>
#include <string>
#include <vector>

#include "../zeromq/zmq.hpp"

using namespace std;


//////////////////////////////////////////////////////////////////////////
//1)消息队列结构数据存储
//消息头
typedef struct
{
	unsigned int nQueueID;			//消息队列ID
	std::string  sQueueName;		//消息队列名称
	unsigned int nMsgID;			//消息ID
	time_t time;					//消息产生时间.
	//..........					//待扩充....
}MSGHEADER;

//消息体
typedef struct
{
	std::string s_data;
}MSGDATA;

//消息元素结构体
typedef struct
{
	MSGHEADER msgHeader;
	MSGDATA	  msgData;
	//..........							//待扩充....
}MSGSTRUCT,*LPMSGSTRUCT;

//消息队列元素集合类型...
typedef std::queue<MSGSTRUCT> MsgQueueArray;

//消息队列与消息元素集合的映射
typedef std::map<std::string, MsgQueueArray> S_MSG_QUEUE_Array;

//消息队列与消息元素集合的映射
typedef std::map<unsigned int, MsgQueueArray> I_MSG_QUEUE_Array;


//////////////////////////////////////////////////////////////////////////
//2)消息队列消费者结构数据存储
typedef struct //消息队列结构体
{
	unsigned int	queueID;				//消息队列ID
	std::string		queueName;				//消息队列名称
	unsigned int	level;					//消息队列等级
	unsigned int	type;					//消息队列类型
	unsigned long	curMsgSize;				//未处理的消息元素个数
	unsigned long	preMsgSize;				//已处理的消息元素个数(计数)
	unsigned int	curCustomers;			//当前消费者个数
	unsigned int	preCustomers;			//历史消息者个数(计数)
	std::queue<zmq::message_t> arrayConnID; //当前连接数组集合.
	//..........							//待扩充....
}QUEUEINFOSTRUCT, *LPQUEUEINFOSTRUCT;

//消息队列集合
typedef std::vector<QUEUEINFOSTRUCT> QueueInfoArray;



//////////////////////////////////////////////////////////////////////////
//3)客户端连接结构数据存储
typedef struct
{
	zmq::message_t connId;	//连接标识
	time_t	connTime;		//连接时间
	//......				//待扩充
}CONNECTINFO,*LPCONNECTINFO;

//客户端连接集合
typedef std::vector<CONNECTINFO> ConnectInfoArray;



#endif	//_H_H_PPISTRUCT_H_H_