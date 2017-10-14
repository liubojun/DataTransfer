/**
  * @file   IRadarPubDataParse.h
  * @brief  雷达产品数据解析接口.
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

#ifndef IRadarPubDataParse_h__
#define IRadarPubDataParse_h__

#include "ILogPrint.h"
#include "commontypedef.h"
#include "ProductDef.h"
#include "pupstruct.h"
#include "struct_swan.h"
#include "datamodel/gridpointdata.h"
using namespace datamodel;

//#define 0.0f 0.0f

class ICtkPluginManager;

class IRadarPubDataParse : public ILogPrint
{
    Q_OBJECT
public:
    /**
     * @brief  解析rdf格式pub数据
     * @param  const char * filefullpath：数据全路径
     * @param  void * header：用于存储解析得到的帧头
     * @param  void * * buffer：用于存储解析得到的数据体
     * @param  int type：0-polar，1-suqar，2-vector
     * @return bool：解析成功返回true，失败返回false
     */
    virtual bool parse(const char *filefullpath, void *header, void *&buffer, EProductType type) = 0;

    /* @brief  解析pup产品数据
     * @param  const char * filefullpath：数据全路径
     * @param  SquareProduct &header：用于存储解析得到的帧头
     * @param  std::vector<char> &buffer：用于存储解析得到的数据体
     * @param  int type：0-polar，1-suqar，2-vector
     * @return bool：解析成功返回true，失败返回false
     */
    virtual bool parsePup(const char *filefullpath, SquareProduct &header, std::vector<char> &buffer, EProductType type) = 0;

    virtual bool PolarToSquare(PolarProduct &pHeader, std::vector<PolarDataBuffer> &pBuff, SquareProduct &header, std::vector<char> &buffer) = 0;

    /**
     * @brief: 读Polar格式的rdf文件,并转化为square格式传出
     * @param: const char * filename
     * @param: SquareProduct & header
     * @param: std::vector<char> & buffer
     * @return: bool: 描述返回值
    */
    virtual bool parsePolarRdfToSquare(const char* filename, SquareProduct &header,  std::vector<char> &buffer) = 0;

    /**
     * @brief  判断文件的类型
     * @param  const char * filefullpath 文件全路径
     * @param  bool & retFlag 读取文件正确为true, 读取错误为false
     * @return EProductType：文件类型 0-polar，1-suqar，2-vector
     */
    virtual EProductType type(const char *filefullpath, bool &retFlag, int &prodId) = 0;

    /**
     * @brief  设置ctk插件管理器
     * @param  ICtkPluginManager * in_pManager：插件管理器指针
     * @return void：无
     */
    virtual void setPluginInstance(ICtkPluginManager *in_pManager) = 0;

    virtual bool parseFromPup(const std::string &filefullpath, PupData &pupData, const std::string &fileTime, const std::string &station) = 0;

    virtual bool parseMaxPup(const QString &filePath, MaxPupData &maxData) = 0;

    virtual bool parseMic4(const QString &fileFullPath, DIAMOND4 &data, QStringList &lstInfo) = 0;

    virtual bool parseMic4(const QString &filePath, GridArray &data) = 0;

    virtual bool parseNc(const QString &strFilePath, const std::string &varName, GridArray &grid) = 0;

    //----临时测试----
    virtual bool calDropZone(const QString &strFilePath, GridArray &data) = 0;

    virtual bool calDzSateRain(GridArray &data) = 0;
    virtual bool calDzSateRain(const QString &filePath, float minVal,float maxVal, GridArray &data) = 0;
    virtual bool calDzSateRef(const QString &filePath, int minVal,GridArray &data) = 0;//处理粒子半径
    virtual bool calDzSateLwp(const QString &filePath, float minVal,GridArray &data) = 0;//处理液水路径

    virtual bool calDropZoneRadar(const QString &strFilePath, GridArray &data) = 0;

    virtual bool calDropZoneRadarCR(const QString &strFilePath, GridArray &data, int DbzValue) = 0;
    virtual bool calDropZoneRadarET(const QString &strFilePath, GridArray &data, int val) = 0;
    virtual bool calDropZoneRadarVIL(const QString &strFilePath, GridArray &data, int val) = 0;
    virtual bool calDropZoneRadarTrec(const QString &strFilePath, GridArray &data,int val) = 0;
    virtual bool parseMic4Soil(const QString &filePath, GridArray &data, float val) = 0;

};

//Q_DECLARE_INTERFACE(IRadarPubDataParse, "org.radar.dataadapter.IRadarPubDataParse")
#endif // IRadarPubDataParse_h__