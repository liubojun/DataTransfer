/*************************************************************************
* Copyright(c) 2016,NRIET
* All Rights Reserved.
* @file		:	pluginmanagerImp.h
* @brief	:	插件管理器接口头文件声明
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

#ifndef pluginmanagerImp_h__
#define pluginmanagerImp_h__

#include <QtPlugin>
#include <QObject>
#include <QSharedPointer>
#include <QStringList>
#include <ctkPluginFrameworkFactory.h>
#include <ctkPluginFramework.h>
#include <QVector>
#include <QString>
#include <QMap>
#include "ICtkPluginManager.h"
//#include "IRadarProcess.h"
#include "pluginBaseInfo.h"

class CPluginManagerImp : public ICtkPluginManager
{
    Q_OBJECT
    // 此处为什么使用interface不能编译成功，有待研究
    // Q_INTERFACES(ICtkPluginManager)

#ifdef WITH_QT5
    Q_PLUGIN_METADATA(IID "library.ctkPluginManager")
#endif

public:
    CPluginManagerImp();

    virtual ~CPluginManagerImp();

    /**
     * @brief  初始化所有插件
     * @return void：无
     */
    virtual void init();

    /**
    * @brief  初始化插件GUI界面
    * @return void：描述返回值
    */
    virtual void initGui(UIInterface *guiInterface);

    virtual void uninit();

    virtual ctkPluginFrameworkFactory *getCtkPluginFrameworkFactory();

    virtual ctkPluginContext *getPluginContext();

    virtual ICtkPluginManager* instance();

    virtual QObject *getService(const QString &interfaceName);

    virtual QVector<pluginInfo*>& getLoadedPluginBaseInfo();

    virtual void SetMsgLogObj(LogInterface* pMsgLog, LOGWIDGETTYPE logWT);

    virtual LogInterface* GetMsgLogObj( LOGWIDGETTYPE logWT );

    virtual void SetTransferObj(IFileTransfer* pTrans, TRANSFERSYSTEMTYPE TranOT);

    virtual IFileTransfer* GetTransferObj(TRANSFERSYSTEMTYPE TranOT);

protected:
    bool isLoadedInterface(const QString& sIName);
    void EraseUnLoadInterface(QSharedPointer<ctkPlugin> plugin);
    QStringList loaPlugins(const QString &plugindir, QStringList &plugins);

    /**
     * @brief  移除插件数据库路径
     * @return void：无
     */
    static void removePluginsDb();

private:
    QSharedPointer<ctkPluginFrameworkFactory> m_pfwFactory;

    QList<QSharedPointer<ctkPlugin> >m_Plugins;

    QVector<QString> m_loadedPlugins;

    QMap<LOGWIDGETTYPE, LogInterface*> m_logWidgetObjs;

    QMap<TRANSFERSYSTEMTYPE, IFileTransfer*> m_TransferObjs;

    QVector<pluginInfo*> m_arrayPluginBaseInfo;

};
#endif // pluginmanagerImp_h__
