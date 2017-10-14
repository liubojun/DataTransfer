#include "qfileinfo.h"
#include "commonfunction.h"
#include <QRegExp>
#include <QDateTime>

// bool olderThen( const QString &str1,const QString &str2 )
// {
// 	QFileInfo f1(str1);
// 	QFileInfo f2(str2);
// 	return f1.lastModified() < f2.lastModified();
// }

bool matchStr(const QString &str, const QString &strRegex)
{
	QRegExp rx(strRegex);
	return rx.exactMatch(str);
}
