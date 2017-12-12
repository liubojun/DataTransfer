/**
  * @file   IBaseInterface.h
  * @brief  雷达服务插件公用方法头文件声明及其内联实现.
  *
  *  这里是详细说明。
  * @author nriet
  * @par   Copyright (c):
  *          nriet.
  *         All Rights Reserved
  * @date   2016:04:11
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef IBaseInterface_h__
#define IBaseInterface_h__

#include <QObject>
#include "ICtkPluginManager.h"
class pluginInfo;

/**
  * @class <IBaseInterface>
  * @brief  所有插件服务接口均需要集成的接口
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class IBaseInterface : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief: 构造函数
     * @return: 描述返回值
    */
    IBaseInterface():m_pManager(NULL) {}
    /**
     * @brief  默认析构函数
     */
    virtual ~IBaseInterface() {}

    /**
     * @brief  雷达服务插件初始化; 内部插件管理器对象指针.
     * @param[in]  ICtkPluginManager * in_pManager 插件管理器对象地址.
     * @return void：无
     */
    void init(ICtkPluginManager *in_pManager)
    {
        m_pManager = in_pManager;
    }

    /**
     * @brief  雷达服务插件初始化; 内部插件管理器对象指针.
     * @return ICtkPluginManager *：返回插件管理器对象地址.
     */
    ICtkPluginManager *getCtkPluginManager()
    {
        return m_pManager;
    }

    /**
     * @brief  读取与本插件相关联配置项. 主要用于由子类插件实现读取与其所需的配置项.
     * @return bool：执行成功返回true , 失败返回false.
     */
    virtual bool ReadRelationConfig()
    {
        return true;
    }

    /**
     * @brief: 加载插件基本信息
     * @return: void: 描述返回值
    */
    virtual pluginInfo* loadPluginBaseInfo()
    {
        return NULL;
    }
signals:
    void forbidden(bool); //true启用  false 禁用
    void load(bool);	  //true加载  false 卸载
private:
    /// 插件管理器指针
    ICtkPluginManager *m_pManager;
};

Q_DECLARE_INTERFACE(IBaseInterface, "radarproject.common.IBaseInterface")

#endif // IBaseInterface_h__
