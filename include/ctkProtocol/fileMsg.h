#ifndef fileMsg_H_
#define fileMsg_H_

#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <QList>
#include "../macro.h"
#include "../org.radar.dataadapter/filenametable.h"

/**
  * @class <FileInfo>
  * @brief  文件信息，用于消息序列化
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
 */
class DLL_EXPORT_CLASS_DECL FileInfo
{
public:
    std::string strFileName;	//文件名
    std::string strFilePath;	//全路径
    std::string strFileTime;	//文件时间
    std::string strFileType;	//文件类型
    std::string strStation;		//站名

    std::string strMdyTime;		//最后修改时间
    RADAR_FILE_TYPE nFileType;	//文件类型
    std::string strOldType;		//原始类型
    long long	nFileSize;		//文件大小（单位：字节）

    std::vector<std::string> strDestQueues;	// 该文件需要发送的消息队列名(该字段无需序列化、反序列化)
    std::string strOrgFilePath;	//原始全路径（用于记录时间）

    FileInfo()
    {
        nFileSize = 0;
    }
//     FileInfo(const FileInfo &fi);
//     FileInfo& operator=(const FileInfo &fi);
};

DLL_EXPORT_C_DECL void FileInfoToString(std::string &strMsg, const FileInfo &fileInfo);
DLL_EXPORT_C_DECL void StringToFileInfo(const std::string &strMsg, FileInfo &fileInfo);

typedef QList<FileInfo> FileInfoList;


#endif // testserial_h__
