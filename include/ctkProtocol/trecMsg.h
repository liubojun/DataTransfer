#ifndef trecMsg_H_
#define trecMsg_H_

#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <QList>
#include "../macro.h"
using namespace std;


class DLL_EXPORT_CLASS_DECL TrecInfo
{
public:
    std::string strType;			///< 类型（pup、mosaic...）
    std::string strProduct;			///< 产品ID
    std::string strStation;			///< 站名
    std::string strFilePath;		///< 全路径
    int			nTrecNum;			///< 外推所需文件数目
    vector<int> vecTrecTime;		///< 外推时次
    vector<float> vecTrecWeight;	///< 外推权重（个数：nTrecNum-1）
    std::string strOutPath;			///< 外推数据根路径
    bool		bDrawPic;			///< 是否绘图
    std::string strOutPicPath;		///< 外推图片根路径
};

DLL_EXPORT_C_DECL bool TrecInfoToString(std::string &strMsg, const TrecInfo &info);
DLL_EXPORT_C_DECL bool StringToTrecInfo(const std::string &strMsg, TrecInfo &info);


#endif
