#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
#include <Windows.h>
#endif

#ifdef _WIN32
#include <windows.h>
#else // linux stuff
#include <sys/vfs.h>
#include <sys/stat.h>
#endif // _WIN32

#include "QsLog/ctkLog.h"
#include "DelFiles.h"
#include <QDir>
#include <QDateTime>
#include "qfileinfo.h"
#include <QFileInfoList>
#ifdef _WIN32
#include "Windows.h"
#endif
#include <QtGlobal>
#include <QMap>
#include <QFileInfo>

bool olderThen( const QString &str1,const QString &str2 )
{
    QFileInfo f1(str1);
    QFileInfo f2(str2);
    return f1.lastModified() < f2.lastModified();
}
//DelFiles::DelFiles(const QString &fullPath,const QString &style,const QString &freeSpace,const int &timeLine,const QString &regex)
//    :m_strFullPath(fullPath),m_freeSpace(freeSpace.toDouble()),m_style(style),m_match(regex),m_time(timeLine)
//{
//    reg.setPatternSyntax(QRegExp::RegExp);
//}

// DelFiles::DelFiles(QList<BaseDatas> datas):m_datas(datas)
// {
// 	reg.setPatternSyntax(QRegExp::RegExp);
// }

//DelFiles::DelFiles(const QString &fullPath,const QString &style,const QString &freeSpace,const int &timeLine,const QStringList &regexs)
//{
//    m_strFullPath = fullPath;
//    m_style = style;
//    m_freeSpace = freeSpace.toDouble();
//    m_time = timeLine;
//    m_matches = regexs;
//}

DelFiles::DelFiles(BaseDatas &in_oData) : m_oData(in_oData)
{

}

DelFiles::~DelFiles()
{

}

bool DelFiles::delFiles()
{
    //QSLOG_INFO(QString("Clear data from %1 in the way of %2").arg(m_strFullPath).arg(m_style));
    return delByTime();

    //bool flag = true;

    //if(m_style == "0")
    //{
    //    //m_oLogSender.sendClearMsg(m_oData.m_taskName, "", QString::fromLocal8Bit("开始目录[%1]清理").arg(CPathBuilder::getFtpUrl(m_oData.ip, m_oData.port, m_oData.m_fullPath)), 1);
    //    flag = delByTime();
    //}
    //else if(m_style == "1")
    //{
    //    flag = delBySize();
    //}
    //else
    //{
    //    flag = (delByTime()&& delBySize());
    //}

    //return flag;
}

bool DelFiles::delByTime()
{
    if (!QDir(m_oData.m_fullPath).exists())
    {
        return false;
    }

    QDir dir(m_oData.m_fullPath);
    QFileInfoList all = dir.entryInfoList();
    // QDateTime m_DateTime = QDateTime::currentDateTime().addDays(-m_time.toInt());
    QDateTime m_DateTime = QDateTime::currentDateTime().addMSecs(-qint64(m_oData.m_time) * 1000);

    foreach (const QFileInfo &f, all)
    {
        if (f.fileName() == "." || f.fileName() == "..")
        {
            continue;
        }
        if (f.isDir())
        {
            delByTime(f.absoluteFilePath());
        }
        else
        {
            if (f.lastModified() <= m_DateTime)
            {
                //qDebug() << f.absoluteFilePath() << f.lastModified() << m_DateTime;
                reg.setPattern(m_oData.m_regex.at(0));
                if (reg.exactMatch(f.absoluteFilePath()))
                {
                    if (QFile::remove(f.absoluteFilePath()))
                    {
                        m_oLogSender.sendClearMsg(m_oData.m_taskName, "", QString::fromLocal8Bit("删除文件[%1]成功").arg(f.fileName()), 1);
                    }
                }
                //foreach (const QString str, m_matches)
                //{
                //    reg.setPattern(str);
                //    if (reg.exactMatch(f.absoluteFilePath()))
                //    {
                //        QFile::remove(f.absoluteFilePath());
                //    }
                //}
            }
        }
    }


    return true;
}

bool DelFiles::delBySize()
{
    //if (!QDir(m_strFullPath).exists())
    //{
    //    return false;
    //}

    //loaPlugins(m_strFullPath, m_fileList);	//获取目录下的所有文件

    //if(!m_fileList.isEmpty() && m_fileList.count()>0)
    //{
    //    //按照最后修改时间进行排序(从最旧至最新)
    //    m_fileList.sort();
    //    qSort(m_fileList.begin(),m_fileList.end(),::olderThen);
    //    //当剩余空间满足前进行顺序删除
    //    double freeSpace;
    //    getFreeTotalSpace(m_strFullPath, freeSpace);
    //    int i = 0;
    //    while(freeSpace <= m_freeSpace && i<m_fileList.count())
    //    {
    //        foreach (const QString str, m_matches)
    //        {
    //            reg.setPattern(str);
    //            if (reg.exactMatch(m_fileList[i]))
    //            {
    //                QFile::remove(m_fileList[i]);
    //            }
    //        }
    //        i++;
    //    }
    //}
    return true;
}

