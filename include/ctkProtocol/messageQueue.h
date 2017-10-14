/**
  * @file   messageQueue.h
  * @brief  消息队列配置文件序列化.
  *
  *  这里是详细说明。
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:11:01
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef messageQueue_H_
#define messageQueue_H_

#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
#include "../macro.h"


class PupMosaicQueue
{
public:
    string queueName;		///< 消息队列名
    string productId;		///< 产品id
    string productEnName;	///< 产品英文缩写
    string productZhName;	///< 产品中文名称
    string colorName;		///< 色标名称
};

/**
  * @class <MsgQueue>
  * @brief  组网消息队列
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
 */
class MsgQueue
{
public:
    string groupName;				///< 组名     PUP_MOSAIC
    //map<string, string> mapQueue;	///< 队列名   ("R 0.5", "PUP_MOSAIC_R_0.5")
    //map<string, pair<string, string> > mapQueue;	///< 队列名 （"R 0.5", <"PUP_MOSAIC_R_0.5", "319">）
    map<string, PupMosaicQueue> pupQueue;	///< Label名-组网产品信息
};

DLL_EXPORT_DECL bool MsgQueueToXml(const string &strPath, const MsgQueue &set);
DLL_EXPORT_DECL bool XmlToMsgQueue(const string &strPath, MsgQueue &set);


#endif
