#ifndef mosaicArea_h__
#define mosaicArea_h__

#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
#include "../macro.h"

#include "ParaConfig.h"


class RadarMosaicArea
{
public:
    std::string sID;						///< 区域编号
    std::string	sAreaName;					///< 区域名称
    bool bUse;								///< 是否启用
    std::vector<CSStation> stationArray;	///< 该区域的所有站点信息
public:
    RadarMosaicArea():sID(""),sAreaName(""),bUse(false)
    {
        stationArray.clear();
    }
    RadarMosaicArea(const std::string& id, const std::string& areaname, bool bFlag, const std::vector<CSStation>& arrayStation):sID(id),sAreaName(areaname),bUse(bFlag),stationArray(arrayStation) {}
    RadarMosaicArea(const RadarMosaicArea& rMA):sID(rMA.sID),sAreaName(rMA.sAreaName),bUse(rMA.bUse),stationArray(rMA.stationArray) {}
    RadarMosaicArea& operator=(const RadarMosaicArea& rMA)
    {
        if ( &rMA != this )
        {
            this->sID = rMA.sID;
            this->sAreaName = rMA.sAreaName;
            this->bUse= rMA.bUse;
            this->stationArray = rMA.stationArray;
        }

        return *this;
    }
    ~RadarMosaicArea() {}
};

class RadarMosaicAreaList
{
public:
    vector<RadarMosaicArea> lst;
};

typedef std::vector<CSStation> ArrayStations;

/**
 * @brief: 简述函数功能
 * @param: const string & strPath
 * @param: const RadarMosaicAreaList & lst
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool RadarMosaicAreaToXml(const string &strPath, const RadarMosaicAreaList &lst);

/**
 * @brief: 简述函数功能
 * @param: const string & strPath
 * @param: RadarMosaicAreaList & lst
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool XmlToRadarMosaicArea(const string &strPath, RadarMosaicAreaList &lst);

/**
  * @class <monitor_>
  * @brief  .
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class monitor_
{
public:
    monitor_():radarnum(""),radarname(""),radarip("") {}
    monitor_(const std::string& radarnum_, const std::string& radarname_, const std::string& radarip_):radarnum(radarnum_),radarname(radarname_),radarip(radarip_) {}
    monitor_(const monitor_& m):radarnum(m.radarnum),radarname(m.radarname),radarip(m.radarip) {}
    monitor_& operator=(const monitor_ &m)
    {
        if ( this != &m )
        {
            this->radarnum = m.radarnum;
            this->radarname= m.radarname;
            this->radarip  = m.radarip;
        }
        return *this;
    }
public:
    std::string radarnum;
    std::string radarname;
    std::string radarip;
};

class monitor_s
{
public:
    std::vector<monitor_> ArrayMonitors;
};

/**
 * @brief: 简述函数功能
 * @param: const string & strPath
 * @param: const monitor_s & lst
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool RadarMonitorToXml(const string &strPath, const monitor_s &lst);

/**
 * @brief: 简述函数功能
 * @param: const string & strPath
 * @param: monitor_s & lst
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool XmlToRadarMonitor(const string &strPath, monitor_s &lst);

/**
  * @class <>
  * @brief  .
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */

class welkin
{
public:
    welkin():m_bType(true),m_picHeight(72),m_picWidth(72) {}
    welkin(bool bType, int picHeight, int picWidth):m_bType(bType),m_picHeight(picHeight),m_picWidth(picWidth) {}
    welkin(const welkin& w):m_bType(w.m_bType),m_picHeight(w.m_picHeight),m_picWidth(w.m_picWidth) {}
    welkin& operator=(const welkin& w)
    {
        if ( this == &w )
        {
            this->m_bType	 = w.m_bType;
            this->m_picHeight= w.m_picHeight;
            this->m_picWidth = w.m_picWidth;
        }

        return *this;
    }
    ~welkin() {}
public:
    bool m_bType;
    int  m_picHeight;
    int	 m_picWidth;
};

class welkin_s
{
public:
    welkin_s():m_bShowPhy(true) {};
    ~welkin_s() {}
public:
    welkin m_tlogP;
    welkin m_windrosepic;
    bool   m_bShowPhy;
};


/**
 * @brief: 简述函数功能
 * @param: const string & strPath
 * @param: const welkin_s & lst
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool WelkinToXml(const string &strPath, const welkin_s &lst);


/**
 * @brief: 简述函数功能
 * @param: const string & strPath
 * @param: welkin_s & lst
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool XmlToWelkin(const string &strPath, welkin_s &lst);


class mosaicQueue
{
public:
    mosaicQueue():m_groupBoxName(""),m_messageQueue(""),m_archiveQueue(""),m_broadcastQueue("") {}
    ~mosaicQueue() {}
public:
    std::string m_groupBoxName;
    std::string m_messageQueue;
    std::string m_archiveQueue;
    std::string m_broadcastQueue;
};

/**
 * @brief: 简述函数功能
 * @param: const string & strPath
 * @param: const welkin_s & lst
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool MosaicQueueToXml(const string &strPath, const mosaicQueue &lst);

/**
 * @brief: 简述函数功能
 * @param: const string & strPath
 * @param: welkin_s & lst
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool XmlToMosaicQueue(const string &strPath, mosaicQueue &lst);

#endif // mosaicArea_h__