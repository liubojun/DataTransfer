/**
  * @file   pluginBaseInfo.h
  * @brief  插件基本信息管理.
  *
  *  这里是详细说明。
  * @author WEIKANGC
  * @par   Copyright (c):
  *          WEIKANGC.
  *         All Rights Reserved
  * @date   2016:09:13
  * @note   这里是注意事项。
  * @version <版本号>
  */
#ifndef pluginBaseInfo_h__
#define pluginBaseInfo_h__

#include "macro.h"
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

/**
  * @class <pluginInfo>
  * @brief  插件基本信息管理.
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */

class pluginInfo
{
public:
	bool bLoad;						///< 是否卸载 ：true 加载 or false 卸载
    bool bUse;						///< 是否启用 ：true 启用 or false 禁用
    std::string symbolic_name;		///< 使用符号 ：对外导出符号，即服务名称
    std::string name;				///< 插件名称 ：英文名称
	std::string cname;				///< 插件名称 ：中文名称 
    std::string version;			///< 插件版本
    std::string vendor;				///< 插件作者
    std::string license;			///< 授权文件
    std::string copyright;			///< 插件版权
    std::string activation_policy;	///< 激活策略 目前存在两个 "eager" or "lazy" 0 or 1
    std::string description;		///< 插件描述
    std::string category;			///< 插件种类
    std::string contact_address;	///< 联系地址
    std::string doc_url;			///< 文档链接
    std::string icon;				///< 插件图标
public:
    pluginInfo()
    {
		this->bLoad=true;
        this->bUse = true;
        this->symbolic_name.empty();
        this->name.empty();
		this->cname.empty();
        this->version.empty();
        this->vendor.empty();
        this->license.empty();
        this->copyright.empty();
        this->activation_policy.empty();
        this->description.empty();
        this->category.empty();
        this->contact_address.empty();
        this->doc_url.empty();
        this->icon.empty();
    }
};

DLL_EXPORT_DECL bool PluginInfoToXml(const string &strPath, const pluginInfo &set);

DLL_EXPORT_DECL bool XmlToPluginInfo(const string &strPath, pluginInfo &set);

#endif // pluginBaseInfo_h__
