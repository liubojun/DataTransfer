/**
  * @file   hwStation.h
  * @brief  大风区站点配置文件.
  *
  *  这里是详细说明。
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:08:25
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef hwStation_H_
#define hwStation_H_


#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include "../macro.h"
using namespace std;


class HWstation
{
public:
    string strId;		// 站号
    string strName;		// 站名
    string strLat;		// 纬度
    string strLon;		// 经度
    string strVgate;	// 速度阈值
    string strAreaGate;	// 大风面积阈值
    string strRange;	// 雷达的最大最小经纬度与中心经纬度之间的经纬度差
};

class HWstationMap
{
public:
    map<string, HWstation> mapIdSet;	// 站号-配置
};

DLL_EXPORT_DECL bool HwStationToXml(const std::string &strPath, const HWstationMap &info);
DLL_EXPORT_DECL bool XmlToHwStation(const std::string &strPath, HWstationMap &info);

#endif