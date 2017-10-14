#ifndef CRADARHEADER_H_
#define CRADARHEADER_H_

#define PMODE_PPP     (0)       /* Pulse Pair Doppler */
#define PMODE_FFT     (1)       /* FFT Doppler */
#define PMODE_RPHASE  (2)       /* Random Phase Doppler */
#define PMODE_KNMI    (3)       /* KNMI custom LOG clutter filter */
#define PMODE_DPT1    (4)	/* Dual-PRT Type-1 (for low duty cycle) */
#define PMODE_DPT2    (5)	/* Dual-PRT Type-2 (for velocity unfolding) */

#define RAWRECORDLENGTH			998
#define SINGLERAWRECORDLENGTH	992
#define RVP7RECORDLENGTH		4000

//#pragma pack(1)
#pragma pack(push,1)
struct RawBin
{
    unsigned char m_dbz,m_vel,m_undbz,m_sw;
};
//强度值，实际dBZ=(m_dbz-64)/2;   0: 无回波
//实际Velocity=最大可测速度*(m_Vel-128)/128。正值表示远离雷达的速度，负值表示朝向雷达的速度.0: 无回波、无速度；  128: 有回波、无速度;
//无地杂波抑制的强度,实际dBZ=(m_undbz-64)/2;  0:无回波
//

struct RVP7Data
{
    unsigned short startaz,startel,endaz,endel;
    RawBin  RawData[RAWRECORDLENGTH];
};
//角度算法为： 实际方位=startaz*360.0/pow(2,16);//pow(2,16)=65536
//              实际仰角=startel*120.0/pow(2,16);

struct SingleIntensity
{
    unsigned short startaz,startel,endaz,endel;
    char dbz[SINGLERAWRECORDLENGTH];
};

#define NEWRADARHEADERLENGTH	1024   //数据文件文件头长度
struct _LAYERPARAM
{
    unsigned char  ambiguousp; 	//本层退模糊状态
    //	 0 = 无退模糊状态
    //	 1 = 软件退模糊
    //	 2 = 双T退模糊
    //	 3 = 批式退模糊
    //	 4 = 双T + 软件退模糊
    //	 5 = 批式 + 软件退模糊
    //	 6 = 双PPI退模糊
    //	 9 = 其他方式
    unsigned short	 Arotate;	//	本层天线转速,计数单位:0.01度/秒
    unsigned short	 Prf1;		//	本层的第一种脉冲重复频率,计数单位: 1/10 Hz
    unsigned short	 Prf2;		//	本层的第二种脉冲重复频率,计数单位: 1/10 Hz(通过重复频率1、重复频率2和速调管频率可计算最大速度：
    //'if(Prf2==0|)    Vmax=30000.0*Prf1/MangFreq*400.0
    //'Else
    //'Vmax = 30000# * Prf1 * Prf2 / MangFreq * 400# * Abs(Prf2 - Prf1)
    unsigned short	 spulseW;	//	本层的脉冲宽度,计数单位:	微秒
    unsigned short	 MaxV;		//	本层的最大可测速度,计数单位:	厘米/秒
    unsigned short	 MaxL;		//	本层的最大可测距离，以10米为计数单位
    unsigned short	 binWidth;	//	 本层数据的库长，以分米为计数单位
    unsigned short	 binnumber;	//	本层扫描线水平方向的点数(每层每个径向的库数)
    unsigned short	 recordnumber;//	 本层扫描线垂直方向的点数(每层记录的径向数)
    short	 Swangles;//		本层的仰角，计数单位	：1/100度
};

struct _RADARSITE
{
    char   country[30];//			国家名，文本格式输入
    char   province[20];//			省名，文本格式输入
    char   station[40];//			站名，文本格式输入
    char   stationnumber[10];//	区站号，文本格式输入
    char   radartype[20];//		雷达型号，文本格式输入
    char   longitude[16];//		天线所在经度，文本格式输入.书写格式例：E 115°32′12″
    char   latitude[16]	;//		天线所在纬度，文本格式输入.书写格式例：N 35°30′15″
    long int  longitudevalue;//	    天线所在经度的数值，以毫秒为计数单位（十进制）
    //	东经（E）为正，西经（W）为负
    long int  lantitudevalue;//		天线所在纬度的数值，以毫秒为计数单位（十进制）
    //	北纬（N）为正，南纬（S）为负
    long int  height;	//			天线的海拔高度以毫米为计数单位
    short   Maxangle;	//			测站四周地物阻挡的最大仰角（以秒为计数单位）
    short   Opangle;	//			测站的最佳观测仰角（地物回波强度<10dbz，以秒为											计数单位）
    short   MangFreq;	//			磁控管频率（通过此频率可计算雷达波长）
};

