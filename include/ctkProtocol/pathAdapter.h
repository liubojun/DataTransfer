/**
  * @file   pathAdapter.h
  * @brief  路径适配序列化.
  *
  *  这里是详细说明。
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:06:13
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef pathAdapter_H_
#define pathAdapter_H_

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


class DataPathSetList
{
public:
    DataPathSetList() {}
    //string strRoot;							//根目录
    map<string, vector<string>> type_root;			///< 类型—根目录(data,pic)
    map<string, string> type_third;			//文件类型及对应的可配三级目录
    map<string, vector<string> > type_name;	//文件类型及对应的文件名规则（数据，图片）
};

DLL_EXPORT_DECL bool DataPathSetToXml(const string &strPath, const DataPathSetList &set);
DLL_EXPORT_DECL bool XmlToDataPathSet(const string &strPath, DataPathSetList &set);

//////////////////////////////////////////////////////////////////////////

class OrgDataBackupPath
{
public:
    OrgDataBackupPath() { }
    string strPath;							//目录规则
};

DLL_EXPORT_DECL bool OrgBackupPathToXml(const string &strPath, const OrgDataBackupPath &set);
DLL_EXPORT_DECL bool XmlToOrgBackupPath(const string &strPath, OrgDataBackupPath &set);


#endif
