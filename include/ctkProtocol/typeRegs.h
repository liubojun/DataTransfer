/**
  * @file   typeRegs.h
  * @brief  数据类型对应的正则表达式.
  *
  *  这里是详细说明。
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:04:29 
  * @note   这里是注意事项。
  * @version <版本号>
  */  
#ifndef typeRegs_H_
#define typeRegs_H_

#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include "../macro.h"

/**
  * @class <TypeRegs>
  * @brief  类型-正则表达式
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */ 
class TypeRegs
{
public:
	std::string strType;
	std::vector<std::string> vecRegs;
};

class TypeRegsList
{
public:
	std::list<TypeRegs> lsInfos;
};

DLL_EXPORT_C_DECL void TypeRegsToXml(const std::string &strPath, const TypeRegsList &infos);
DLL_EXPORT_C_DECL void XmlToTypeRegs(const std::string &strPath, TypeRegsList &infos);


#endif // testserial_h__