struct _RADARPERFORMANCEPARAM
{
    long  int	 AntennaG;//		天线增益，以0.001db为计数单位
    unsigned short  BeamH;//		垂直波束宽度，以微秒为计数单位
    unsigned short  BeamL;// 		水平波束宽度，以微秒为计数单位
    unsigned char  polarizations;/*	极化状况
								 0 = 水平
 								 1 = 垂直
								 2 = 双偏振
								 3 = 圆偏振
								 4 = 其他*/
    char   sidelobe;//				第一旁瓣计数单位：db（注意：输入负号）
    long int  Power;//			雷达脉冲峰值功率，以瓦为计数单位
    long int  wavelength;//		波长，以微米为计数单位
    unsigned short  logA;//		对数接收机动态范围,以0.01db为计数单位
    unsigned short  LineA;//		线性接收机动态范围,以0.01为计数单位
    unsigned short  AGCP;//		AGC延迟量，以微秒为计数单位
    unsigned char 	 clutterT;//		杂波消除阀值，计数单位0.01db
    unsigned char 	 VelocityP;/*	速度处理方式
	   							 0 = 无速度处理
								 1 = PPP
								 2 = FFT
								 3 = 随机编码*/
    unsigned char 	 filderP;/*		地物消除方式
								 0 = 无地物消除
								 1 = 地物杂波图扣除法
								 2 = 地物杂波图 + 滤波器处理
								 3 = 滤波器处理
								 4 = 谱分析处理*/
    unsigned char	 noiseT;//		噪声消除阀值	（0-255）
    unsigned char 	 SQIT;//		SQI阀值，以0.01为计数单位
    unsigned char 	 intensityC;/*	rvp强度值估算采用的通道
								 1 = 对数通道
								 2 = 线性通道*/
    unsigned char	intensityR;/*	强度估算是否进行了距离订正
								 0 = 无
							 	 1 = 已进行了距离订正*/
};


struct _RADAROBSERVATIONPARAM
{
    unsigned char 	 stype;/*		扫描方式
							 1 = RHI
							 10 = PPI
							 1XX = Vol 	XX为扫描圈数*/
//unsigned char 	 syear1;//		观测记录开始时间的年的千位百位（19-20）
    unsigned short 	 syear;//		观测记录开始时间的年的十位个位（01-99）
    unsigned char 	 smonth;//		观测记录开始时间的月（1-12）
    unsigned char 	 sday;//		观测记录开始时间的日（1-31）
    unsigned char 	 shour;//		观测记录开始时间的时（00-23）
    unsigned char 	 sminute;//		观测记录开始时间的分（00-59）
    unsigned char 	 ssecond;//		观测记录开始时间的秒（00-59）
    unsigned char 	 Timep;/*		时间来源
							 0 = 计算机时钟，但一天内未进行对时
							 1 = 计算机时钟，但一天内已进行对时
							 2 = GPS
							 3 = 其他*/
    unsigned long int smillisecond;// 		秒的小数位（计数单位微秒）
    unsigned char 	 calibration;/*	标校状态
							 0 = 无标校
							 1 = 自动标校
							 2 = 1星期内人工标校
							 3 = 1月内人工标校
							其他码不用*/
    unsigned char 	 intensityI;//		强度积分次数（32-128）
    unsigned char 	 VelocityP;//	速度处理样本数（31-255）(样本数-1）
    struct _LAYERPARAM LayerParam[30]; //各圈扫描状态设置
//注：当扫描方式为RHI或PPI时，只在第一个元素中填写，其他元素为0。
//作PPI和立体扫描时为65535
    unsigned short	 RHIA;//		作RHI时的所在方位角，计数单位为1/100度
    //			作PPI和立体扫描时不用
    short	 RHIL;//				作RHI时的最低仰角，计数单位为1/100度
    //					作其他扫描时不用
    short	 RHIH;//				作RHI时的最高仰角，计数单位为1/100度
    //				作其他扫描时不用
//unsigned char 	 Eyear1;//		观测结束时间的年的千位百位（19-20）
    unsigned short 	 Eyear;//		观测结束时间的年的十位个位（01-99）
    unsigned char 	 Emonth;//		观测结束时间的月（1-12）
    unsigned char 	 Eday;//		观测结束时间的日（1-31）
    unsigned char 	 Ehour;//		观测结束时间的时（00-23）
    unsigned char 	 Eminute;//		观测结束时间的分（00-59）
    unsigned char 	 Esecond;//		观测结束时间的秒（00-59）
    unsigned char 	 Etenth;//		观测结束时间的1/100秒（00-59）
};

struct NewRadarHeader
{
    struct _RADARSITE  SiteInfo;
    struct _RADARPERFORMANCEPARAM  PerformanceInfo;
    struct _RADAROBSERVATIONPARAM  ObservationInfo;
    char Reserved[163];
};
struct NewRadarHeader *OnepData;

#pragma pack(pop)

#endif
