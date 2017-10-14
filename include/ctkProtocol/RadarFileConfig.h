#ifndef _RADARFILECONFIG_H_20160730_
#define _RADARFILECONFIG_H_20160730_

#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
#include "../macro.h"
/**
  * @class <IXClientCommSet>
  * @brief  雷达文件接受、管理
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class RadarFileMgrConfig
{
public:
    RadarFileMgrConfig()
	{
		RadarFileKeepDays = 7;
		RadarFileHours = 24;
	}
    ~RadarFileMgrConfig(){}
    
    string RadarFileBasePath;///<雷达文件存储的基准路径
	int RadarFileKeepDays;///<雷达文件保存天数
	int RadarFileHours;///<取多少小时内的雷达文件
};


/**
 * @brief  将雷达文件管理配置对象写入XML文件
 * @param  const string & strPath：XML文件的全路径
 * @param  const RadarFileMgrConfig & lst：雷达文件管理配置对象
 * @return DLL_EXPORT_DECL bool：成功返回true，失败返回false
 */
DLL_EXPORT_DECL bool RadarFileMgrConfig2XML(const string &strPath, const RadarFileMgrConfig &lst);
/**
 * @brief  从XML文件读取信息到雷达文件管理配置对象
 * @param  const string & strPath：XML文件的全路径
 * @param  const RadarFileMgrConfig & lst：雷达文件管理配置对象
 * @return DLL_EXPORT_DECL bool：成功返回true，失败返回false
 */
DLL_EXPORT_DECL bool XML2RadarFileMgrConfig(const string &strPath, RadarFileMgrConfig &lst);

#endif
