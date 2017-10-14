#ifndef SourceDataClear_h__
#define SourceDataClear_h__

#include <vector>
#include <string>
#include "macro.h"

#ifdef _WIN32
#pragma warning(disable:4251)
#endif

class DLL_EXPORT_CLASS_DECL CSourceDataClear
{
public:
    std::string strGuid;			/// 需要清理的目录id
    std::string strClearDir;		/// 需要清理的目录
    bool bUsed;						/// 是否启用
    int nClearDay;					/// 清理多少天之前的数据
};


class DLL_EXPORT_CLASS_DECL CSourceDataClearSet
{
public:
    std::vector<CSourceDataClear> m_data;
};

/**
 * @brief 对象序列化到xml文件
 * @param const std::string & path
 * @param const CSourceDataClearSet & in_obj
 * @return DLL_EXPORT_C_DECL bool：无
 */
DLL_EXPORT_C_DECL bool CSourceDataClearSet2Xml(const std::string &path, const CSourceDataClearSet &in_obj);

/**
 * @brief 从xml文件中反序列化到对象中
 * @param const std::string & path
 * @param CSourceDataClearSet & in_obj
 * @return DLL_EXPORT_C_DECL bool：无
 */
DLL_EXPORT_C_DECL bool Xml2CSourceDataClearSet(const std::string &path, CSourceDataClearSet &in_obj);

#endif // SourceDataClear_h__
