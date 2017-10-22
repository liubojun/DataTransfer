#include <QCoreApplication>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QDir>
#include <iostream>
#include "pathbuilder.h"


QString CPathBuilder::getTranslatorFilePath(language_t lan)
{
    makeBuilderOrRunning();
    QString translatorPath = m_strProgramPath +  "/i18n/";

    QStringList qmfiles = QDir(translatorPath).entryList();

    // 翻译家路径
    int ilen = sizeof(l2f)/sizeof(language2file);


    const char *pSuffix = NULL;
    for (int i = 0; i < ilen; ++i)
    {

        if (l2f[i].oLan == lan)
        {
            pSuffix = l2f[i].filesuffix;
            break;
        }
    }

    translatorPath = translatorPath + qApp->applicationName() + "_" + pSuffix + ".qm";
    if (QFileInfo(translatorPath).exists())
    {
        return translatorPath;
    }
    else
    {
        return QString();
    }
    /*foreach (const QString &f, qmfiles)
    {
    int aIndex = f.indexOf("_");
    int bIndex = f.indexOf(".");
    if (aIndex != -1 && bIndex != -1 && bIndex > aIndex)
    {
    QString suffix = f.mid(aIndex+1, bIndex - aIndex -1);
    if (suffix == QString(pSuffix))
    {
    translatorPath.append("/").append(f);
    break;
    }
    }

    }*/

    // return translatorPath;
}

QString CPathBuilder::getPluginManagerPath()
{
    if (makeBuilderOrRunning())
    {
        return qApp->applicationDirPath().toLocal8Bit();
    }
    else
    {
#ifdef _WIN32
        std::cout << (qApp->applicationDirPath() + "/plugins/ctkPluginManager").toStdString() << std::endl;
        return qApp->applicationDirPath() + "/plugins/ctkPluginManager";
#else
        return qApp->applicationDirPath();
#endif
        // return m_strProgramPath;
    }
}

bool CPathBuilder::makeBuilderOrRunning()
{
    QString startup = QCoreApplication::applicationDirPath();

    QString prefixPath = qApp->applicationDirPath();

    QFile f;
    foreach ( QString path, QStringList() << "" << "/.." << "/bin" << "/../../.." )
    {
        f.setFileName( prefixPath + path + "/radarprojectbuildpath.txt" );
        if ( f.exists() )
        {
            break;
        }
    }

    QString translatorpath;

    // 是编译时
    if ( f.exists() && f.open( QIODevice::ReadOnly ) )
    {
        m_strProgramPath = f.readLine();
        return true;
    }
    else
    {
        m_strProgramPath = startup;
        return false;
    }
}

QString CPathBuilder::getPluginsPath()
{
    QString strPluginPath;

    QString prefixPath = QCoreApplication::applicationDirPath();

    if (makeBuilderOrRunning())
    {
        //文件记录路径与startupPath之间，剩余的为Cfg路径
        QString mCfgIntDir;
        if (prefixPath.length() > m_strProgramPath.length())
        {
            mCfgIntDir = prefixPath.mid(m_strProgramPath.length());
        }

        strPluginPath = m_strProgramPath + "plugins/" + mCfgIntDir;
    }
    else
    {
        strPluginPath = prefixPath + "/plugins";
    }
    return strPluginPath;
}

QString CPathBuilder::getAlgorithmPath()
{
    QString strAlgorithmpath;
    if (makeBuilderOrRunning())
    {
        strAlgorithmpath = qApp->applicationDirPath();
    }
    else
    {
        strAlgorithmpath = qApp->applicationDirPath() + "/algorithm";
    }
    return strAlgorithmpath;
}

QString CPathBuilder::getConfigPath()
{
    QString strConfigPath;
    if (makeBuilderOrRunning())
    {
        strConfigPath = qApp->applicationDirPath() + "/config";
    }
    else
    {
        strConfigPath = qApp->applicationDirPath() + "/config";
    }

    return strConfigPath;
}

