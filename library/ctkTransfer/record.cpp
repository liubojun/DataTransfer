#include "record.h"
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDateTime>
#include <iostream>
#include "ctkLog.h"

#define  NEWLINEFLAG "\n"
CDirRecord::CDirRecord(const QString &dirName)
{
    QString strDir = qApp->applicationDirPath() + "/work/record/" + dirName;
    QDir odir(strDir);
    if (!odir.exists(strDir))
    {
        if (!odir.mkpath(strDir))
        {
            QSLOG_ERROR(QString("mkpath %1 failure").arg(strDir));
        }
    }

    m_strUrl = strDir + "/record.txt";

}


void CDirRecord::loadLatestFileSize()
{
    //QString strDBPath = qApp->applicationDirPath() + "/work/record/" + "m_collectSet.dirID" + "/record.txt";
    // 文件格式说明
    // 首行为文件数
    // 从第二行开始，按照：文件全路径,文件大小 的方式进行存储
    QFile file(m_strUrl);

    if (!file.exists())
    {
        return;
    }

    if (!file.open(QIODevice::ReadOnly))
    {
        QSLOG_ERROR("open file failure:" + file.errorString());
        return;
    }

    QTextStream stream(&file);
    QString strLine = stream.readLine();
    stream.readLine();
    //QString strContent = stream.readAll();
    //int startIndex = 0;
    //int endIndex = 0;
    while (!stream.atEnd())
    {
        strLine = stream.readLine();
        QStringList strData = strLine.split(",");
        QString filename = strData.at(0);
        QString filetime = strData.at(1);
        int filesize = strData.at(2).toInt();
        m_oMemLastFileInfo.insert(filename, recordFileInfo_t(filename, filetime, filesize));
    }
    //while (-1 != (endIndex = strContent.indexOf(NEWLINEFLAG, startIndex)))
    //{
    //    QString strLine = strContent.mid(startIndex, endIndex).trimmed();
    //    QStringList strData = strLine.split(",");
    //    QString filename = strData.at(0);
    //    QString filetime = strData.at(1);
    //    int filesize = strData.at(2).toInt();
    //    m_oMemLastFileInfo.insert(filename, recordFileInfo_t(filename, filetime, filesize));
    //    startIndex = endIndex+1;
    //}

}

bool CDirRecord::updateLatestFileSize(const QMap<QString, recordFileInfo_t> &oFileSizeInfo)
{
    // 文件格式说明
    // 首行为文件数
    // 从第二行开始，按照：文件全路径,文件大小 的方式进行存储
    QFile file(m_strUrl);
    if (!file.open(QIODevice::WriteOnly))
    {
        QSLOG_ERROR("open file failure:" + file.errorString());
        return false;
    }

    QTextStream stream(&file);
    stream << oFileSizeInfo.size() << ","
           << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:ss:mm")
           << NEWLINEFLAG;
    stream << "format:filepath,lastmodifiedtime,filesize"
           << NEWLINEFLAG;

    for (QMap<QString, recordFileInfo_t>::const_iterator iter = oFileSizeInfo.begin(); iter != oFileSizeInfo.end(); ++iter)
    {
        stream << iter.key() << "," << iter.value().m_strFileDt << "," << iter.value().m_iFileSize << NEWLINEFLAG;
    }
    return true;
}

void CDirRecord::updateLatestFileSize(const QString &filename, const QString &dt, int ifilesize)
{
    m_oMemThisFileInfo.insert(filename, recordFileInfo_t(filename, dt, ifilesize));
}

bool CDirRecord::checkIsNewFile(const QString &url, const QString &dt, int size)
{
    QMap<QString, recordFileInfo_t>::iterator iter = m_oMemLastFileInfo.find(url);
    if (iter != m_oMemLastFileInfo.end())
    {
        // 查找其他属性
        if (dt != iter->m_strFileDt)
        {
            return true;
        }

        if (size != iter->m_iFileSize)
        {
            return true;
        }

        return false;
    }

    return true;
}

void CDirRecord::reflush()
{
    updateLatestFileSize(m_oMemThisFileInfo);
}
