#ifndef ftpclear_h__
#define ftpclear_h__

#include "LibCurlFtp.h"
class BaseDatas;

class CFtpClear
{
public:
	CFtpClear(BaseDatas &in_oData);

	virtual ~CFtpClear();

	void run();

private:
	BaseDatas &m_oData;
};
#endif // ftpclear_h__
