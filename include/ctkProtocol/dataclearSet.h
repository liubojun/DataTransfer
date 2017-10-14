/**
  * @file   dataclearSet.h
  * @brief  数据清除配置序列化.
  *
  *  这里是详细说明。
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:05:19
  * @note   这里是注意事项。
  * @version <版本号>
  */
#ifndef dataclearSet_H_
#define dataclearSet_H_

#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
#include "../macro.h"


class DataClearSet
{
public:
    string  strTask;				///< 任务名（要求唯一）
    string	strPath;				///< 文件路径
    string	strDrule;				///< 调度规则
    int	nStyle;						///< 删除方式（0-时间，1-空间，2-两者）
    vector<string> reges;			///< 匹配规则（正则表达式）
    int	nSize;						///< 剩余空间（单位：M）
    int	nTime;						///< 保留时间（单位：h）

    //重载==，便于容器直接remove
    bool operator==(const DataClearSet& e)
    {
        if ( this->strPath == e.strPath )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

class DataClearSetList
{
public:
    list<DataClearSet> lst;
};


DLL_EXPORT_DECL bool DataClearSetToXml(const string &strPath, const DataClearSetList &setlst);
DLL_EXPORT_DECL bool XmlToDataClearSet(const string &strPath, DataClearSetList &setlst);

// /**
//   * @class <DataClearSet_N>
//   * @brief  .
//   *
//   *  这里是类的详细描述
//   * @note    这里是注意事项
//   * @see     这里是参考类名
//  */
// class DataClearSet_N
// {
// public:
//     std::string sClearTask;	///< 清除任务
//     std::string sDir;		///< 监控目录
//     std::string sFileRule;	///< 文件模板
//     int		bClearType;		///< 清除方式	0-监控 1-轮询
//     int		bClearscheme;	///< 清除方案  （0-时间，1-空间，2-两者）
//     int		nSize;			///< 剩余空间（单位：M）
//     int		nTime;			///< 保留时间（单位：t）
//     string	strDrule;		///< 调度规则
// };
//
// /**
//   * @class <DataClearSetList_N>
//   * @brief  .
//   *
//   *  这里是类的详细描述
//   * @note    这里是注意事项
//   * @see     这里是参考类名
//  */
// class DataClearSetList_N
// {
// public:
//     std::vector<DataClearSet_N> arrayDCS;
// };
//
// /**
//  * @brief: 简述函数功能
//  * @param: const string & strPath
//  * @param: const DataClearSetList_N & setlst
//  * @return: DLL_EXPORT_DECL bool: 描述返回值
// */
// DLL_EXPORT_DECL bool DataClearSetNToXml(const string &strPath, const DataClearSetList_N &setlst);
//
// /**
//  * @brief: 简述函数功能
//  * @param: const string & strPath
//  * @param: DataClearSetList_N & setlst
//  * @return: DLL_EXPORT_DECL bool: 描述返回值
// */
// DLL_EXPORT_DECL bool XmlToDataClearSetN(const string &strPath, DataClearSetList_N &setlst);

#endif