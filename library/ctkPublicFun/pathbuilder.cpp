#include <QCoreApplication>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QDir>
#include <iostream>
#include <QDebug>
#include <QProcess>
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
        return qApp->applicationDirPath();
//#ifdef _WIN32
//        std::cout << (qApp->applicationDirPath() + "/plugins/ctkPluginManager").toStdString() << std::endl;
//        return qApp->applicationDirPath() + "/plugins/ctkPluginManager";
//#else
//        return qApp->applicationDirPath();
//#endif
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

QStringList CPathBuilder::getFinalPathFromUrl(const QString &path)
{
    QString strNewUrl;

    // %T-1H %T表示当前的系统时间 -1H表示基于当前系统时间的增减量，还支持天d、分钟M、秒s
    // %T-1H%t%y/%t%m/%t%d/%t%H/%t%M/%t%s, %t后接时间，%y%m%d%H%M%s分别表示年月日时分秒
    // modified by liubojun @210171029,添加时间增减量的支持
    // %T-[0,1]d ,表示基于当前系统时间，获取当前的天以及前一天
    //QUrl oUrl(path);
    QString strUrl = path;//oUrl.path();

    QStringList retUrls;
    // 先插入一个元素
    retUrls.append("");

    QString &strFinalUrl = retUrls[0];

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
        QStringList allUrls = parseFromUrl(str, QDateTime::currentDateTime());

        // 如果返回了多条记录，原有URL需要进行复制
        int size = allUrls.size();
        for (int i = 0; i < size - 1; ++i)
        {
            retUrls.append(retUrls);
        }

        for (int i = 0; i < retUrls.size(); ++i)
        {
            QString &refUrl = retUrls[i];
            refUrl.append(allUrls[i % size]);
        }

        // strFinalUrl.append(parseFromUrl(str, QDateTime::currentDateTime()));
    }

    //QSLOG_DEBUG("final url = " + strFinalUrl);
    return retUrls;
    // strFinalUrl;
}

QStringList CPathBuilder::getFinalPathFromUrl(const QString &path, QDateTime in_oDt)
{
    //QStringList retUrls;
    //QString strNewUrl;

    //// %T-1H %T表示当前的系统时间 -1H表示基于当前系统时间的增减量，还支持天d、分钟M、秒s
    //// %T-1H%t%y/%t%m/%t%d/%t%H/%t%M/%t%s, %t后接时间，%y%m%d%H%M%s分别表示年月日时分秒
    ////QUrl oUrl(path);
    //QString strUrl = path;//oUrl.path();

    //QString strFinalUrl;
    //QStringList urlList = strUrl.split("/");

    ////
    ////for (int i = 0; i != path.length(); ++i)
    ////{
    ////    if ('/' != path.at(i))
    ////    {
    ////        break;
    ////    }
    ////    strFinalUrl.append(path.at(i));
    ////}
    ////foreach(QString str, urlList)
    ////{
    ////    if (str.isEmpty())
    ////    {
    ////        continue;
    ////    }
    ////    QStringList allUrls = parseFromUrl(str, in_oDt);
    ////    // strFinalUrl.append(parseFromUrl(str, in_oDt));
    ////}

    ////QSLOG_DEBUG("final url = " + strFinalUrl);
    //return retUrls;// strFinalUrl;

    QString strNewUrl;

    // %T-1H %T表示当前的系统时间 -1H表示基于当前系统时间的增减量，还支持天d、分钟M、秒s
    // %T-1H%t%y/%t%m/%t%d/%t%H/%t%M/%t%s, %t后接时间，%y%m%d%H%M%s分别表示年月日时分秒
    // modified by liubojun @210171029,添加时间增减量的支持
    // %T-[0,1]d ,表示基于当前系统时间，获取当前的天以及前一天
    //QUrl oUrl(path);
    QString strUrl = path;//oUrl.path();

    QStringList retUrls;
    // 先插入一个元素
    retUrls.append("");

    QString &strFinalUrl = retUrls[0];

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
        QStringList allUrls = parseFromUrl(str, in_oDt);

        // 如果返回了多条记录，原有URL需要进行复制
        int size = allUrls.size();
        for (int i = 0; i < size - 1; ++i)
        {
            retUrls.append(retUrls);
        }

        for (int i = 0; i < retUrls.size(); ++i)
        {
            QString &refUrl = retUrls[i];
            refUrl.append(allUrls[i % size]);
        }

        // strFinalUrl.append(parseFromUrl(str, QDateTime::currentDateTime()));
    }

    //QSLOG_DEBUG("final url = " + strFinalUrl);
    return retUrls;
    // strFinalUrl;
}

