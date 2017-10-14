/**
  * @file   parsejd717.h
  * @brief  军队717格式雷达数据标准化.
  *
  *  这里是详细说明。
  * @author nriet
  * @par   Copyright (c):
  *          nriet.
  *         All Rights Reserved
  * @date   2016.5.13
  * @note   这里是注意事项。
  * @version <版本号>
  */
#ifndef PARSEJD717_H_
#define PARSEJD717_H_

#include "struct_vtb.h"
#include "struct_717.h"
#include <QVector>

class CParseJD717
{
public:
    CParseJD717(void);
    ~CParseJD717(void);

    // 解析军队717格式雷达文件
    bool parse(const char*fileName, RadarDataHead &fileHead, QVector<LineDataBlock> &dataBuffer);

    // 获取扫描层数
    int getLayNumber() const;

    static bool isJD717File(const char*fileName);

private:
    // 读军队717格式文件
    bool loadJD717File(const char *fileName);

    // 转军队717格式雷达数据头到VTB格式
    void convertJD717HeaderToVTB(RadarDataHead &fileHead);
    // 转军队717格式雷达数据体到VTB格式
    void convertJD717BufferToVTB(RadarDataHead &fileHead, QVector<LineDataBlock> &dataBuffer);

private:
    int m_layerNumber;	// 扫描层数
    int m_recordNumber; // 每层的记录条数
    NewHeader717 m_radarHeader717;
    QVector<QVector<DataRecord717>> m_717DataLst;

};


#endif //PARSEJD717_H_