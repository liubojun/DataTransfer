/**
  * @file   fileTmplate.h
  * @brief  文件模板.
  *
  *  这里是详细说明。
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:09:01
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef fileTmplate_H_
#define fileTmplate_H_


#include <QString>
#include <QMap>
#include <QVector>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
#include "../macro.h"


class FileTmplate
{
public:
    FileTmplate() {}
    vector<string> vecType;		///< 类型
    vector<string> vecReg;		///< 正则
    vector<int>	   vecIndex;	///< 索引
};

DLL_EXPORT_DECL bool FileTmplateToXml(const string &strPath, const FileTmplate &set);
DLL_EXPORT_DECL bool XmlToFileTmplate(const string &strPath, FileTmplate &set);


#endif
