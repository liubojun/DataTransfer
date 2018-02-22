#include "subdirfilter.h"
#include "ctkLog.h"

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>

CSubDirFilter::CSubDirFilter()
{

}

CSubDirFilter::~CSubDirFilter()
{

}

void CSubDirFilter::init()
{
    QString rulefile = QCoreApplication::applicationDirPath() + "/config/subdirfilter.rule";

    QFile oFile(rulefile);
    if (!oFile.open(QIODevice::ReadOnly))
    {
        QSLOG_ERROR("open subdirfilter failure.");
        return;
    }

    QTextStream stream(&oFile);
    stream.setCodec("UTF-8");
    while (!stream.atEnd())
    {
        QString strLine = stream.readLine().trimmed();
        if (strLine.startsWith("#"))
        {
            continue;
        }
        if (strLine.isEmpty())
        {
            continue;
        }

        if (strLine.startsWith("[") && strLine.endsWith("]"))
        {
            // 找到一个规则片区，处理完当前的规则片区
            QString strRuleId = strLine.mid(1, strLine.length() - 2);
            Level2filterRule level2Rule;

            while (!stream.atEnd())
            {
                strLine = stream.readLine().trimmed();
                if (strLine.startsWith("#"))
                {
                    break;
                }
                if (strLine.isEmpty())
                {
                    break;
                }

                // 以等号作为分割
                int index = strLine.indexOf("=");
                QString strKey = strLine.mid(0, index);
                QString strValue = strLine.mid(index+1);
                level2Rule.insert(strKey, strValue);
            }

            m_oRule.insert(strRuleId, level2Rule);
        }
    }
}

bool CSubDirFilter::match(const QString &ruleId, const QString &dirname, int iLevel)
{
    FilterRule::iterator iter = m_oRule.find(ruleId);
    if (iter != m_oRule.end())
    {
        QString strLevel = QString("level%1").arg(iLevel);
        Level2filterRule::iterator iter2 = iter->find(strLevel);
        if (iter2 != iter->end())
        {
            QStringList strDirList = m_oPathBuilder.getFinalPathFromUrl(iter2.value());
            foreach (QString strDir, strDirList)
            {
                QRegExp oExp(strDir.mid(0, strDir.length()-1));
                if (oExp.exactMatch(dirname))
                {
                    return true;
                }
            }
            return false;
        }
    }

    return true;
}

QStringList CSubDirFilter::getAllRuleId()
{
    return m_oRule.keys();
}
