#ifndef udplogsender_h__
#define udplogsender_h__


#include <QUdpSocket>
#include <QString>
#include "macro.h"

class DLL_EXPORT_CLASS_DECL CUdpLogSender
{
public:
    CUdpLogSender();

    virtual ~CUdpLogSender();

    void sendCollectMsg(const QString &taskName, const QString &taskId, const QString &taskContent, int level);

    void sendClearMsg(const QString &taskName, const QString &taskId, const QString &taskContent, int level);

    void sendBroadCastMsg(const QString &taskName, const QString &srcfile, const QString &dstfile);

protected:
    void sendMsg(const QString &msgType, const QString &taskName, const QString &taskId, const QString &taskContent, int level);



private:
    QUdpSocket m_oLogSocket;
    QUdpSocket m_oBroadCastSocket;
    int m_iUdpLogPort;
    int m_iUdpBroadCastPort;
};
#endif // udplogsender_h__
