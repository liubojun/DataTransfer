/**
  * @file   ConnectConfig.h
  * @brief  组网参数及高度层分辨率设置序列化与反序列化声明头文件.
  *
  *  这里是详细说明。
  * @author WEIKANGC
  * @par   Copyright (c):
  *          WEIKANGC.
  *         All Rights Reserved
  * @date   2016:05:25
  * @note   这里是注意事项。
  * @version <版本号>
  */
#ifndef ParaConfig_h__
#define ParaConfig_h__
#include "macro.h"

#include <string>
#include <vector>
#include <map>
using namespace std;

#pragma warning(disable:4251)

/**
  * @class <CSStation>
  * @brief 雷达站基本信息描述
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL CSStation
{
public:
    CSStation();
    CSStation(std::string sStationNo, std::string sStationName, const double fLon,
              const double fLat, const double elevation, std::string radarType, const double fRange, std::string sBasePath);
    CSStation(const CSStation& cSS);
    CSStation& operator=(const CSStation& cSS);
    ~CSStation();
public:
    void SetParam(std::string sStationNo, std::string sStationName, const double fLon,
                  const double fLat, const double elevation, std::string radarType, const double fRange, std::string sBasePath);
public:
    std::string m_sStationNo;	///< 站号
    std::string m_sStationName;	///< 站名
    double m_fLon;				///< 经度
    double m_fLat;				///< 纬度
    double m_elevation;			///< 海拔高度
    std::string m_sRadarType;	///< 雷达型号
    double m_fRange;			///< 探测范围
    std::string m_sBasePath;	///< 基数据存储路径
};

/**
 * @brief: 序列化单个站点信息至XmlFile
 * @param: std::string & sXmlPath
 * @param: const CSStation & pC
 * @return: DLL_EXPORT_DECL bool 成功序列化返回true 否则返回false.
*/
DLL_EXPORT_DECL bool SerializeSStation2XmlFile(std::string& sXmlPath, const CSStation& pC);

/**
 * @brief: 反序列化单个站点信息
 * @param: const std::string & sXmlPath
 * @param: CSStation & pC
 * @return: DLL_EXPORT_DECL bool 成功序列化返回true 否则返回false.
*/
DLL_EXPORT_DECL bool DeserializeXmlFile2SStation(const std::string& sXmlPath, CSStation& pC);

/**
  * @class <StationConfig>
  * @brief  站点集合描述.
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL StationConfig
{
public:
    StationConfig();
    StationConfig(const StationConfig& sC);
    StationConfig& operator=(const StationConfig& sC);
    ~StationConfig();
public:
    std::map< std::string, std::vector<CSStation> > m_ArrayRadar;
};

/**
 * @brief: 序列化StationConfig去XML文件
 * @param: std::string & sXmlPath
 * @param: const StationConfig & pC
 * @return: DLL_EXPORT_DECL bool 成功序列化返回true 否则返回false.
*/
DLL_EXPORT_DECL bool SerializeStationConfig2XmlFile(std::string& sXmlPath, const StationConfig& pC);

/**
 * @brief: 反序列化StationConfig
 * @param: const std::string & sXmlPath
 * @param: StationConfig & pC
 * @return: DLL_EXPORT_DECL bool 成功反序列化返回true 否则返回false.
*/
DLL_EXPORT_DECL bool DeserializeXmlFile2StationConfig(const std::string& sXmlPath, StationConfig& pC);

/**
  * @class <>
  * @brief  .
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL StationS
{
public:
    StationS();
    StationS(const StationS& sC);
    StationS& operator=(const StationS& sC);
    ~StationS();
public:
    std::vector<CSStation> m_arrayStations;
};

/**
 * @brief: 简述函数功能
 * @param: std::string & sXmlPath
 * @param: const StationS & pC
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool SerializeStationS2XmlFile(const std::string& sXmlPath, const StationS& pC);

/**
 * @brief: 简述函数功能
 * @param: const std::string & sXmlPath
 * @param: StationS & pC
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool DeserializeXmlFile2StationS(const std::string& sXmlPath, StationS& pC);

/**
  * @class <ParaConfig>
  * @brief  序列化组网时间参数、高度与分辨率参数.
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL ParaConfig
{
public:
    ParaConfig();
    ParaConfig(std::string dir_, const unsigned int matchTime_, const unsigned int timeGap_,
               const unsigned int timeDelay_, const double raial_, const double zonal_);
    ParaConfig(const ParaConfig& pC);
    ParaConfig& operator=(const ParaConfig& pC);
    ~ParaConfig();
public:
    void SetParam(std::string dir_, const unsigned int matchTime_,const unsigned int timeGap_,
                  const unsigned int timeDelay_, const double raial_, const double zonal_);
public:
    std::string m_dir;					///生成文件路径
    unsigned int m_matchTime;			///匹配时间
    unsigned int m_timeGap;				///时间间隔
    unsigned int m_timeDelay;			///时间延迟
    double m_raial;						///径向
    double m_zonal;     				///纬向
    std::map<unsigned int, bool> m_highlayers; ///高度层
};

/**
 * @brief: 序列化客户端配置信息至XML文件中
 * @param: std::string & sXml
 * @param: const ParaConfig & pC
 * @return: void
*/
DLL_EXPORT_DECL bool SerializePara2XmlFile(std::string& sXmlPath, const ParaConfig& pC);

/**
 * @brief: 反序列化客户端信息至其对象中
 * @param: const std::string & sXmlPath
 * @param: const ParaConfig & pC
 * @return: void
*/
DLL_EXPORT_DECL bool DeserializeXmlFile2Para(const std::string& sXmlPath,  ParaConfig& pC);
#endif // ParaConfig_h__
