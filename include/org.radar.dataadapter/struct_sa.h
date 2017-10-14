#ifndef STRUCT_SA_H
#define STRUCT_SA_H

//#pragma pack(1)
#pragma pack(push,1)
//Orb: Define data format for all possable kind of radar base data and common basedata shared in this system.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//tagBaseData 98D雷达信息结构（目标结构）
typedef struct tagSaBaseData
{
    //雷达信息头（28字节）
    short temp1[7];						//1~14保留
    unsigned short RadarStatus;         //15~16, 1 - 表示为雷达数据
    short temp2[6];						//保留
    unsigned int   mSeconds;			//29~32径向数据收集时间
    unsigned short JulianDate;			//33~34从1970/1/1起的日期
    unsigned short URange;              //35~36不模糊距离（表示：数值/10.=千米）
    unsigned short Az;                  //37~38方位角度（编码方式：[数值/8.]*[180./4096.]=度）
    unsigned short RadialNumber;		//39~40当前仰角内径向数据序号
    unsigned short RadialStatus;		//41~42径向数据状态
    //0：该仰角的第一条径向数据
    //1：该仰角中间的径向数据
    //2：该仰角的最后一条径向数据
    //3：体扫开始的第一条径向数据
    //4：体扫结束的最后一条径向数据

    unsigned short El;                                          //43~44仰角
    unsigned short ElNumber;                                    //45~46体扫内的仰角编号
    unsigned short RangeToFirstGateOfReflectivity;              //47~48第一个反射率数据表示的实际距离
    unsigned short RangeToFirstGateOfDoppler;                   //49~50第一个多普勒数据表示的实际距离
    unsigned short GateSizeOfReflectivity;                      //51~52反射率数据的距离库长
    unsigned short GateSizeOfDoppler;                           //53~54多普勒数据的距离库长
    unsigned short GatesNumberOfReflectivity;                   //55~56反射率数据的距离库数
    unsigned short GatesNumberOfDoppler;                        //57~58多普勒数据的距离库数
    short temp3[3];                                             //保留
    unsigned short PtrOfReflectivity;                           //65~66反射率数据指针（偏离雷达数据信息头的字节数）
    //表示第一个反射率数据的位置
    unsigned short PtrOfVelocity;                               //67~68速度数据指针
    unsigned short PtrOfWidth;                                  //69~70谱宽数据指针
    short ResolutionOfVelocity;                                 //71~72多普勒速度分辨率,2表示0.5米/秒, 4表示1.0米/秒
    unsigned short VcpNumber;                                   //73~74体扫号
    short temp4[4];                                             //保留
    unsigned short PtrOfArcReflectivity;                        //83~84用于回放的反射率数据指针
    unsigned short PtrOfArcVelocity;                            //85~86用于回放的速度数据指针
    unsigned short PtrOfArcWidth;                               //87~88用于回放的谱宽数据指针
    unsigned short Nyquist;                                     //89~90不模糊速度
    short temp46;                                               //保留
    short temp47;                                               //保留
    short temp48;                                               //保留
    short CircleTotal;                                          //97~98仰角数
    unsigned char temp5[30];                                    //保留
    unsigned char Echodata[2304];                               //129－588 共460字节反射率数据
    //129－1508 共1380字节速度数据
    //129－2428 共2300字节谱宽数据
} SaRadialData;

#pragma pack(pop)
#endif // STRUCT_SA_H
