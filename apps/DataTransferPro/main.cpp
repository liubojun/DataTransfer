#include "curl/curl.h"
#include <QApplication>
#include <QDebug>
#include <QTextCodec>
#include <QFileInfo>
#include <QDateTime>
#include <QPluginLoader>
#include <QTranslator>
#include <QProcess>
#include <QDir>
#include <QSharedMemory>
#include <QLockFile>
#include "ctkLog.h"
#include "pathbuilder.h"
#include "change_name.h"
#include "DataBase.h"
#include "CollectorBase.h"
#include "ftpCollector.h"

// 单进程模式完成一次数据的收集、分发
// DataTransferPro 收集目录ID
int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    //if (argc != 2)
    //{
    //    QSLOG_ERROR("param num incorrect!");
    //    return -1;
    //}

    QString strLockPath = app.applicationDirPath() + "/lock/";

    QLockFile oMem(strLockPath + argv[1]);
    oMem.lock();

    CChangeName::init_rename_rules((app.applicationDirPath() + "/config/rename.rule").toStdString().c_str());

    QSLOG_INFO("START DATA TRANSFER PROGRAM");

    //用于支持中文路径
    QTextCodec *codec = QTextCodec::codecForLocale();
    QTextCodec::setCodecForLocale(codec);

    CollectorBase::init();

    QWaitCondition oCnd;
    QMutex oLocker;
    int iLogSize = 0;

    CollectTask oTask;
    //oTask.dirID = " {60c62861-de8d-4872-85e4-255a807ff57b}";
    oTask.dirID = argv[1];
    DataBase::getInstance()->QueryCollectTask(oTask);

    CollectorBase *pCollect = new FtpCollector(oCnd, oLocker, iLogSize);

    pCollect->getNewFiles(oTask);

    QCoreApplication::processEvents();

    delete pCollect;

    ////QSLOG_RELEASE
    //oMem.unlock();
    return 0;
}