/**
  * @file   msgqueue.h
  * @brief  消息队列配置文件序列化-反序列化.
  *
  *  这里是详细说明。
  * @author 刘伯俊
  * @par   Copyright (c):
  *          刘伯俊.
  *         All Rights Reserved
  * @date   2016:07:11
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef msgqueue_h__
#define msgqueue_h__

#include "macro.h"
#include <string>
#include <map>
#include <vector>


#pragma warning(disable:4251)
#pragma warning(disable:4018)

/**
  * @class <>
  * @brief  消息队列配置文件类.(单个队列信息)
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */

class DLL_EXPORT_CLASS_DECL CMsgQueueSingle
{
public:
    /**
     * @brief  默认构造函数
     * @return ：无
     */
    CMsgQueueSingle();

    /**
     * @brief  默认析构函数
     * @return ：无
     */
    virtual ~CMsgQueueSingle();

public:
    ///< 显示在QLabel上的名称
    std::string m_strLabelName;

    ///< 消息队列名称
    std::string m_strQueueName;

    bool m_bIsRt;
};


class DLL_EXPORT_CLASS_DECL CMsgQueue
{
public:
    /**
     * @brief  默认构造函数
     * @return ：无
     */
    CMsgQueue();

    /**
     * @brief  默认析构函数
     * @return ：无
     */
    virtual ~CMsgQueue();

public:
    ///< 消息队列组名
    std::string m_groupName;

    ///< 消息队列组
    std::vector<CMsgQueueSingle> m_queue;
};

/**
  * @class <>
  * @brief  消息队列组 配置类.
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL CMsgQueueGroup
{
public:
    CMsgQueueGroup();

    virtual ~CMsgQueueGroup();

public:
    //
    std::map<std::string, CMsgQueue> m_queuedata;
};


/**
 * @brief  将消息队列组 配置对象序列化到xml配置文件中
 * @param  const CMsgQueueGroup & m 消息队列组对象
 * @param  const std::string & xmlFile xml配置文件全路径
 * @return bool：序列化成功返回true,失败返回false
 */
DLL_EXPORT_C_DECL bool SerializeQueue2Xml(const CMsgQueueGroup& m, const std::string &xmlFile);


/**
 * @brief  从xml配置文件中反序列化到消息队列组对象中
 * @param  const std::string & xmlFile xml配置文件全路径
 * @param  CMsgQueueGroup & m 消息队列组对象
 * @return bool：反序列化成功返回true,失败返回false
 */
DLL_EXPORT_C_DECL bool DeserializeXml2Queue(const std::string &xmlFile, CMsgQueueGroup& m);


#endif // msgqueue_h__
