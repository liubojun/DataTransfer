
#ifndef _H_H_INVOKEMETHOD_H_H_
#define _H_H_INVOKEMETHOD_H_H_

#include <QObject>

#include "../macro.h"
#include <string>
using namespace std;

class msg;
class msgQuery;

class DLL_EXPORT_CLASS_DECL CInvokeMethod : public QObject
{
	Q_OBJECT
public:
	CInvokeMethod(QObject* pParent=0);
	~CInvokeMethod(void);
public:
	Q_INVOKABLE bool DeserializeStdString2MsgQuery(const std::string& sMsg, msgQuery& m);
};

#endif