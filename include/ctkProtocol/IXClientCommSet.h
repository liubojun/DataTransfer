#ifndef _IXCLIENTCOMMSET_H_20160728_
#define _IXCLIENTCOMMSET_H_20160728_

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
  * @brief  通讯设置载入保存配置信息
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class IXClientCommSet
{
public:
    string Server;///<服务器地址
    int SubClientPort;///<服务器广播消息的端口（不含文件内容）
    int ReqClientPort;///<服务器请求数据的端口（请求文件和接受文件的端口）
};


/**
 * @brief  将通讯设置对象写入XML文件
 * @param  const string & strPath：XML文件的全路径
 * @param  const IXClientCommSet & lst：通讯设置对象
 * @return DLL_EXPORT_DECL bool：成功返回true，失败返回false
 */
DLL_EXPORT_DECL bool IXClientCommSet2XML(const string &strPath, const IXClientCommSet &lst);
/**
 * @brief  从XML文件读取信息到通讯设置对象
 * @param  const string & strPath：XML文件的全路径
 * @param  const IXClientCommSet & lst：通讯设置对象
 * @return DLL_EXPORT_DECL bool：成功返回true，失败返回false
 */
DLL_EXPORT_DECL bool XML2IXClientCommSet(const string &strPath, IXClientCommSet &lst);

//////////////////////////////////////////////////////////////////////////
//@brief: add by weikangc [9/13/2016 Administrator]

/**
  * @class <FourProductInfo>
  * @brief  四分屏产品类型列表单一类型.
  *
  *  这里是类的详细描述
  * @note
  * @see     这里是参考类名
 */
class FourProductInfo
{
public:
    std::string  productname[4];
    std::string  productcode[4];
    unsigned int productheight[4];
public:
    FourProductInfo()
    {
		init();
    }
	void init()
	{
		for ( int i = 0; i < 4; i++)
		{
			productname[i]	 = "";
			productcode[i]	 = "";
			productheight[i] = 0 ;
		}
	}
	bool operator==(const FourProductInfo& rhs)
	{
		for ( int i = 0; i < 4; i++)
		{
			if (this->productcode[i] != rhs.productcode[i] 
			|| this->productheight[i] != rhs.productheight[i])
			return false;
		}
		return true;
	}
};

class FourProductInfos
{
public:
    std::vector<FourProductInfo> m_fPIS;
};

DLL_EXPORT_DECL bool FourProductInfos2Xml(const string &strPath, const FourProductInfos &lst);

DLL_EXPORT_DECL bool Xml2FourProductInfos(const string &strPath, FourProductInfos &lst);

//////////////////////////////////////////////////////////////////////////

#endif
