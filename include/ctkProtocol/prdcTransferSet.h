/**
  * @file   prdcTransferSet.h
  * @brief  产品分发设置.
  *
  *  这里是详细说明。
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:08:01
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef prdcTransferSet_H_
#define prdcTransferSet_H_


#include <string>
#include <map>
#include <vector>
#include <list>
#include <sstream>
#include <fstream>
using namespace std;
#include "../macro.h"
#include "fileMsg.h"


/**
  * @class <PrdcTransferSet>
  * @brief  产品分发设置序列化
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
 */
class PrdcTransferSet
{
public:
    string	strID;				//任务标识（要求唯一）
    bool	bRadarData;
    bool	bRadarPic;
    bool	bMosaicData;
    bool	bMosaicPic;
    bool	bPupData;
    bool	bPupPic;
    bool	bHighData;
    bool	bHighPic;
    vector<string> vecRecvID;	//用户ID
    bool	bEnable;			//是否启用

    PrdcTransferSet()
    {
        bRadarData = false;
        bRadarPic = false;
        bMosaicData = false;
        bMosaicPic = false;
        bPupData = false;
        bPupPic = false;
        bHighData = false;
        bHighPic = false;
        bEnable = true;
    }
};

class PrdcTransferSetList
{
public:
    list<PrdcTransferSet> lst;
};

DLL_EXPORT_DECL bool PrdcTransferSetToXml(const string &strPath, const PrdcTransferSetList &info);
DLL_EXPORT_DECL bool XmlToPrdcTransferSet(const string &strPath, PrdcTransferSetList &info);


#endif