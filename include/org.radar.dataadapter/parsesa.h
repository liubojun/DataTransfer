/*
 * ParseSa.h
 *
 *  Created on: Sep 23, 2014
 *      Author: root
 */

#ifndef PARSESA_H_
#define PARSESA_H_
#include "struct_sa.h"
#include "struct_vtb.h"
#include "ctkLog.h"
#include <vector>
#include <string>
using namespace std;

class CParseSa
{
public:
    CParseSa();
    virtual ~CParseSa();
    //解析SA或SB基数据雷达文件(Z_RADR_I_Z9002_20141224223000_O_DOR_SA_CAP.bin.bz2),fileName为文件的全路径名
    bool parse(const char *fileName, RadarDataHead &fileHead, QVector<LineDataBlock> &dataBuffer);

    //解析WZ基数据雷达文件(2015071916.32A),fileName为文件的全路径名
    bool parseWZSA(const char *fileName, RadarDataHead &fileHead, QVector<LineDataBlock> &dataBuffer);
    int getLayNumber() const;

    static bool isSAFile(const char *fileName);

private:
    //根据文件名判断是否符合SA数据
    bool isValidBz2FileName(const char *fileName);

    //根据文件名称判断是否符合温州基数据
    bool isValidWZSAFileName(const char *fileName);

    //读SA文件里的所有数据，并保存到m_saDataLst里
    bool loadSAFile(const char *fileName);

    //读WZ基数据文件里的所有数据，并保存到m_saDataLst里
    bool loadWZSAFile(const char *fileName);

    /**
     * @brief  把SA结构体的部分转换到VTB的头
     * @param  RadarDataHead & fileHead：转换后的头部
     * @param  int validData：有效资料索引号
     * @return void：描述返回值
     */
    void convertSAToVTB(RadarDataHead &fileHead, int validData);

    //把SA结构体的数据部分转换到VTB的数据
    void convertSABufferToVTB(RadarDataHead &fileHead, QVector<LineDataBlock> &dataBuffer, int validData);
private:
    void GetSiteInfo(RadarDataHead &fileHead);

private:
    vector<SaRadialData> m_saDataLst;
    int m_layerNumber;
    string m_stationNumber;
    string m_strtime;
};

#endif /* PARSESA_H_ */
