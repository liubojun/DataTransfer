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
#include "ICtkPluginManager.h"
#include "pathbuilder.h"
#include "change_name.h"
#include "ICollectManager.h"
#include "DataBase.h"
#include "CollectorBase.h"

// 单进程模式完成一次数据的收集、分发
// DataTransferPro 收集目录ID
int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    if (argc != 2)
    {
        QSLOG_ERROR("param num incorrect!");
        return -1;
    }

    QString strLockPath = app.applicationDirPath() + "/lock/";
    //QDir oLockDir(strLockPath);
    //if (!oLockDir.exists())
    //{
    //    oLockDir.mkpath(strLockPath);
    //}
    QLockFile oMem(strLockPath + argv[1]);
    oMem.lock();
    //if (!oMem.create(1))
    //{
    //    QSLOG_ERROR("create shared memory failure!");
    //    return false;
    //}

    CChangeName::init_rename_rules((app.applicationDirPath() + "/config/rename.rule").toStdString().c_str());

    QSLOG_INFO("START DATA TRANSFER PROGRAM");

    //用于支持中文路径
    QTextCodec *codec = QTextCodec::codecForLocale();
    QTextCodec::setCodecForLocale(codec);

    curl_global_init(CURL_GLOBAL_ALL);

    CPathBuilder oPathBuilder;

#ifndef _WIN32
    QPluginLoader loader(oPathBuilder.getPluginManagerPath() + "/libctkPluginManager.so");
#else
    QPluginLoader loader(oPathBuilder.getPluginManagerPath() + "/ctkPluginManager.dll");
    // QPluginLoader loader("F:\\新建文件夹\\bin\\plugins\\ctkPluginManager\\ctkPluginManager.dll");
#endif

    ICtkPluginManager *plugin = dynamic_cast<ICtkPluginManager *>(loader.instance());
    if (NULL != plugin)
    {
        plugin->init();
    }
    else
    {
        oMem.unlock();
        return -1;
    }
    QSLOG_INFO(QObject::tr("load all plugins successful"));


    QSharedPointer<ICollectManager> pCollect;
    QWaitCondition oCondition;
    QMutex oLocker;
    int iLogSize = 0;

    QObject *pObj = plugin->getService("IDataCollection");
    if (NULL == pObj)
    {
        oMem.unlock();
        return -1;
    }
    pCollect = (qobject_cast<IDataCollection*>(pObj))->createCollector(oCondition, oLocker, iLogSize);

    CollectTask oTask;
    oTask.dirID = " {60c62861-de8d-4872-85e4-255a807ff57b}";
    oTask.dirID = argv[1];
    DataBase::getInstance()->QueryCollectTask(oTask);

    // 启动收集
    //pCollect->addSyncTransfer((collection_type) oTask.collectType);

    CollectorBase *pCollectFather = pCollect->creatCollector((collection_type) oTask.collectType);
    if (NULL == pCollectFather)
    {
        oMem.unlock();
        return -1;
    }

    pCollectFather->getNewFiles(oTask);

    QCoreApplication::processEvents();

    delete pCollectFather;

    pCollectFather = NULL;

    pCollect.clear();

    plugin->uninit();

    loader.unload();	//只在最后一个应用实例unload有效.

    curl_global_cleanup();	// 清理libcurl资源

    //QSLOG_RELEASE
    oMem.unlock();
    return 0;
}