QStringList CPathBuilder::parseFromUrl(const QString &url, QDateTime in_oDt)
{
    // 最终的时间

    QDateTime finalDateTime = in_oDt;

    QStringList strNewUrl;
    QStringList strFinalUrl;
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

        //
        QList<QDateTime> oDateTimeLists;

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

            // valueRange = -[0,1]d
            QString midValue = valueRange.mid(1, valueRange.length() - 2);
            QStringList strTimeLists;

            // 出现多个时间增减量
            if (midValue.length() > 1 && midValue[0] == '[' && midValue[midValue.length() - 1] == ']')
            {
                strTimeLists = midValue.mid(1, midValue.length() - 2).split(",");
                qDebug() << strTimeLists;
            }
            else // 单一时间
            {
                strTimeLists.append(valueRange.mid(1, valueRange.length() - 2));
            }

            foreach (const QString &strTimePart , strTimeLists)
            {
                // int iTime = valueRange.mid(1, valueRange.length() - 2).toInt() * addorsub;
                int iTime = strTimePart.toInt() * addorsub;

                QDateTime tmpFinalDateTime = finalDateTime;
                int step = 0;
                if ('H' == lastc)
                {
                    tmpFinalDateTime = tmpFinalDateTime.addSecs(iTime * 3600);
                    //QSLOG_DEBUG(QString("%1 H").arg(iTime));
                }
                else if ('d' == lastc)
                {
                    tmpFinalDateTime = tmpFinalDateTime.addDays(iTime);
                    //QSLOG_DEBUG(QString("%1 d").arg(iTime));
                }
                else if ('M' == lastc)
                {
                    tmpFinalDateTime = tmpFinalDateTime.addSecs(iTime * 60);
                    //QSLOG_DEBUG(QString("%1 M").arg(iTime));
                }
                else if ('s' == lastc)
                {
                    tmpFinalDateTime = tmpFinalDateTime.addSecs(iTime);
                    //QSLOG_DEBUG(QString("%1 s").arg(iTime));
                }
                else
                {
                    //QSLOG_ERROR("check unsupport character after %T should be H / d / M / s");
                    return strNewUrl;
                }

                oDateTimeLists.append(tmpFinalDateTime);
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

        foreach(const QDateTime &tmpDt , oDateTimeLists)
        {
            strFinalUrl.append(tmpDt.toString(strDateTimeFormat) + "/");
        }


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

        strFinalUrl.append(finalDateTime.toString(strDateTimeFormat) + "/");
    }
    else
    {
        strFinalUrl.append(url + "/");
    }

    // return strFinalUrl + "/";
    return QStringList::fromSet(strFinalUrl.toSet());
    //return strFinalUrl;
}

QDateTime CPathBuilder::getDateTimeFrom2Urls(const QString &rule, const QString &path)
{
    QDateTime retDt = QDateTime::currentDateTime();

    QMap<QString, int> datetime;
    datetime.insert("yyyy", retDt.date().year());
    datetime.insert("MM", retDt.date().month());
    datetime.insert("dd", retDt.date().day());
    datetime.insert("hh", retDt.time().hour());
    datetime.insert("mm", retDt.time().minute());
    datetime.insert("ss", retDt.time().second());

    QStringList ruleParts = rule.split("/");
    QStringList pathParts = path.split("/");

    for (int i = 0; i < ruleParts.count(); ++i)
    {
        QString eachRule = ruleParts.at(i);
        int timeIndex = eachRule.indexOf("%t");
        if (-1 == timeIndex)
        {
            continue;
        }

        // 存在时间变量
        QString pathTime = pathParts.at(i);
        QString timeRule = eachRule.mid(timeIndex);
        QStringList tmpTimePart = timeRule.split("%t");
        int moveIndex = 0;
        foreach(QString eachTimePart, tmpTimePart)
        {
            // qDebug() << eachTimePart;
            if (eachTimePart == "%Y")
            {
                QString year = pathTime.mid(moveIndex, 4);
                moveIndex += 4;
                datetime["yyyy"] = year.toInt();

            }
            else if (eachTimePart == "%m")
            {
                QString month = pathTime.mid(moveIndex, 2);
                moveIndex += 2;
                datetime["MM"] = month.toInt();
            }
            else if (eachTimePart == "%d")
            {
                QString day = pathTime.mid(moveIndex, 2);
                moveIndex += 2;

                datetime["dd"] = day.toInt();
            }
            else if (eachTimePart == "%H")
            {
                QString hour = pathTime.mid(moveIndex, 2);
                moveIndex += 2;

                datetime["hh"] = hour.toInt();
            }
            else if (eachTimePart == "%M")
            {
                QString min = pathTime.mid(moveIndex, 2);
                moveIndex += 2;
                datetime["mm"] = min.toInt();
            }
            else if (eachTimePart == "%S")
            {
                QString sec = pathTime.mid(moveIndex, 2);
                moveIndex += 2;
                datetime["ss"] = sec.toInt();
            }
            else
            {

            }
        }
    }
    retDt = QDateTime(QDate(datetime["yyyy"], datetime["MM"], datetime["dd"]), QTime(datetime["hh"], datetime["mm"], datetime["ss"]));
    //qDebug() << retDt;
    return retDt;
}

