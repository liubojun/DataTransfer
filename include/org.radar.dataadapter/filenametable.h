#ifndef filenametable_h__
#define filenametable_h__

#include <QMap>
#include <QStringList>

// 对雷达文件类型进行划分

typedef enum
{
    SA = 1000,
    SB = 1001,
    SC = 1002,
    CA = 1003,
    CC = 1004,
    CD = 1005,
    VTB_OLD = 2000,
    VTB_NEW = 2001,
    VTB_STATDARD = 2003,
    JD717 = 3000,
    X_TYPE = 9999,		//未知类型，数据传输用
    NETREF,				///< 三维组网数据
    MAXR,				///< MAXR.bin数据
    HW_M4				///< 大风外推
} RADAR_FILE_TYPE;

namespace filenametable
{
static const std::string SA_S = "SA";
static const std::string SB_S = "SB";
static const std::string SC_S = "SC";
static const std::string CA_S = "CA";
static const std::string CC_S = "CC";
static const std::string CD_S = "CD";
static const std::string VTB_OLD_S = "VTB_OLD";
static const std::string VTB_NEW_S = "VTB_NEW";
static const std::string X_TYPE_S = "X_TYPE";
}

class FileTypeInfo
{
public:
    std::string strNameReg;
    int nStationIndex;
    int nDateTimeIndex;
};

// 文件类型-文件名表
// 其中key为文件类型, value为当前类型的文件名正则表达式列表
// 如下列组织方式
// SA -----------------------------------------
//	  --Z_RADR_I_Z[0-9]{14}_[0-9]{14}_O_DOR_SA_CAP.bin
//
// VTB ----------------------------------------
//     --VTB[0-9]{14}.[0-9]{14}.[0-9]{5}
//	   --VTB[0-9]{14}.[0-9]{3}.zip

// typedef QMap<RADAR_FILE_TYPE, QStringList> FILENAME_TABLE;
typedef QMap<RADAR_FILE_TYPE, FileTypeInfo> FILENAME_TABLE;

#endif // filenametable_h__
