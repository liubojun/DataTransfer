/**
  * @file   IMakePath.h
  * @brief  设置存储路径接口.
  *
  *  这里是详细说明。
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:04:11
  * @note   供外部调用。
  * @version <版本号>
  */

#include <QObject>
#include <QString>
#include "ILogPrint.h"
#include "../org.radar.dataadapter/ProductDef.h"
#include "commontypedef.h"
#include "../ctkProtocol/fileMsg.h"
#include "../3dpart/Common//PPIStruct.h"


#ifndef IMakePath_H_
#define IMakePath_H_

class FileInfo;

class IMakePath : public ILogPrint
{
    Q_OBJECT
public:
    virtual ~IMakePath() { }

    /**
     * @brief  根据文件信息，生成数据（或产品）的存储路径
     * @param  FileNameRule & rule：数据文件信息
     * @param  QString & o_path：生成的存储全路径
     * @return int：路径构造成功返回0
     */
    virtual int makeFileFullPath(FileNameRule &rule, QString &o_path) = 0;

    /**
     * @brief  获取指定日期的基数据文件的路径
     * @param  const QDate & date：指定日期
     * @param  const QString & strStation：站号
     * @return QString：文件全路径
     */
    virtual QString getVtbDirPath(const QDate &date, const QString &strStation) = 0;

    // 返回指定子站、指定时间段的最新vtb基数据
    /**
     * @brief  返回指定子站、指定时间段的最新vtb基数据
     * @param  const QString & strStation：站号
     * @param  const QString & startTime：起始时间
     * @param  const QString & endTime：终止时间
     * @return QString：文件全路径
     */
    virtual QString getLastVtbPath(const QString &strStation, const QString &startTime, const QString &endTime) = 0;

    /**
     * @brief  根据文件全路径和类型，获取时间、站号等信息
     * @param  const QString & strFullPath：文件全路径
     * @param  FileNameRule & rule：文件信息
     * @return bool：成功返回true，否则返回false
     */
    virtual bool getFileInfo(const QString &strFullPath, FileNameRule &rule) = 0;

    virtual QString getFileRootPath(const QString &strType, bool bPic) = 0;

    /**
     * @brief  获取某类型产品在指定时间段的所有文件的全路径
     * @param  const FileNameRule & rule：指定类型、产品ID等
     * @param  const QString & strStart：起始时间
     * @param  const QString & strEnd：终止时间
     * @return QStringList：文件全路径集合
     */
    virtual QStringList getHistoryFileFullPath(const FileNameRule &rule, const QDateTime &strStart, const QDateTime &strEnd) = 0;

    virtual QStringList getRecentFileFullPath(FileNameRule &rule, const QString &lastFile, int num) = 0;

    //////////////////////////////////////////////////////////////////////////
    // 临时
    // 移走原始文件，根据strStation和strTime
    virtual bool moveOrgFile(const QString &orgFullPath, const QString &strStation, const QString &strTime) = 0;

    virtual int makePdctVtbPath(const PRODUCTMODULE &proModule, const RadarDataHead &header, QString &o_path) = 0;

    /**
    * @brief  设置基数据产品根路径
    * @param  const QString & in_strPath：产品根路径
    * @return
    */
    virtual void setBasePrdctPath(const QString &in_strPath) = 0;

    /**
    * @brief  根据vtb文件信息，生成产品存储路径
    * @param  const File_info_t & fileInfo：数据文件信息
    * @param  const RadarDataHead & header vtb结构头信息
    * @param  QString & o_path：解析的产品数据存储路径
    * @return int：路径构造成功，返回0；失败返回-1
    */
    virtual int makePreVtbPath(const FileInfo &fileInfo, const RadarDataHead &header, QString &o_path) = 0;
};

#endif