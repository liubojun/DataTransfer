#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QThread>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QSet>

class FileWatcher : public QObject
{
    Q_OBJECT
public:
    explicit FileWatcher(QObject *parent = 0);
    virtual ~FileWatcher();

signals:
    void checkDir(QSet<QString>);
public slots:
    void dirChanged(QString path);
    void onTimer();

public:
    // 添加文件夹监控
    bool addDirWatch(const QString &strRoot, bool bSub=true);
    // 删除文件夹监控
    void removeDirWatch(const QString &strRoot, bool bSub=true);

private:
    QStringList listAllDirs(const QString &strPath);
    void addDirToWatch(QString strPath);
    void addDirsToWatch(QStringList strPaths);

    QThread m_oThread;

    QFileSystemWatcher m_fileWatch;
    QSet<QString> m_watchDirs;          // 已监控的目录

    QSet<QString> m_uncheckDirs;        // 待检查的目录

    QTimer m_oTimer;
};

#endif // FILEWATCHER_H
