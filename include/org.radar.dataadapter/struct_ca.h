#ifndef struct_ca_H_
#define struct_ca_H_


#define RGates_CA  800				//c BAND 反射率距离库数
#define VGates_CA  1600				//c BAND 速度距离库数
#define WGates_CA  1600				//c BAND 谱宽距离库数

typedef struct _tagBaseDataCA
{
    unsigned short		temp1[7];						//保留
    unsigned short		RadarStatus;					//1 - 表示为雷达数据
    unsigned short		temp2[6];						//保留
    unsigned int		mSeconds;						//径向数据收集时间
    unsigned short		JulianDate;						//从1970/1/1起的日期
    unsigned short		URange;							//不模糊距离
    unsigned short		Az;								//方位角度
    unsigned short		RadialNumber;					//径向数据序号
    unsigned short		RadialStatus;					//径向数据状态
    unsigned short		El;								//仰角
    unsigned short		ElNumber;						//体扫内的仰角编号
    short				RangeToFirstGateOfRef;			//第一个反射率数据表示的实际距离(m)
    short				RangeToFirstGateOfDop;			//第一个多普勒数据表示的实际距离(m)
    unsigned short		GateSizeOfReflectivity;			//反射率数据的距离库长(m)
    unsigned short		GateSizeOfDoppler;				//多普勒数据的距离库长(m)
    unsigned short		GatesNumberOfReflectivity;		//反射率数据的距离库数
    unsigned short		GatesNumberOfDoppler;			//多普勒数据的距离库数
    unsigned short		CutSectorNumber; 				//扇区号
    unsigned int		CalibrationConst; 				//标定常数
    unsigned short		PtrOfReflectivity;				//反射率数据指针
    unsigned short		PtrOfVelocity;					//速度数据指针
    unsigned short		PtrOfSpectrumWidth;				//谱宽数据指针
    unsigned short		ResolutionOfVelocity;			//多普勒速度分辨率
    unsigned short		VcpNumber;						//体扫号
    unsigned short		temp4[4];						//保留
    unsigned short		PtrOfArcReflectivity;			//反射率数据指针
    unsigned short		PtrOfArcVelocity;				//速度数据指针
    unsigned short		PtrOfArcWidth;					//谱宽数据指针
    unsigned short		Nyquist;						//不模糊速度
    unsigned short      temp46;                         //保留
    unsigned short      temp47;                         //保留
    unsigned short      temp48;                         //保留
    unsigned short      CircleTotal;					//仰角数
    char				SiteCode[8];					//雷达站代码
    unsigned char		temp5[22];						//保留
    unsigned char		Echodata[RGates_CA+VGates_CA+WGates_CA];	//129－588 共460字节反射率数据
    unsigned char		temp[4];						//保留
    //129－1508 共1380字节速度数据
    //129－2428 共2300字节谱宽数据
} CaRadialData;

#endif
