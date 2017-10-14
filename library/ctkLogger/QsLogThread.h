#ifndef QsLogThread_h__
#define QsLogThread_h__

#include <QThread>

/**
  * @class <>
  * @brief  线程ID获取.
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class QslogThread : public QThread
{
public:

    /**
     * @brief  获取当前线程ID
     * @return QT_NAMESPACE::quint32：当前线程ID
     */
    inline static quint32 threadId()
    {
        return (quint32) currentThreadId();
    }
};
#endif // QsLogThread_h__