QString CPathBuilder::getGisPluginsPath()
{
    QString strPluginPath;

    QString prefixPath = QCoreApplication::applicationDirPath();

    if (makeBuilderOrRunning())
    {
        //文件记录路径与startupPath之间，剩余的为Cfg路径
        QString mCfgIntDir;
        if (prefixPath.length() > m_strProgramPath.length())
        {
            mCfgIntDir = prefixPath.mid(m_strProgramPath.length());
        }

        strPluginPath = m_strProgramPath + "gisplugins/" + mCfgIntDir;
    }
    else
    {
        strPluginPath = prefixPath + "/gisplugins";
    }
    return strPluginPath;
}

QString CPathBuilder::getBaseMapsPath()
{
    QString strMapPath;
    strMapPath = qApp->applicationDirPath() + "/maps";
    return strMapPath;
}

QString CPathBuilder::getResourcesPath()
{
    QString strResourcesPath;
    strResourcesPath = qApp->applicationDirPath() + "/resources";
    return strResourcesPath;
}

QString CPathBuilder::getImagePath()
{
    QString strImagePath;
    strImagePath = qApp->applicationDirPath() + "/image";
    return strImagePath;
}

QString CPathBuilder::getCurlFilePath()
{
    QString strFilePath = qApp->applicationDirPath() + "/tmp/download";
    return strFilePath;
}

QString CPathBuilder::getFinalPathFromUrl(const QString &path)
{
    QString strNewUrl;

    // %T-1H %T表示当前的系统时间 -1H表示基于当前系统时间的增减量，还支持天d、分钟M、秒s
    // %T-1H%t%y/%t%m/%t%d/%t%H/%t%M/%t%s, %t后接时间，%y%m%d%H%M%s分别表示年月日时分秒
    //QUrl oUrl(path);
    QString strUrl = path;//oUrl.path();

    QString strFinalUrl;
    QStringList urlList = strUrl.split("/");

    //
    for (int i = 0; i != path.length(); ++i)
    {
        if ('/' != path.at(i))
        {
            break;
        }
        strFinalUrl.append(path.at(i));
    }
    foreach(QString str, urlList)
    {
        if (str.isEmpty())
        {
            continue;
        }
        strFinalUrl.append(parseFromUrl(str, QDateTime::currentDateTime()));
    }

    //QSLOG_DEBUG("final url = " + strFinalUrl);
    return strFinalUrl;
}

QString CPathBuilder::getFinalPathFromUrl(const QString &path, QDateTime in_oDt)
{
    QString strNewUrl;

    // %T-1H %T表示当前的系统时间 -1H表示基于当前系统时间的增减量，还支持天d、分钟M、秒s
    // %T-1H%t%y/%t%m/%t%d/%t%H/%t%M/%t%s, %t后接时间，%y%m%d%H%M%s分别表示年月日时分秒
    //QUrl oUrl(path);
    QString strUrl = path;//oUrl.path();

    QString strFinalUrl;
    QStringList urlList = strUrl.split("/");

    //
    for (int i = 0; i != path.length(); ++i)
    {
        if ('/' != path.at(i))
        {
            break;
        }
        strFinalUrl.append(path.at(i));
    }
    foreach(QString str, urlList)
    {
        if (str.isEmpty())
        {
            continue;
        }
        strFinalUrl.append(parseFromUrl(str, in_oDt));
    }

    //QSLOG_DEBUG("final url = " + strFinalUrl);
    return strFinalUrl;
}

