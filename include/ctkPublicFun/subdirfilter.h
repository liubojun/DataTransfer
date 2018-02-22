#ifndef subdirfilter_h__
#define subdirfilter_h__

#include <QMap>
#include <QString>

#include "../macro.h"
#include "pathbuilder.h"

typedef QMap<QString, QString> Level2filterRule;

typedef QMap<QString, Level2filterRule> FilterRule;

// 针对民航amc新需求，需要支持对子目录进行按照文件名过滤

class DLL_EXPORT_CLASS_DECL CSubDirFilter
{
public:
    CSubDirFilter();

    virtual ~CSubDirFilter();

    /**
     * @brief:完成针对subdirfilter.rule配置文件的读取
     * @return:void 无
     */
    void init();

    /**
     * @brief: 获取所有规则ID
     * @return:QT_NAMESPACE::QStringList 所有规则ID
     */
    QStringList getAllRuleId();

    /**
     * @brief:根据规则ID以及目录名称、目录层级进行匹配
     * @param: const QString & ruleId 规则标识ID
     * @param: const QString & dirname 目录名称
     * @param: int iLevel 目录层级
     * @return: 返回true，表明该目录被规则所匹配；false，表明该目录未被规则所匹配
     */
    bool match(const QString &ruleId, const QString &dirname, int iLevel);

private:
    FilterRule m_oRule;
    CPathBuilder m_oPathBuilder;
};
#endif // subdirfilter_h__