QDateTime CPathBuilder::getDateTimeFromFileName(const QString &rule, const QString &filename)
{
    QDateTime oRetDt;
    QRegExp oReg(rule);
    if (-1 != oReg.indexIn(filename))
    {
        QString strDateTime = oReg.cap(1);
        switch (strDateTime.length())
        {
        case 4:
            oRetDt = QDateTime::fromString(strDateTime, "yyyy");
            break;
        case 6:
            oRetDt = QDateTime::fromString(strDateTime, "yyyyMM");
            break;
        case 8:
            oRetDt = QDateTime::fromString(strDateTime, "yyyyMMdd");
            break;
        case 10:
            oRetDt = QDateTime::fromString(strDateTime, "yyyyMMddhh");
            break;
        case 12:
            oRetDt = QDateTime::fromString(strDateTime, "yyyyMMddhhmm");
            break;
        case 14:
            oRetDt = QDateTime::fromString(strDateTime, "yyyyMMddhhmmss");
            break;
        default:
            break;
        }

        if (!oRetDt.isValid())
        {
            qDebug() << "DateTime is invalid";
        }
    }
    return oRetDt;
}

int CPathBuilder::setChildProcessEnv()
{
    QStringList env = QProcess::systemEnvironment();
    foreach (QString str, env)
    {
        //qDebug() << str;
        if (str.startsWith("PATH"))
        {
            if (-1 == str.indexOf(qApp->applicationDirPath()))
            {
                str.append(":").append(qApp->applicationDirPath());
                char *envbuffer = (char *)malloc(str.length()+1);
                if (envbuffer == NULL)
                {
                    //QSLOG_ERROR("malloc error");
                    return -1;
                }

                strcpy(envbuffer, str.toLocal8Bit().toStdString().data());
                //qDebug() << envbuffer;
                //QSLOG_DEBUG(envbuffer);
                if (0 != putenv(envbuffer))
                {
                    //QSLOG_ERROR("putenv errr");
                }
                else
                {
                    //QSLOG_DEBUG("set child env success");
                }
                qDebug() << getenv("PATH");
                // can not free, or putenv failure
                //free(envbuffer);
            }
        }
    }
    return 0;
}

QString CPathBuilder::getFinalPathFromUrlV2(const QString &path, const QString &filename)
{
    QStringList each_paths = path.split("/");
    for (int k = 0; k < each_paths.size(); ++k)
    {
        QString &partUrl = each_paths[k];
        //%On-m   - means the range n to m of characters from original
        //        file name.n = '^' - from the beginning
        //        m = '$' - to the end
        int index1 = partUrl.indexOf("%O");
        int index2 = partUrl.indexOf("-", index1);
        if (-1 != index1 && -1 != index2 && index1 < index2)
        {
            // 截取之间的内容
            int iStartIndex = partUrl.mid(index1 + 2, index2 - index1 - 2).toInt();
            int iMoveIndex = index2+1 ;
            for (; iMoveIndex < partUrl.length(); ++iMoveIndex)
            {
                if (partUrl.at(iMoveIndex) < '0' || partUrl.at(iMoveIndex) > '9')
                {
                    break;
                }
            }

            // 得到EndIndex位置
            int iEndIndex = partUrl.mid(index2 + 1, iMoveIndex - index2 - 1).toInt();

            // 根据startIndex以及endIndex，从原始文件名中截取内容
            if (iEndIndex <= filename.length())
            {
                QString strFinalUrl = filename.mid(iStartIndex, iEndIndex - iStartIndex);
                // 替换index1到iMoveIndex之间的内容
                partUrl.replace(index1, iMoveIndex - index1, strFinalUrl);
                //qDebug() << partUrl;
            }

        }
    }

    QString finalUrl;
    foreach(QString str, each_paths)
    {
        if (str.trimmed().isEmpty())
        {
            continue;
        }
        finalUrl.append(str).append("/");
    }
    if (path.startsWith("/"))
    {
        finalUrl.prepend("/");
    }
    return finalUrl;
}
