#ifndef commonfunction_h__
#define commonfunction_h__

#include <QString>

//bool olderThen(const QString &str1,const QString &str2);

/**
 * @brief  匹配数据文件
 * @param  const QString & str：数据文件名
 * @param  const QString & strRegex：正则表达式
 * @return bool：匹配返回true，否则false
 */
bool matchStr(const QString &str, const QString &strRegex);

#endif // commonfunction_h__
