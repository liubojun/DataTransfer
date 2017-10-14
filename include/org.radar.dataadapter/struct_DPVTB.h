#ifndef STRUCT_DPVTB_H_
#define STRUCT_DPVTB_H_
//马来格式
#define MAXRADIALDOT	1000 //最大距离库数
#define MAXLAYER    32
#define BLANKDP -32768 //偏振量的空值

#pragma pack(1)

//原始数据文件标识
typedef struct{  //12字节
	char FileID[4];				//雷达数据标识
	float VersionNo;			//数据格式的版本号
	long int FileHeaderLength;	//文件头长度
}DP_RADARDATAHEAD;

//站信息
typedef struct{ //168字节
	char Country[30];			//国家名,文本格式输入
	char Province[20];			//省名,文本格式输入
	char Station[40];			//站名,文本格式输入
	char StationNumber[10];		//区站号,文本格式输入
	char RadarType[20];			//雷达型号,文本格式输入
	char Longitude[16];			//天线所在经度,文本格式输入
	char Latitude[16];			//天线所在纬度,文本格式输入
	long int LongitudeValue;	//天线所在经度的数值
	long int LatitudeValue;		//天线所在纬度的数值
	long int Height;			//天线海拔高度
	short MaxAngle;				//测站四周地物阻挡的最大仰角
	short OptiAngle;			//测站的最佳观测仰角
}DP_RADARSITE;


//性能参数数据
typedef struct{	//36字节
	long int AntennaG;				//天线增益,计数单位0.01dB
	unsigned short VerBeamW;		//垂直波束宽度,计数单位0.01度
	unsigned short HorBeamW;		//水平波束宽度,计数单位0.01度
	unsigned char  Polarizations;	//偏振状况 0=水平 1=垂直 2=双偏振 3=圆偏振 4=其它
	unsigned short SideLobe;		//第一旁瓣,计数单位0.01dB
	long int Power;					//雷达脉冲峰值功率,计数单位:瓦
	long int WaveLength;			//波长,计数单位:微米
	unsigned short LogA;
	unsigned short LineA;
	unsigned short AGCP ;
	unsigned short LogMinPower;
	unsigned short LineMinPower;
	unsigned char ClutterT;
	unsigned char VelocityP;
	unsigned char FilterP;
	unsigned char NoiseT;
	unsigned char SQIT;
	unsigned char IntensityC;
	unsigned char IntersityR;
}DP_RADARPERFORMANCEPARAM;

//层数据结构
typedef struct{  //35W
	unsigned char DataType;
	unsigned char Ambiguousp;
	unsigned short Arotate;
	unsigned short PRF1;    //4620
	unsigned short PRF2;    //0
	unsigned short PulseW;
	unsigned short MaxV;
	unsigned short MaxL;
	unsigned short ZBinWidth;
	unsigned short VBinWidth;
	unsigned short WBinWidth;
	unsigned short ZBinNumber;//1000  667
	unsigned short VBinNumber;
	unsigned short WBinNumber;
	unsigned short RecordNumber;
	short SwpAngles;
	char DataForm;//24 CorZ+UnZ+V+W
				  //60 CorZ+UnZ+V+W+ZDR
	              //61 CorZ+V+W+ZDR+PHDP+LDRH+ROHV          +KDP (KDP不输出)
				  //62 CorZ+V+W+ZDR+PHDP+ROHV               +KDP      (KDP不输出)
	unsigned long int DBegin;
}DP_LAYERPARAM;

//库变化数据结构
typedef struct{   //8W
	short Code;
	short Begin;
	short End;
	short BinLength;
}DP_BINPARAM;

//观测参数数据
typedef struct{  //1279W
	unsigned char SType;
	unsigned short SYear;
	unsigned char SMonth;
	unsigned char SDay;
	unsigned char SHour;
	unsigned char SMinute;
	unsigned char SSecond;
	unsigned char TimP;
	unsigned long int SMillisecond;
	unsigned char Calibration;
	unsigned char IntensityI;
	unsigned char VelocityP;
	unsigned short ZStartBin;
	unsigned short VStartBin;
	unsigned short WStartBin;
	DP_LAYERPARAM LayerInfo[MAXLAYER];
	unsigned short RHIA;
	short RHIL;
	short RHIH;
	unsigned short EYear;
	unsigned char EMonth;
	unsigned char EDay;
	unsigned char EHour;
	unsigned char EMinute;
	unsigned char ESecond;
	unsigned char ETenth;
	unsigned short ZBinByte;
	DP_BINPARAM BinRange1[5];
	unsigned short VBinByte;
	DP_BINPARAM BinRange2[5];
	unsigned short WBinByte;
	DP_BINPARAM BinRange3[5];
}DP_RADAROBSERVATIONPARAM;

//其它信息参数
typedef struct{ //562W
	char StationID[2];
	char Spare[560];
}DP_RADAROTHERINFORMATION;


typedef struct {
	DP_RADARDATAHEAD radflag;
	DP_RADARSITE SiteInfo;             //站址基本情况
	DP_RADARPERFORMANCEPARAM PerformanceInfo;  //性能参数
	DP_RADAROBSERVATIONPARAM ObservationInfo;  //观测参数
	DP_RADAROTHERINFORMATION OtherInfo;        //其他信息参数
}DP_RadarDataHead;

//数据记录块
typedef struct{ //1355W
	short Elev;
	unsigned short Az;
	unsigned char Hh;
	unsigned char Mm;
	unsigned char Ss;
	unsigned long int Min;
	unsigned char CorZ[MAXRADIALDOT];	//抑制
	unsigned char UnZ[MAXRADIALDOT];    //
	char 		  V[MAXRADIALDOT];               //
	unsigned char W[MAXRADIALDOT];      //
	short 		  ZDR[MAXRADIALDOT];					//(-2 ~ 6)反射率差值 -6      +6                                   范围10     x0.01
	short         PHDP[MAXRADIALDOT];					//相移差值      -180    +180                             范围180    x0.01
	short         KDP[MAXRADIALDOT];					//(-1 ~ 6)没有              -8       8
	short         LDRH[MAXRADIALDOT];					//退极化比值(只记水平)  -33    -10     范围35     x0.01
	short         ROHV[MAXRADIALDOT];					//相关系数值     0       1                                  范围1      x0.001
}DP_LineDataBlock;


#endif /*STRUCT_DPVTB_H_*/
