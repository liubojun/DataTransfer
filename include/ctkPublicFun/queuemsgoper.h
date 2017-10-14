#ifndef queuemsgoper_h__
#define queuemsgoper_h__

#include "../macro.h"

#include <QReadWriteLock>

class CMsgQueueGroup;

/**
  * @class <>
  * @brief  消息队列配置文件操作类.
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL CQueueMsgOper
{
public:
    /**
     * @brief  默认构造函数
     * @return ：无
     */
    CQueueMsgOper();

    /**
     * @brief  析构函数
     * @return ：描述返回值
     */
    virtual ~CQueueMsgOper();

    /**
     * @brief  获取配置中所有队列组
     * @param  CMsgQueueGroup & io_queue 从文件中读取并返回的对象
     * @return bool：获取成功，返回true;失败返回false
     */
    static bool getQueueGroup(CMsgQueueGroup &io_queue);

    /**
     * @brief  保存消息组对象
     * @param  const CMsgQueueGroup & in_queue 消息组
     * @return bool：保存成功返回true；失败返回false
     */
    static bool saveQueueGroup(const CMsgQueueGroup &in_queue);


private:
    ///< 全局读写锁
    static QReadWriteLock s_oMutex;
};
#endif // queuemsgoper_h__
