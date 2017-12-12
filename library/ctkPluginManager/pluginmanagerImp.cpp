/*************************************************************************
* Copyright(c) 2016,NRIET
* All Rights Reserved.
* @file		:	pluginmanagerImp.cpp
* @brief	:	插件管理器接口实现源文件
* @version	:	V1.0
* @author	:	NRIET
* @date		:	2016-03-09
---------------------------------------------------------------------------
* @modify	:	修改说明
* @brief	:	修改内容
* @author	:	修改者
* @date		:	修改日期
* @version	:	替换版本
**************************************************************************/

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <ctkPluginFrameworkLauncher.h>
#include <ctkPluginFrameworkFactory.h>
#include <ctkPluginFramework.h>
#include <ctkPluginException.h>
#include <ctkPluginContext.h>
#include <QPluginLoader>
#include <QDir>
#include <QFileInfoList>
#include <ctkLog.h>
#include <exception>

#include "pluginmanagerImp.h"
#include "IBaseInterface.h"
#include "pathbuilder.h"
#include "LogInterface.h"
//#include "IServerMessage.h"
#include "IFileTransfer.h"
//#include "IRadarMosaic.h"

//是否开启内存泄露检查，该宏定义在cmake中
#ifdef WITH_MEM_CHECK
#include "vld.h"
#endif

CPluginManagerImp::CPluginManagerImp():m_pfwFactory( NULL )
{
    m_logWidgetObjs.clear();
    m_TransferObjs.clear();
    m_arrayPluginBaseInfo.clear();
}

CPluginManagerImp::~CPluginManagerImp() {}

ctkPluginFrameworkFactory * CPluginManagerImp::getCtkPluginFrameworkFactory()
{
    if (NULL == m_pfwFactory)
    {
        ctkProperties prop;
        prop.insert("ctk.configuration.area", QApplication::applicationDirPath() + "/configuration");
        m_pfwFactory = QSharedPointer<ctkPluginFrameworkFactory>(new ctkPluginFrameworkFactory(prop));
    }
    return &(*m_pfwFactory);
}

ctkPluginContext * CPluginManagerImp::getPluginContext()
{
    return getCtkPluginFrameworkFactory()->getFramework()->getPluginContext();
}

ICtkPluginManager* CPluginManagerImp::instance()
{
    return this;
}

