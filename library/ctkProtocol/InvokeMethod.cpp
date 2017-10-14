#include "InvokeMethod.h"

#include <QMetaObject>
#include <qmetatype.h>
#include "msg.h"

CInvokeMethod::CInvokeMethod(QObject* pParent) : QObject(pParent)
{
	qRegisterMetaType<msg>("msg");
	qRegisterMetaType<msgQuery>("msgQuery");
}

CInvokeMethod::~CInvokeMethod(void)
{

}

bool CInvokeMethod::DeserializeStdString2MsgQuery(const std::string& sMsg, msgQuery& m)
{
	bool bRet = true;

	bRet = ::DeserializeStdString2MsgQuery(sMsg, m);

	return bRet;
}