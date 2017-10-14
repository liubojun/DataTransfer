/**
 * CInitm_BaseDatainfo->.h
 * @Author: Jackie Chen
 * @CreateDate: 2013-4-22
 * @Version 1.0
 * Copyright (C) 2013 NRIET
 */
#ifndef PRIMARYDATA_STRUCT_H
#define PRIMARYDATA_STRUCT_H

#include "PacketData_Struct.h"

// 2字节高低位互换
#define SWAP16(vaule) ((((unsigned short)vaule&0x00ff)<< 8)|(((unsigned short)vaule&0xff00)>> 8))
#define SWAP32(vaule) (((unsigned int)(vaule&0xff000000)>>24)|(((unsigned int)vaule&0x00ff0000)>>8)|(((unsigned int)vaule&0x0000ff00)<<8)|(((unsigned int)vaule&0x000000ff)<<24))

#pragma pack(push,1)

typedef struct _PrimaryDataHeader_
{
    unsigned short ProductType;  //产品名称
    unsigned short MSGdate;      //产品日期
    int            MSGTime;      //产品时间
    int            MSGlenth;     //产品的长度，包含头文件的长度(BYTE)
    unsigned short SourceID;     //发送者ID
    unsigned short DestID;       //接受者ID
    unsigned short Numberblock;  //块的个数(包含头的文件描述)
} MessageHeader;

//产品基本数据描述
typedef struct _ProductBaseInfo_
{
    int           Latitude;				//雷达站经度，单位：0.001度
    int           Longitude;			//雷达站纬度，单位：0.001度
    short          Height;              //海拔高度，单位：英尺
    short          ProductCode;         //产品代号
    unsigned short OperationalMode;     //运行模式，0=Maintenance 1=Clear Air 2=Precipitation/Server Weather
    unsigned short VolumeCoverage;      //VCP方式
    unsigned short SequenceNumber;      //产品生成序列号
    unsigned short VolumeScanNumber;    //体扫计数，1~80之间循环
    unsigned short VolumeScanDate;      //体扫日期(Julian-1/1/70)
    int            VolumeScanStartTime; //体扫时间(GMT)
    unsigned short ProductGenerationDate;//产品生成日期
} ProductBaseInfo;

typedef struct _DependentProductMSG_
{
    unsigned int   ProductGenerationTime;    //产品生成GMT时间
    unsigned short ProductDependent1;
    unsigned short ProductDependent2;
    unsigned short ElevationNumber;          //产品生成日期
    unsigned short ProductDependent3;
    unsigned short DataLevelThreshold1;      // 数据分级门限值，共16个数据
    unsigned short DataLevelThreshold2;
    unsigned short DataLevelThreshold3;
    unsigned short DataLevelThreshold4;
    unsigned short DataLevelThreshold5;
    unsigned short DataLevelThreshold6;
    unsigned short DataLevelThreshold7;
    unsigned short DataLevelThreshold8;
    unsigned short DataLevelThreshold9;
    unsigned short DataLevelThreshold10;
    unsigned short DataLevelThreshold11;
    unsigned short DataLevelThreshold12;
    unsigned short DataLevelThreshold13;
    unsigned short DataLevelThreshold14;
    unsigned short DataLevelThreshold15;
    unsigned short DataLevelThreshold16;
    unsigned short ProductDependent4;
    unsigned short ProductDependent5;
    unsigned short ProductDependent6;
    unsigned short ProductDependent7;
    unsigned short ProductDependent8;
    unsigned short ProductDependent9;
    unsigned short ProductDependent10;
    unsigned short MapsNumber;    //底图数据中的块数；或者产品的版本号，原始产品为0；
    //1=SpotBlank ON, 2=SpotBlank OFF
    int            SymbologyOffset;//符号数据偏移
    int            GraphicOffset;  //图形数据偏移
    int            TabularOffset;  //表格数据偏移
} DependentProductMSG;
//产品符号
typedef struct _ProductSymbology_
{
    short BlockDivider;//分隔符
    short BlockID;     //数据块ID
    int   ProductSymbologyBlockLenth;//产品系统描述块长度
    short LayersNumber;  //数据块个数
    short LayerDivider;  //分隔符
    int   DataLayerLenth;//数据块长度
} ProductSymbology;
////////////////////////图形数据格式//////////////////////////////

//图形数据
typedef struct _GraphicAlphanumeric_
{
    short BlockDivider;  //分隔符
    unsigned short GraphicAlphanumericBlockID; //数据块ID
    int            TabularAlphnumericBlockLenth; //数据块长度
    unsigned short GraphicPagesNumber;           //数据块总个数
} GraphicAlphanumeric;
////////////////////////图形数据格式//////////////////////////////

////////////////////////表格数据格式//////////////////////////////
//格式化表格数据
typedef struct _AlphaProductMSGFormatData_
{
    unsigned short BlockDivider;  //分隔符
    unsigned short TabularPagesNumber;//数据的页数
    unsigned short CharactersNumber;  //每页的字符个数
    char           *PageDataall;
    unsigned short FlagEndpage;      //结束标示位
} AlphaProductMSGFormatData;

//表格数据
typedef struct _TabularAlphnumeric_
{
    unsigned short TabularBlockDivider;//分隔符
    unsigned short TabularAlphnumericBlockID;//数据块ID
    int            TabularAlphnumericBlockLenth;//数据块长度
    MessageHeader  SecondHeader;   //雷达信息头
    AlphaProductMSGFormatData AlphaProductData;//alpha产品信息描述
} TabularAlphnumeric;
////////////////////////表格数据格式//////////////////////////////

#pragma pack(pop)

#endif // PRIMARYDATA_STRUCT_H
