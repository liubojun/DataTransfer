#ifndef _RADARANIMATIONSET_H_20160730_
#define _RADARANIMATIONSET_H_20160730_

#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
#include "../macro.h"

class RadarAnimationSet
{
public:
  RadarAnimationSet()
  {
	  m_interval=1;
	  m_adjust=200;
	  m_realInterval=m_interval;
  }
  ~RadarAnimationSet(){}
  int realInterval()
  {
	  return m_realInterval;
  }
  void speedUp()
  {
	  int t_interval = m_realInterval - m_adjust;
	  if (t_interval < 0)
		  return;
	  m_realInterval = t_interval;
  }
  void slowDown()
  {
	  m_realInterval = m_realInterval + m_adjust;
  }
  int m_interval;///<动画播放间隔,毫秒
  int m_adjust;///<调整的毫秒数，比如：加速一次加多少毫秒，为正是减速，为负数为加速
  int m_realInterval;///<实际的播放间隔
};


/**
 * @brief  将动画设置写入XML文件
 * @param  const string & strPath：XML文件的全路径
 * @param  const RadarAnimationSet & lst：动画设置
 * @return DLL_EXPORT_DECL bool：成功返回true，失败返回false
 */
DLL_EXPORT_DECL bool RadarAnimationSetToXml(const string &strPath, const RadarAnimationSet &lst);

DLL_EXPORT_DECL bool XmlToRadarAnimationSet(const string &strPath, RadarAnimationSet &lst);

#endif