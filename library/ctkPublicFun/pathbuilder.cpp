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
