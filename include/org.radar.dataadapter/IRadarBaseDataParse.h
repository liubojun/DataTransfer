/**
  * @file   IRadarBaseDataParse.h
  * @brief  雷达基数据解析接口.
  *
  *  这里是详细说明。
  * @author 刘伯俊
  * @par   Copyright (c):
  *          刘伯俊.
  *         All Rights Reserved
  * @date   2016:04:07
  * @note   这里是注意事项。
  * @version 1.0
  */

#ifndef IRadarBaseDataParse_h_
#define IRadarBaseDataParse_h_

#include "ILogPrint.h"
#include "filenametable.h"
#include "struct_vtb.h"
#include <vector>
using namespace std;

class IRadarBaseDataParse : public ILogPrint
{
    Q_OBJECT
public:
    virtual ~IRadarBaseDataParse() { }
    /**
     * @brief  读取vtb文件
     * @param  const char * fileName：vtb文件全路径
     * @param  RadarDataHead & head：数据头信息
     * @param  QVector<LineDataBlock> & databuffer：数据体信息
     * @return bool：成功返回true，失败返回false
     */
    virtual bool readVtbFile(const char *fileName, RadarDataHead &head, QVector<LineDataBlock> &databuffer) = 0;
    /**
     * @brief  标准化处理
     * @param  const QString & orgfilefullpath：原始文件全路径
     * @param  QString & filetype：文件类型
     * @param  QString & vtbfilefullpath：标准化后的全路径
     * @param  QString & strFileTime：文件时间
     * @param  QString & strStation：站名
     * @return bool：成功返回true，失败返回false
     */
    virtual bool ChangeToVtb(const QString &orgfilefullpath, QString &filetype, QString &vtbfilefullpath, QString &strFileTime, QString &strStation) = 0;

    // 临时提供这个接口，process server需要
    virtual bool parse(const QString &filefullpath, const RADAR_FILE_TYPE &filetab, RadarDataHead &io_pDataHeader, QVector<LineDataBlock> &io_pDataBuffer) = 0;
};


//Q_DECLARE_INTERFACE(IRadarBaseDataParse, "org.radar.dataadapter.IRadarBaseDataParse")

#endif // IRadarBaseDataParse_h__
