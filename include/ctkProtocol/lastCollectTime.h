/**
  * @file   lastCollectTime.h
  * @brief  记录上次收集的时间.
  *
  *  这里是详细说明。
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:08:02
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef lastCollectTime_H_
#define lastCollectTime_H_


#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include "../macro.h"
using namespace std;


class CollectTime
{
public:
    map<string, string> dir_time;		///< 目录-时间
};

class CollectTimeList
{
public:
    map<string, CollectTime> id_last;	///< ID(唯一)-最新收集时间
};

class LatestFileList
{
public:
	list<string> m_files;	/// 当前目录下最新时间的文件列表
};


DLL_EXPORT_DECL bool CollectTimeToXml(const std::string &strPath, const CollectTimeList &lst);
DLL_EXPORT_DECL bool XmlToCollectTime(const std::string &strPath, CollectTimeList &lst);


DLL_EXPORT_DECL bool LatestFileListToXml(const std::string &strPath, const LatestFileList &lst);
DLL_EXPORT_DECL bool XmlToLatestFileList(const std::string &strPath, LatestFileList &lst);

#endif