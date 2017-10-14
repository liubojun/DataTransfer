#include "filewatcher.h"
#include <QDir>

FileWatcher::FileWatcher(QObject *parent) :
    QObject(parent)
{
    m_oThread.start();
    this->moveToThread(&m_oThread);

    m_oTimer.start(1000);
    connect(&m_oTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
    connect(&m_fileWatch, SIGNAL(directoryChanged(QString)), this, SLOT(dirChanged(QString)));
}

FileWatcher::~FileWatcher()
{
}

bool FileWatcher::addDirWatch(const QString &strRoot, bool bSub)
{
    QDir qdir(strRoot);
    if (!qdir.exists())
    {
        return false;
    }

    QStringList allDirs = listAllDirs(strRoot);
    addDirsToWatch(allDirs);

    return true;
}

void FileWatcher::removeDirWatch(const QString &strRoot, bool bSub/*=true*/)
{
    QStringList allDirs = listAllDirs(strRoot);
    foreach (QString strPath, allDirs)
    {
        m_watchDirs.remove(strPath);
        m_fileWatch.removePath(strPath);
    }
}

void FileWatcher::dirChanged(QString path)
{
    // 及时监控子文件夹
    QDir qdir(path);
    QStringList subPath = qdir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    QString strFullPath;
    for (int i=0; i<subPath.size(); ++i)
    {
        strFullPath = path + "/" + subPath.at(i);
        addDirToWatch(strFullPath);
    }

    m_uncheckDirs.insert(path);
}

QStringList FileWatcher::listAllDirs(const QString &strPath)
{
    QStringList lstResult;
    lstResult.append(strPath);

    QList<QString> leftDir;
    leftDir.push_back(strPath);

    while (leftDir.size() > 0)
    {
        QString curPath = leftDir.first();
        leftDir.pop_front();
        QDir qdir(curPath);
        QStringList subPath = qdir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
        for (int i=0; i<subPath.size(); ++i)
        {
            QString strFullPath = curPath + "/" + subPath.at(i);
            lstResult.append(strFullPath);
            leftDir.append(strFullPath);
        }
    }

    return lstResult;
}

void FileWatcher::addDirToWatch(QString strPath)
{
    if (m_watchDirs.contains(strPath))
    {
        return;
    }

    m_fileWatch.addPath(strPath);
    m_watchDirs.insert(strPath);
}

void FileWatcher::addDirsToWatch(QStringList strPaths)
{
    for (int i=0; i<strPaths.size(); ++i)
    {
        addDirToWatch(strPaths.at(i));
    }
}

void FileWatcher::onTimer()
{
    if (!m_uncheckDirs.empty())
    {
        emit checkDir(m_uncheckDirs);
        m_uncheckDirs.clear();
    }
}
