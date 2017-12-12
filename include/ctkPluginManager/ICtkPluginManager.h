/*************************************************************************
* Copyright(c) 2016,NRIET
* All Rights Reserved.
* @file		:	ICtkPluginManager.h
* @brief	:	插件管理器接口头文件定义
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

#ifndef ICtkPluginManager_h__
#define ICtkPluginManager_h__

#include <QObject>
#include <QtPlugin>

#include "typedefs.h"


class ctkPluginContext;
class ctkPluginFrameworkFactory;
class LogInterface;
class IFileTransfer;
class pluginInfo;

class ICtkPluginManager : public QObject
{
    Q_OBJECT
public:
    virtual ~ICtkPluginManager() {}

    virtual void init() = 0;

    virtual void uninit() = 0;

    virtual ctkPluginFrameworkFactory *getCtkPluginFrameworkFactory() = 0;

    virtual ctkPluginContext *getPluginContext() = 0;

    virtual ICtkPluginManager* instance() = 0;

    virtual QObject *getService(const QString &interfaceName) = 0;

    virtual QVector<pluginInfo*>& getLoadedPluginBaseInfo() = 0 ;

    virtual LogInterface* GetMsgLogObj(LOGWIDGETTYPE logWT) = 0 ;

    virtual void SetMsgLogObj(LogInterface* pLog, LOGWIDGETTYPE logWT) = 0 ;

    virtual IFileTransfer* GetTransferObj(TRANSFERSYSTEMTYPE TranOT) = 0;

    virtual void SetTransferObj(IFileTransfer* pTrans, TRANSFERSYSTEMTYPE TranOT) = 0;
};

Q_DECLARE_INTERFACE(ICtkPluginManager, "ctkPluginManager.ICtkPluginManager")


#endif // ICtkPluginManager_h__