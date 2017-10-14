/**
 * struct_cc.h
 * @Author: Fanghua Guo
 * @CreateDate: 2014-5-5
 * @Version 1.0
 * Copyright (C) 2014 NRIET
 */
#ifndef STRUCT_CC_H
#define STRUCT_CC_H
/*CINRAD/CC型天气雷达体扫数据在5分钟之内9个仰角上完成9次扫描，
每一层包涵三个产品即回波强度（Z）、径向速度（V）、速度谱宽(W)，
雷达每一层扫描就有512个径向，每一个径向存储500个Z、500个V、500个W。
因此一个体扫数据总共包涵27个图层。
文件名：
平扫（PPI）：NNNNYYDDHH.MMP
高扫（RHI）：NNNNYYDDHH.MMR
体扫（VPPI）：NNNNYYDDHH.MMV
单库ＦＦＴ：NNNNYYDDHH.MMF
等高PPI（CAPPI）：NNNNYYDDHH.MMC
N：年   Y：月    D：日    H：时   M：分
如：2013051416.26V
数据组成：
整个数据由文件头(1个)和基于极坐标系的原始数据(512个径向)组成。
21505024字节 = 1024字节 + 14*512*1500*2字节
*/

//文件头：
//tagWEATHERRADAR雷达信息的结构，1024个字节
typedef struct tagWEATHERRADAR
{
    char FileType[16];						 //3830数据标识(CINRADC)
    char Country[30];						//国家名
    char Province[20];					        //省名
    char Station[40];                           //站名
    char StationNumber[10];                               //区站号
    char RadarType[20];                                       //雷达型号
    char Longitude[16];					       //天线所在经度
    char Latitude[16];					       //天线所在纬度
    long LongitudeValue;				      //具体经度
    long LatitudeValue;					       //具体纬度
    long Height;						      //天线海拔高度
    short MaxAngle;					     //地物阻挡最大仰角
    short OptAngle;                                             //最佳观测仰角
    unsigned char SYear1;				      //观测开始时间的年千百位(19-20)
    unsigned char SYear2;				      //观测开始时间的年十个位(00-99)
    unsigned char SMonth;                                   //观测开始时间的月(1-12)
    unsigned char SDay;                                         //观测开始时间的日(1-31)
    unsigned char SHour;                                      //观测开始时间的时(0-23)
    unsigned char SMinute;                                  //观测开始时间的分(0-59)
    unsigned char SSecond;                                  //观测开始时间的秒(0-59)
    unsigned char TimeFrom;                             //时间来源 0-计算机时钟(1天内未对时)   1-计算机时钟(1天内已对时)   2-GPS   3-其它
    unsigned char EYear1;				      //观测结束时间的年千百位(19-20)
    unsigned char EYear2;                                   //观测结束时间的年十个位(00-99)
    unsigned char EMonth;                                 //观测结束时间的月(1-12)
    unsigned char EDay;                                      //观测结束时间的日(1-31)
    unsigned char EHour;				   //观测结束时间的时(0-23)
    unsigned char EMinute;				   //观测结束时间的分(0-59)
    unsigned char ESecond;				   //观测结束时间的秒(0-59)
    unsigned char ScanMode;			  //扫描方式  1-RHI   10-PPI和ZPPI   1XX=VPPI(XX为扫描圈数)
    unsigned long SmilliSecond;			  //以微秒为单位表示的秒的小数位
    unsigned short RHIA;				   //RHI所在的方位角(0.01度为单位)，PPI和VPPI时为FFFF
    short RHIL;                                                   //RHI所在的最低仰角(0.01度为单位)，PPI和VPPI时为FFFF
    short RHIH;                                                  //RHI所在的最高仰角(0.01度为单位)，PPI和VPPI时为FFFF
    unsigned short EchoType;                        //回波类型  0x405a-Z  0x406a-V  0x407a-W， 0x408a-ZVW三要素
    unsigned short ProdCode;                       //数据类型  0x8001-PPI数据  0x8002-RHI数据，0x8003-VPPI数据
    //0x8004-单强度PPI数据，0x8005-CAPPI数据
    unsigned char Calibration;			//标校状态  0-无  1-自动  2-1星期内人工   3-1月内人工
    unsigned char remain1[3];			//保留字
    unsigned char remain2[660];             //保留字,放VPPISCANPARAMETER数据，该结构的说明见后。以上有218个字节，22(字节/层)*30(层) = 660(字节)
    long AntennaG;                                            //天线增益(0.001dB)
    long Power;                                                   //峰值功率(瓦)
    long Wavelength;					//波长(微米)
    unsigned short BeamH;			       //垂直波束宽度(秒)
    unsigned short BeamL;                            //水平波束宽度(秒)
    unsigned short Polarization;		      //极化状态 0-水平 1-垂直 2-双偏振  3-圆偏振 4-其它
    unsigned short LogA;			      //对数动态范围(0.01dB)
    unsigned short LineA;			      //线性动态范围(0.01dB)
    unsigned short AGCP;			      //AGC延迟量(微秒)
    unsigned short FreqMode;		     //频率方式	1-单重复频率  2-双重复频率3:2   3-双重复频率4:3
    unsigned short FreqRepeat;		     //重复频率
    unsigned short PPPPulse;	            //PPP脉冲数
    unsigned short FFTPoint;                    //FFT间隔点数
    unsigned short ProcessType;            //信号处理方式	1-PPP	2-全程FFT   3-单库FFT
    unsigned char  ClutterT;			    //杂波消除阀值(即STC)
    char Sidelobe;                                           //第一旁瓣(dB)
    unsigned char VelocityT;			   //速度门限
    unsigned char FilderP;			   //地物消除方式   0-无   1-IIR滤波器1    2-IIR滤波器2   3-IIR滤波器3   4-IIR滤波器4
    unsigned char NoiseT;		          //噪声消除阀值(即强度门限)
    unsigned char SQIT;				  //SQI门限
    unsigned char IntensityC;                   //DVIP强度值估算采用的通道： 1-对数通道 2-线性通道
    unsigned char IntensityR;	         //强度值估算是否距离订正： 0-无(dB) 1-已订正(dBZ)
    unsigned char CalNoise;			 //噪声系数标定值
    unsigned char CalPower;			 //发射功率标定值
    unsigned char CalPulseWidth;         //脉冲宽度标定值
    unsigned char CalWorkFreq;		//工作频率标定值
    unsigned char CalLog;			//对数斜率标定值
    char Remain3[92];				//保留字
    unsigned long int DataOffset;	//数据偏移地址
} WEATHERRADAR;

//tagVPPISCANPARAMETER仰角层的结构，22个字节。 VPPI有几层填几个，PPI  RHI  FFT数据只填写一层即一个．
typedef struct tagVPPISCANPARAMETER
{
    unsigned short MaxV;			//最大可测速度(厘米/秒)
    unsigned short MaxL;			//最大可测距离(10米)
    unsigned short BindWidth;		//库长(米)
    unsigned short BinNumber;            //每径向库数
    unsigned short RecordNumber;     //本圈径向数
    unsigned short Arotate;                   //本圈转速(0.01度/秒)
    unsigned short Prf1;                        //本圈第一次重复频率(0.1Hz)对应单重频或双重频的高者
    unsigned short Prf2;                        //本圈第二次重复频率(0.1Hz)对应双重频的低者
    unsigned short SpulseW;                 //本圈脉宽(微秒)
    short                 Angle;                       //仰角(0.01度)
    unsigned char  SweepStatus;		//1=单要素	2=三要素(单重频)	3=三要素(双重频)
    unsigned char  Ambiguousp;		//0=无软件退模糊	1=软件退模糊
} VPPISCANPARAMETER;

#endif // STRUCT_CC_H