bool DelFiles::delByTime(const QString &fullpath)
{
    //m_strFullPath = fullpath;
    if (!QDir(fullpath).exists())
    {
        return false;
    }
    QDir dir(fullpath);
    QFileInfoList all = dir.entryInfoList();
    QDateTime m_DateTime = QDateTime::currentDateTime().addMSecs(-(qint64(m_oData.m_time) * 1000));
    foreach (const QFileInfo &f, all)
    {
        if (f.fileName() == "." || f.fileName() == "..")
        {
            continue;
        }
        if (f.isDir())
        {
            delByTime(f.absoluteFilePath());
        }
        else
        {
            if (f.lastModified()<=m_DateTime)
            {
                reg.setPattern(m_oData.m_regex.at(0));
                if (reg.exactMatch(f.absoluteFilePath()))
                {
                    if (QFile::remove(f.absoluteFilePath()))
                    {
                        m_oLogSender.sendClearMsg(m_oData.m_taskName, "", QString::fromLocal8Bit("删除文件[%1]成功").arg(f.fileName()), 1);
                    }
                }
                //foreach (const QString &str, m_matches)
                //{
                //    reg.setPattern(str);
                //    if (reg.exactMatch(f.absoluteFilePath()))
                //    {
                //        QFile::remove(f.absoluteFilePath());
                //    }
                //}
                //QFile::remove(f.absoluteFilePath());
            }
        }
    }

    dir.rmdir(fullpath);

    return true;
}


bool DelFiles::delBySize( const QString &fullpath )
{
    //m_strFullPath = fullpath;
    //if (!QDir(m_strFullPath).exists())
    //{
    //    return false;
    //}
    //loaPlugins(m_strFullPath,m_fileList);
    //if(!m_fileList.isEmpty()&&m_fileList.count()>0)
    //{
    //    //按照最后修改时间进行排序(从最旧至最新)
    //    m_fileList.sort();
    //    qSort(m_fileList.begin(),m_fileList.end(),::olderThen);
    //    //当剩余空间满足前进行顺序删除
    //    double freeSpace;
    //    getFreeTotalSpace(m_strFullPath,freeSpace);
    //    int i = 0;
    //    while(freeSpace <= m_freeSpace && i<m_fileList.count())
    //    {
    //        QString STR = m_fileList[i];
    //        QFile::remove(m_fileList[i]);
    //        i++;
    //    }
    //}

    return true;
}

QStringList DelFiles::loaPlugins( const QString &plugindir, QStringList &plugins)
{
    QDir dir(plugindir);
    QFileInfoList all = dir.entryInfoList();
    foreach (const QFileInfo &f, all)
    {
        if (f.fileName() == "." || f.fileName() == "..")
        {
            continue;
        }
        if (f.isDir())
        {
            loaPlugins(f.absoluteFilePath(), plugins);
        }
        else
        {
            plugins.append(f.absoluteFilePath());
        }
    }
    return plugins;
}

double DelFiles::getDiskFreeSpace( const QString& sDirPath )
{
#ifdef _WIN32
    LPCTSTR lpcwstrDriver=(LPCTSTR)sDirPath.utf16();

    ULARGE_INTEGER liFreeBytesAvailable;
    ULARGE_INTEGER liTotalBytesAvailabel;
    ULARGE_INTEGER liTotalFreeBytes;
    if(!GetDiskFreeSpaceEx(lpcwstrDriver,&liFreeBytesAvailable,&liTotalBytesAvailabel,&liTotalFreeBytes))
    {
        return 0;
    }
    return (double)liTotalFreeBytes.QuadPart/1024/1024/1024;
#else
    return 0;
#endif


}

bool DelFiles::getFreeTotalSpace( const QString& sDirPath, double& fFree )
{
    fFree = 20000;
    float fKB = 1024 * 1024;
#ifdef _WIN32
    QString sCurDir = QDir::current().absolutePath ();
    QDir::setCurrent( sDirPath );

    ULARGE_INTEGER free,total;
    bool bRes = ::GetDiskFreeSpaceExA( 0, &free, &total, NULL );
    if ( !bRes ) return false;

    QDir::setCurrent( sCurDir );

    fFree = static_cast<double>( static_cast<__int64>(free.QuadPart) ) / fKB;
    /*fTotal = static_cast<double>( static_cast<__int64>(total.QuadPart) ) / fKB;*/

#else //linux

    struct stat stst;
    struct statfs stfs;

    if ( ::stat(sDirPath.toLocal8Bit(),&stst) == -1 ) return false;
    if ( ::statfs(sDirPath.toLocal8Bit(),&stfs) == -1 ) return false;

    fFree = stfs.f_bavail * ( stst.st_blksize / fKB );
    /*fTotal = stfs.f_blocks * ( stst.st_blksize / fKB );*/

#endif // _WIN32
    return true;
}