void CPluginManagerImp::init()
{
    // 判断plugins.db中的内容是否正确
    QString strDbUrl = qApp->applicationDirPath() + "/configuration/plugins.db";
    if (QFileInfo(strDbUrl).exists())
    {
        QSqlDatabase oSqlDb = QSqlDatabase::addDatabase("QSQLITE");		// 添加数据库驱动
        oSqlDb.setDatabaseName(strDbUrl);
        if (oSqlDb.open())
        {
            QSqlQuery oQuery(oSqlDb);
            if (!oQuery.exec("SELECT LocalPath FROM Plugins"))
            {
                QSLOG_ERROR(QString::fromLocal8Bit("查询插件数据库失败."));
                oSqlDb.close();
                removePluginsDb();
            }
            else
            {
                while (oQuery.next())
                {
                    QString strPluginsUrl = oQuery.value(0).toString();
                    if (!QFileInfo(strPluginsUrl).exists())
                    {
                        oSqlDb.close();
                        removePluginsDb();
                    }
                    break;
                }
            }
        }
        else
        {
            removePluginsDb();
        }

    }

    ///////////////////////////////插件管理start///////////////////////////

    ctkPluginFrameworkFactory * fwFactory = getCtkPluginFrameworkFactory();
    QSharedPointer<ctkPluginFramework> framework = fwFactory->getFramework();
    ctkPluginFramework *pf = &(*framework);

    try
    {
        framework->init();
    }
    catch (const ctkPluginException& exc)
    {
        QSLOG_ERROR(QString(tr("Failed to initialize the plug-in framework:")) + exc.message());
        exit(1);
    }

    try
    {
        CPathBuilder oPathBuilder;
        QString strPluginPath = oPathBuilder.getPluginsPath();

        // 开始遍历插件目录下所有的插件，并加载，使用递归遍历
        QStringList pluginsList;
        loaPlugins(strPluginPath, pluginsList);

        QSLOG_INFO(tr("Begin loading plugins...") + strPluginPath);

        foreach (const QString &p, pluginsList)
        {
            QSLOG_INFO(tr("LOAD:") + p);

            //by weikangc modify 2016-05-18.
            QSharedPointer<ctkPlugin> plugin;

            try
            {
                plugin = framework->getPluginContext()->installPlugin(QUrl::fromLocalFile(p));
                plugin->start(ctkPlugin::START_TRANSIENT);

            }
            catch(const ctkPluginException& e)
            {
                QSLOG_ERROR(QString("init Plugin [%1] exception, reason : [%2]").arg(p).arg(e.what()));
                continue;
            }

            m_Plugins.append(plugin);


            QHash<QString, QString> hs = plugin->getHeaders();
            QHash<QString, QString>::iterator iter = hs.find("Export-Interface");

            if (iter != hs.end())
            {
                QStringList strExportInterfaces = iter->split(" ");
                foreach (const QString &strInterface, strExportInterfaces)
                {
                    if (strInterface.trimmed().isEmpty())
                    {
                        continue;
                    }
                    QSLOG_INFO(tr("plugin ") + p + tr(" has interface:") + strInterface);

                    // 此处有可能一个插件提供多个服务接口
                    QStringList strInterfaceList = strInterface.split(",");

                    bool bContinue = false;

                    foreach (const QString &interf, strInterfaceList)
                    {
                        ctkPluginContext *pluginsContext = getPluginContext();
                        ctkServiceReference ref = pluginsContext->getServiceReference(interf);
                        QObject *obj = pluginsContext->getService(ref);
                        IBaseInterface * baseInterface = (qobject_cast<IBaseInterface *> (obj));

                        if (baseInterface)
                        {
                            baseInterface->init(instance());

                            /*add by weikangc 2016-05-18*/
                            //pluginInfo *pPI = baseInterface->loadPluginBaseInfo();
                            //if ( pPI != NULL ) m_arrayPluginBaseInfo.push_back(pPI);
#if 0
                            if( !pPI->bLoad )
                            {
                                plugin->stop();
                                plugin->uninstall();
                                QPluginLoader loader(plugin->getLocation());
                                if (loader.isLoaded())
                                {
                                    loader.unload();
                                }
                                loader.unload();
                                bContinue = true;
                                m_Plugins.removeLast();
                                framework->getPluginContext()->getPlugins().removeLast();
                                break;
                            }
#endif
                        }
                        else
                        {
                            QSLOG_ERROR(tr("Resolve interface:") + strInterface + tr("failure"));
                        }

                        /*add by weikangc 2016-05-18*/
                        m_loadedPlugins.push_back(interf);
                    }

                    if( bContinue ) break;
                }
            }
            else
            {
                QSLOG_ERROR((tr("plugin") + p + tr("has no Export-Interface in manifest_headers.cmake")));
            }
        }
    }
    catch (const ctkPluginException& e)
    {
        QSLOG_ERROR(tr("init ctkPluginFramework exception, reason :") + e.what());
    }

    framework->start();
    ////////////////////////////////////////////////插件管理end///////////////////////////////////////////////
}

/**
 * @brief  卸载所有插件
 * @return void
 */
void CPluginManagerImp::uninit()
{
    foreach (QSharedPointer<ctkPlugin> plugin, m_Plugins)
    {
        QString str(plugin->getLocation().mid(8));
        qDebug() << "stop" << str;
        EraseUnLoadInterface(plugin);
        plugin->stop();
        plugin->uninstall();
        qDebug() << "uninstall" << str;
        QPluginLoader loader(plugin->getLocation());
        if (loader.isLoaded())
        {
            loader.unload();
        }
        loader.unload();

        qDebug() << "finish to uninit" << str;
    }
    m_Plugins.clear();
}

