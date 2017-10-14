#ifndef radarProduct_H_
#define radarProduct_H_

#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
#include "../macro.h"


class RadarProductSet
{
public:
    int		nID;			///< 产品编号
    string	strChName;		///< 中文描述
    string	strEnName;		///< 英文缩写
    bool	bData;			///< 是否出数据
    bool	bPicture;		///< 是否出图片
    int		nType;			///< 产品类型（从0开始）

    RadarProductSet()
    {
        nID = 0;
        bData = false;
        bPicture = false;
        nType = 0;
    }
};

class RadarProductList
{
public:
    vector<RadarProductSet> lst;
};

/**
 * @brief  将产品设置写入XML文件
 * @param  const string & strPath：XML文件的全路径
 * @param  const RadarProductList & lst：产品设置
 * @return DLL_EXPORT_DECL bool：成功返回true，失败返回false
 */
DLL_EXPORT_DECL bool RadarProductToXml(const string &strPath, const RadarProductList &lst);

DLL_EXPORT_DECL bool XmlToRadarProduct(const string &strPath, RadarProductList &lst);

#endif