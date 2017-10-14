#ifndef PARSEVTB_H_
#define PARSEVTB_H_
#include "struct_vtb.h"
#include <string>
#include <QVector>

using namespace std;

class CParseVtb
{
public:
    CParseVtb();
    virtual ~CParseVtb();
    //解析VTB20140816190023.009.zip基数据雷达文件,fileName为文件的全路径名
    bool parse(const char *fileName, RadarDataHead &fileHead, QVector<LineDataBlock> &dataBuffer);
    //01_20140801074000.000.rdv
    bool parseStandardVtb(const char *fileName, RadarDataHead &fileHead, QVector<LineDataBlock> &dataBuffer);

    int getLayNumber() const;

    static bool isVtbNewFile(const char *fileName);

public:
    //根据文件名称判断是否符合VTB基数据
    bool isValidVTBFileName(const char *fileName);
    //读VTB基数据文件里的所有数据，并保存到VTB 格式里
    bool loadVTBFile(const char *fileName);
    // 判断是否为非压缩的VTB文件
    bool isVTBFile(const char *fileName);
    // 解析非压缩vtb文件
    bool loadNoCompVtbFile(const char *fileName);

private:
    RadarDataHead m_RadarDataHead;      // 雷达数据头
    QVector<LineDataBlock> m_pLineData;      // 径向数据指针
    int m_layerNumber;                  // 扫描层数
    string m_stationNumber;             // 站点
    int m_nNumSum;                      // DBegin累加
    int m_nRadialNum;                   // 每层径向数，理论上应该为360的整数
private:
    //根据文件头中各层的DataForm值，重置内部格式文件中的各层数据起始位置
    void ReAssignDBegin();
    // 分配内存并进行初始化
    long int AssignLineDataBlock();
    void SetPointData(int layerNo,int radialNo,int gateNo,int productNo,short value);
private:

};
#endif