QStringList CPluginManagerImp::loaPlugins( const QString &plugindir, QStringList &plugins)
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
            if (f.fileName().startsWith("liborg") && (f.fileName().endsWith(".dll") || f.fileName().endsWith(".so")))
            {
                plugins.append(f.absoluteFilePath());
            }
        }
    }
    return plugins;
}

QObject * CPluginManagerImp::getService( const QString &interfaceName )
{
    QObject *obj = NULL;

    if ( isLoadedInterface(interfaceName) )
    {
        try
        {
            ctkPluginContext *pluginsContext = getPluginContext();
            ctkServiceReference ref = pluginsContext->getServiceReference(interfaceName);
            obj = pluginsContext->getService(ref);
        }
        catch (const ctkPluginException& e)
        {
            QSLOG_ERROR(tr("get Service:") + interfaceName + tr("from ctk pluginFramework exception, reason :") + e.what());
            //throw std::exception(interfaceName.toStdString().c_str());
            return NULL;
        }
    }

    return obj;

}

bool CPluginManagerImp::isLoadedInterface(const QString& sIName)
{
    bool bRet = false;

    for ( QVector<QString>::Iterator it = m_loadedPlugins.begin();
            it < m_loadedPlugins.end(); ++it)
    {
        if ( sIName == *it )
        {
            bRet = true;
            break;
        }
    }

    return bRet;
}

void CPluginManagerImp::SetMsgLogObj(LogInterface* pMsgLog, LOGWIDGETTYPE logWT)
{
    m_logWidgetObjs[logWT] = pMsgLog;
}

LogInterface* CPluginManagerImp::GetMsgLogObj(LOGWIDGETTYPE logWT)
{
    if ( m_logWidgetObjs.count(logWT) == 0 )
    {
        return NULL;
    }

    return m_logWidgetObjs[logWT];
}

void CPluginManagerImp::SetTransferObj(IFileTransfer* pTrans, TRANSFERSYSTEMTYPE TranOT)
{
    m_TransferObjs[TranOT] = pTrans;
}

IFileTransfer* CPluginManagerImp::GetTransferObj(TRANSFERSYSTEMTYPE TranOT)
{
    if ( m_TransferObjs.count(TranOT) == 0 )
    {
        return NULL;
    }

    return m_TransferObjs[TranOT];
}

void CPluginManagerImp::EraseUnLoadInterface(QSharedPointer<ctkPlugin> plugin)
{
    QHash<QString, QString> hs = plugin->getHeaders();
    QHash<QString, QString>::iterator iter = hs.find("Export-Interface");

    if (iter != hs.end())
    {
        QStringList strExportInterfaces = iter->split(" ");
        foreach (const QString &strInterface, strExportInterfaces)
        {
            QStringList strInterfaceList = strInterface.split(",");

            foreach (const QString &interf, strInterfaceList)
            {
                for ( QVector<QString>::Iterator it = m_loadedPlugins.begin();
                        it < m_loadedPlugins.end(); ++it)
                {
                    if ( interf == *it )
                    {
                        m_loadedPlugins.erase(it);
                        break;
                    }
                }
            }
        }
    }
}

void CPluginManagerImp::removePluginsDb()
{
    QDir dir(qApp->applicationDirPath() + "/configuration");

#ifndef WITH_QT5
    if (dir.rmpath(qApp->applicationDirPath() + "/configuration"))
    {
        QSLOG_ERROR(QString::fromLocal8Bit("移除数据库缓存目录失败."));
    }
#else
    if (!dir.removeRecursively())
    {
        QSLOG_ERROR(QString::fromLocal8Bit("移除数据库缓存目录失败."));
    }
#endif
}

QVector<pluginInfo*>& CPluginManagerImp::getLoadedPluginBaseInfo()
{
    return m_arrayPluginBaseInfo;
}

// Qt5之后已经移除了此功能
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(library.ctkPluginManager, CPluginManagerImp)
#endif

