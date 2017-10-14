#ifndef PARSECC_H
#define PARSECC_H

#include "struct_cc.h"
#include "vtbsource.h"

//解析CINRAD/CC雷达
class CParseCC
{
public:
    CParseCC();
    ~CParseCC();
    //解析CINRAD/CC雷达入口函数
    bool parse(const QString &fileName, RadarDataHead &fileHead, LineDataBlock *&dataBuffer);
    bool parse(const char *fileName, RadarDataHead &fileHead, QVector<LineDataBlock> &dataBuffer);

private:
    bool isCCFile(const char *filename);
    bool loadVPPIData(const char *filename);
    void convertCCToVTB(RadarDataHead &filehead);
    //void convertCCBufferToVTB(RadarDataHead &filehead, LineDataBlock* &databuffer);
    void convertCCBufferToVTB(RadarDataHead &fileHead, QVector<LineDataBlock> &dataBuffer);

private:
    WEATHERRADAR m_ccFileHead;     //文件头
    short *m_pData;
    VPPISCANPARAMETER m_vppiParam[30];
    int m_layerNumber;                              //体扫层数
};

#endif // PARSECC_H
