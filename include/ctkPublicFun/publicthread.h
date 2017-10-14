#ifndef publicthread_h_
#define publicthread_h_

#include <QThread>

// 定义此类是为了兼容qt4和qt5
class PublicThread : public QThread
{
public:
    PublicThread();
    ~PublicThread();

    // 使线程休眠msecs毫秒
    static void msleep ( unsigned long msecs )
    {
        QThread::msleep(msecs);
    }
};

#endif // publicthread_h_
