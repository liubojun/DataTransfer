#ifndef DataCollectionPath2Type_h__
#define DataCollectionPath2Type_h__

#include <string>
#include <vector>
#include "macro.h"

#pragma warning(disable:4251)
#pragma warning(disable:4018)

typedef enum
{
    JDSK = 1,
    DMSK = 2,
    JDSK_TXT = 3,
    DMSK_TXT = 4,
    CLOUD_FY2 = 5
} DATA_COLLECTION_TYPE;

namespace DataCollectionPath2Type
{
const std::string JDSK = "CJdDmSk";
const std::string DMSK = "CDfDmSk";
const std::string JDSK_TXT = "ArmyJob";
const std::string DMSK_TXT = "GroundStationJob";
const std::string CLOUD_FY2 = "CloudJob";
};


// 数据解析使用配置类
class DLL_EXPORT_CLASS_DECL CDataCollPath2Type
{
public:
    std::string strName;		// 资料名，唯一ID
    DATA_COLLECTION_TYPE nType; // 资料类型
    std::string strClassName;   // 资料类型
    std::string strUrl;			// 资料路径
};

class DLL_EXPORT_CLASS_DECL CDataCollPath2Types
{
public:
    std::vector<CDataCollPath2Type>  m_data;
};
typedef std::vector<CDataCollPath2Type> Path2Type;

DLL_EXPORT_C_DECL bool DataCollPathToXml(const std::string &strPath, const CDataCollPath2Types &info);
DLL_EXPORT_C_DECL bool XmlToDataCollPath(const std::string &strPath, CDataCollPath2Types &info);
#endif // DataCollectionPath2Type_h__
