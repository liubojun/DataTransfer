/**
  * @file   datacollectionActivator.h
  * @brief  数据收集插件加载器.
  *
  *  这里是详细说明。
  * @author 刘伯俊
  * @par   Copyright (c):
  *          刘伯俊.
  *         All Rights Reserved
  * @date   2016:04:07 
  * @note   这里是注意事项。
  * @version 1.0
  */  

#ifndef datacollectionActivator_h__
#define datacollectionActivator_h__

#include <ctkPluginActivator.h>


// 文件系统监控插件激活类,通过该类向上下文中注册各种服务

class IDataCollection;

class DataCollectionActivator : public QObject, public ctkPluginActivator
{
	Q_OBJECT
	Q_INTERFACES(ctkPluginActivator)
#ifdef WITH_QT5
	Q_PLUGIN_METADATA(IID "org_radar_datacollection")
#endif

public:

	/**
	 * @brief  默认构造函数
	 */
	DataCollectionActivator();

	/**
	 * @brief  析构函数
	 */
	virtual ~DataCollectionActivator();	

	/**
	 * @brief  启动插件，将插件中对外暴露的服务注册到插件上下文中
	 * @param  ctkPluginContext * context 插件上下文
	 * @return void：无
	 */
	virtual void start(ctkPluginContext *context);

	/**
	 * @brief  停止插件，将插件中需要暴露的服务从上下文移除
	 * @param  ctkPluginContext * context 插件上下文
	 * @return void：无
	 */
	virtual void stop(ctkPluginContext *context);

private:
	/// 对外暴露服务,由服务中提供对外的方法
	IDataCollection *m_pDataCollection;

};


#endif // datacollectionActivator_h__
