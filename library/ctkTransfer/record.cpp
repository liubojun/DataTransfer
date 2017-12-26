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
    //stream.setCodec("GB2312");
    QString strLine = stream.readLine();
    dir2filetable_t oTempDirTable;
    QString oTempDir;
    while (!stream.atEnd())
    {
        strLine = stream.readLine();
        QStringList strData = strLine.split(",");
        if (strLine.startsWith("dir="))
        {
            //保存当前dir,创建一个dir2filetable_t对象
            oTempDir = strData.at(0).mid(4);

            oTempDirTable.clear();
            //oTempDirTable.insert(strDir, recordFileInfo_t());
        }
        else if (strLine.startsWith("NNNN"))
        {
            // 创建一个dir2filetable_t对象
            m_oMemLastFileInfo.insert(oTempDir, oTempDirTable);
            oTempDirTable.clear();
            oTempDir.clear();
        }
        else if (strLine.startsWith(" "))
        {
            continue;
        }
        else
        {
            QString filename = strData.at(0);
            QString filetime = strData.at(1);
            int filesize = strData.at(2).toInt();
            oTempDirTable.insert(filename, recordFileInfo_t(filename, filetime, filesize));
        }


    }
}

bool CDirRecord::updateLatestFileSize(memfiletable_t &oFileSizeInfo)
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

    int iTotalFileCount = 0;
    for (memfiletable_t::const_iterator iter = oFileSizeInfo.begin(); iter != oFileSizeInfo.end(); ++iter)
    {
        iTotalFileCount += iter->count();
    }

    QTextStream stream(&file);
    //stream.setCodec("GB2312");
    stream << "totaldircount=" << oFileSizeInfo.size() << ","
           << "totalfilecount=" << iTotalFileCount << ","
           << "lastprocesstime=" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:ss:mm")
           << NEWLINEFLAG;

    //stream << "format:filepath,lastmodifiedtime,filesize"
    //       << NEWLINEFLAG;

    for (memfiletable_t::const_iterator iter = oFileSizeInfo.begin(); iter != oFileSizeInfo.end(); ++iter)
    {
        stream << "dir=" << iter.key() << "," << "filecount=" << iter->count() << NEWLINEFLAG;
        for (dir2filetable_t::const_iterator subiter = iter->begin(); subiter != iter->end(); ++subiter)
        {
            stream << subiter->m_strFileName << "," << subiter->m_strFileDt << "," << subiter->m_iFileSize << NEWLINEFLAG;
        }
        stream << "NNNN" << NEWLINEFLAG;

    }
    return true;
}

void CDirRecord::updateLatestFileSize(const QString &dir, const QString &filename, const QString &dt, int ifilesize)
{
    memfiletable_t::iterator iter = m_oMemThisFileInfo.find(dir);
    if (iter != m_oMemThisFileInfo.end())
    {
        dir2filetable_t::iterator subiter = iter->find(filename);
        if (subiter == iter->end())
        {
            recordFileInfo_t fi;
            iter->insert(filename, recordFileInfo_t(filename, dt, ifilesize));
        }
    }
    else
    {
        dir2filetable_t dir2table;
        dir2table.insert(filename, recordFileInfo_t(filename, dt, ifilesize));
        m_oMemThisFileInfo.insert(dir, dir2table);
    }
}

bool CDirRecord::checkIsNewFile(const QString &dir, const QString &fn, const QString &dt, int size)
{
    memfiletable_t::iterator iter = m_oMemLastFileInfo.find(dir);
    if (iter != m_oMemLastFileInfo.end())
    {
        dir2filetable_t::iterator subiter = iter->find(fn);
        if (subiter != iter->end())
        {
            // 查找其他属性
            if (dt != subiter->m_strFileDt)
            {
                return true;
            }

            if (size != subiter->m_iFileSize)
            {
                return true;
            }

            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return true;
    }
    //QMap<QString, recordFileInfo_t>::iterator iter = m_oMemLastFileInfo.find(url);
    //if (iter != m_oMemLastFileInfo.end())
    //{
    //    // 查找其他属性
    //    if (dt != iter->m_strFileDt)
    //    {
    //        return true;
    //    }

    //    if (size != iter->m_iFileSize)
    //    {
    //        return true;
    //    }

    //    return false;
    //}

    //return true;
}

void CDirRecord::reflush()
{
    updateLatestFileSize(m_oMemThisFileInfo);
}
