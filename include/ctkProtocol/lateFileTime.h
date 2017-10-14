/**
  * @file   lateFileTime.h
  * @brief  上次处理文件的时间.
  *
  *  这里是详细说明。
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:04:28
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef lateFileTime_H_
#define lateFileTime_H_

#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include "../macro.h"

/**
  * @class <StationTypeTime>
  * @brief  站名-数据类型-上次处理时间
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class StationTypeTime
{
public:
    // std::string strStation;							//站名
    std::map<std::string, std::string> mapTypeTime;	//类型-时间
};

class StationTypeTimeList
{
public:
    //StationTypeTimeList()
    //{
    //	lsInfos.clear();
    //}
    //std::list<StationTypeTime> lsInfos;
    std::map<std::string, StationTypeTime> lsInfos;
};


DLL_EXPORT_C_DECL bool StationTimeToXml(const std::string &strPath, const StationTypeTimeList &info);
DLL_EXPORT_C_DECL bool XmlToStationTime(const std::string &strPath, StationTypeTimeList &info);

#endif // testserial_h__