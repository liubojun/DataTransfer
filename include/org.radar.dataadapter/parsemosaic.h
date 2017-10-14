/**
  * @file   parsemosaic.h
  * @brief  解决雷达组网拼图数据.
  *
  *  这里是详细说明。
  * @author WEIKANGC
  * @par   Copyright (c):
  *          WEIKANGC.
  *         All Rights Reserved
  * @date   2016:10:20
  * @note   这里是注意事项。
  * @version <版本号>
  */
#ifndef parsemosaic_h__
#define parsemosaic_h__

#include "IRadarMosaicDataParse.h"

/**
  * @class <CParseMosaic>
  * @brief  雷达组网拼图产品数据解决类.
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class CParseMosaic : public IRadarMosaicDataParse
{
public:
    CParseMosaic();
    virtual ~CParseMosaic();
public:
    // 解析标准化后的组网数据
    virtual int readMosaicFile(const QString &filename, MOSAICDATAHEADER &mosaicDataHeader, std::vector<RADARSITEINFO> &pSiteInfo,
                               std::vector<unsigned char> &pData, std::vector<short> &pDataStartPos);

    //解析地方组网拼图数据
    virtual int parseRegionalMosaicFile(QString &filename);

    //解析地方组网拼图数据
    virtual int parseRegionalMosaicFile(const QString &filename, MOSAICDATAHEADER &mosaicDataHeader, std::vector<RADARSITEINFO> &pSiteInfo,
                                        std::vector<unsigned char> &pData, std::vector<short> &pDataStartPos);
private:
    //根据文件名判断是否符合地方bz2组网数据
    bool isValidMosaicBz2FileName(const char *fileName);

    //根据文件名判断是否符合地方zip组网数据
    bool isValidMosaicZipFileName(const char *fileName);

    //根据文件名称判断是否符合地方bin组网数据
    bool isValidMosaicBinFileName(const char *fileName);

    //读取地方bin组网拼图文件
    bool ReadBinMosaicFile(const std::string& r_fileName, MosaicHead &m_mosaicHead, unsigned char* &m_pMosaicData);

    //读取地方zip组网拼图文件
    bool ReadZipMosaicFile(const std::string& r_fileName, MosaicHead&	m_mosaicHead, unsigned char * &m_pMosaicData );

    //读取地方bz2组网拼图文件
    bool ReadBz2MosaicFile(const std::string& r_fileName, MosaicHead&	m_mosaicHead, unsigned char * &m_pMosaicData );

    //生成文件名称
    QString getFileName(const QString& path);
};

#endif // parsemosaic_h__
