/**
 * CInitBaseinfo.h
 * @Author: Jackie Chen
 * @CreateDate: 2013-4-22
 * @Version 1.0
 * Copyright (C) 2013 NRIET
 */
#ifndef PACKETDATA_STRUCT_H
#define PACKETDATA_STRUCT_H
#include <vector>

/////////////////////Wind Bard data Packet////////////////
typedef struct _WindBarbPacket_0004_
{
    unsigned short PacketCodeType;  //type = 4
    unsigned short PacketBlockDataLenth;//BYTE
    unsigned short WindBarbValue;
    short          x;                  //x坐标
    short          y;                  //y坐标
    short          DirectionWind;      //风向
    short          WindSpeed;          //风速
} WindBarbPacket_0004;
/////////////////////Wind Bard data Packet////////////////
/////////////////////GraphicTextInfo data Packet////////////////
typedef struct _GraphicTextInfo_0008_
{
    unsigned short PacketCodeType;  //type = 8
    short PacketBlockDataLenth;     //BYTES长度不包含PacketCodeType的长度
    short TextValue;                //文本颜色值
    short x;                        //画图的坐标
    short y;
    char  szDataInfo[100];            //风向图的描述信息,信息的长度是PacketBlockDataLenth - 6
} GraphicTextInfo_0008;
/////////////////////GraphicTextInfo data Packet////////////////
/////////////////////AF1F data Packet////////////////
typedef struct _RadialsData_
{
    //一个short中拥有两对数据，前后颠倒是因为字节序的问题
    unsigned short ColorCode:4;
    unsigned short Run:4;
    unsigned short ColorCode1:4;
    unsigned short Run1:4;
} RadialsData;
typedef struct _RadialsData_AF1F_
{
    unsigned short NumberRLERadials;	//编号
    short StartRadialAngle;				//开始角度
    short DeltaRangleRadial;			//delta角度
    std::vector<RadialsData> pData;					//数据
    bool operator < (_RadialsData_AF1F_ &af1f)
    {
        return StartRadialAngle < af1f.StartRadialAngle;
    }
} RadialsData_AF1F;
typedef struct _DisplayDataPackets_AF1F_
{
    unsigned short ProductCode;  //产品类型
    short FirsrtIndexRange;      //起始径向数据
    short NumberRange;           //径向数据的个数(本层扫描强度径向的库数)
    short I_Center_Sweep;        //扫描中心点
    short J_Center_Sweep;
    short ScaleFactor;          //每个径向数据的像素间隔
    short NumberRadials;        //径向数据的数目
} DisplayDataPackets_AF1F;
/////////////////////AF1F data Packet////////////////
/////////////////////BA07/BA0F data Packet////////////////
typedef struct _RasterDataDecrip_
{
    short  NumberBytesRow;  //数据个数
    //QList<RadialsData>  pdatalst;
    std::vector<RadialsData> pdata;     //数据
} RasterDataDecrip;
typedef struct _RasterDataPackets_BA0F_
{
    unsigned short ProductCode;    //产品编号
    unsigned short ProductType;    //产品类型
    unsigned short ProductType1;   //产品类型
    short          Start_I_Data;   //开始数据
    short          Start_J_Data;   //开始数据
    short          Scale_X;        //扫描X
    short           ScaleFractional_X;
    short          Scale_Y;
    short           ScaleFractional_Y;
    short          NumberRows;     //数据总组数
    short          DescripPacket;
} RasterDataPackets_BA0F;
/////////////////////BA07/BA0F data Packet////////////////
#endif // PACKETDATA_STRUCT_H