QString CPathBuilder::parseFromUrl(const QString &url, QDateTime in_oDt)
{
    // 最终的时间

    QDateTime finalDateTime = in_oDt;

    QString strNewUrl("");
    QString strFinalUrl;
    QString strUrl = url;

    int index0 = 0;
    int index1 = 0;

    if (-1 != (index1 = strUrl.indexOf("%T")))
    {
        //QSLOG_DEBUG(QString("find add or sub character"));
        // 找到%T与%t之间的内容

        int index2 = strUrl.indexOf("%t", index1);

        if (-1 == index2)
        {
            //QSLOG_ERROR("check url format error, should be with t after T");
            return strNewUrl;
        }
        // 截断之间的内容
        QString valueRange = strUrl.mid(index1 + 2, index2 - index1 - 2);

        // 分析第一个是+号还是-号
        QChar c = valueRange.at(0);
        if ('-' == c || '+' == c)
        {
            // 计算当前时间的增减量
            int addorsub = 1;
            if ('-' == c)
            {
                addorsub = -1;
            }

            // 再计算后面的时间内容
            // 确定valueRange的最后一个字符
            QChar lastc = valueRange.at(valueRange.length() - 1);
            int iTime = valueRange.mid(1, valueRange.length() - 2).toInt() * addorsub;

            int step = 0;
            if ('H' == lastc)
            {
                finalDateTime = finalDateTime.addSecs(iTime * 3600);
                //QSLOG_DEBUG(QString("%1 H").arg(iTime));
            }
            else if ('d' == lastc)
            {
                finalDateTime = finalDateTime.addDays(iTime);
                //QSLOG_DEBUG(QString("%1 d").arg(iTime));
            }
            else if ('M' == lastc)
            {
                finalDateTime = finalDateTime.addSecs(iTime * 60);
                //QSLOG_DEBUG(QString("%1 M").arg(iTime));
            }
            else if ('s' == lastc)
            {
                finalDateTime = finalDateTime.addSecs(iTime);
                //QSLOG_DEBUG(QString("%1 s").arg(iTime));
            }
            else
            {
                //QSLOG_ERROR("check unsupport character after %T should be H / d / M / s");
                return strNewUrl;
            }
        }
        else
        {
            // QSLOG_ERROR("The character after %T is incorrect, should be + or -");
            return strNewUrl;
        }

        QStringList strTimeBody = strUrl.mid(index2).split("%t");
        QString strDateTimeFormat;
        foreach(QString strBody, strTimeBody)
        {
            if (strBody.isEmpty())
            {
                continue;
            }
            // %y%m%d%H%M%s
            if ("%Y" == strBody)
            {
                strDateTimeFormat.append("yyyy");
            }
            else if ("%m" == strBody)
            {
                strDateTimeFormat.append("MM");
            }
            else if ("%d" == strBody)
            {
                strDateTimeFormat.append("dd");
            }
            else if ("%H" == strBody)
            {
                strDateTimeFormat.append("hh");
            }
            else if ("%M" == strBody)
            {
                strDateTimeFormat.append("mm");
            }
            else if ("%S" == strBody)
            {
                strDateTimeFormat.append("ss");
            }
            else
            {
                //QSLOG_ERROR("The character after %t is incorrect, should be ymdHMs ");
                return strNewUrl;
            }
        }

        strFinalUrl.append(finalDateTime.toString(strDateTimeFormat));

    }
    else if (-1 != (index1 = strUrl.indexOf("%t")))
    {
        // QSLOG_DEBUG(QString("find time character"));
        QStringList strTimeBody = strUrl.split("%t");
        QString strDateTimeFormat;
        foreach(QString strBody, strTimeBody)
        {
            if (strBody.isEmpty())
            {
                continue;
            }
            // %y%m%d%H%M%s
            if ("%Y" == strBody)
            {
                strDateTimeFormat.append("yyyy");
            }
            else if ("%m" == strBody)
            {
                strDateTimeFormat.append("MM");
            }
            else if ("%d" == strBody)
            {
                strDateTimeFormat.append("dd");
            }
            else if ("%H" == strBody)
            {
                strDateTimeFormat.append("hh");
            }
            else if ("%M" == strBody)
            {
                strDateTimeFormat.append("mm");
            }
            else if ("%S" == strBody)
            {
                strDateTimeFormat.append("ss");
            }
            else
            {
                // QSLOG_ERROR("The character after %t is incorrect, should be ymdHMs");
                return strNewUrl;
            }
        }

        strFinalUrl.append(finalDateTime.toString(strDateTimeFormat));
    }
    else
    {
        strFinalUrl.append(url);
    }

    return strFinalUrl + "/";
}

