#ifndef ftpclear_h__
#define ftpclear_h__

#include "LibCurlFtp.h"
#include "udplogsender.h"

class BaseDatas;

class CFtpClear
{
public:
    CFtpClear(BaseDatas &in_oData);

    virtual ~CFtpClear();

    void run();

private:
    BaseDatas &m_oData;
    CUdpLogSender m_oLogSender;
};
#endif // ftpclear_h__
