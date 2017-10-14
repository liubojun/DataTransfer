#ifndef _720STRUCT_H_
#define _720STRUCT_H_

#pragma pack(1)
  
//站址参数
typedef struct{
	char RadarType[20];                    //雷达型号，文本格式输入[20]
	char StationNumber[4];                 //站号，文本格式输入[4]
	int Height;                            //天线海拔高度，计数单位:0.1m
	short MaxAngle;                        //测站周围地物最大遮挡仰角，以1/100度为计数单位
	unsigned short Range;                  
	short Sazimuth;
	short Eazimuth;
	short OpAngle;                         //测站的最佳观测仰角（地物回波强度<10dBZ）
	                                       //以1/100度为计数单位
	char Longitude[16];						//经度//后加2007.4.6
	char Latitude[16];						//纬度
}RADARSITEAIR;


//性能参数
typedef struct{
	long AntennaG;                   //天线增益，以0.001dBZ为计数单位
	unsigned short BeamuH;           //垂直波束宽度，以1/100度为计数单位
	unsigned short BeamL;            //水平波束宽度，以1/100读为计数单位
	unsigned char Polarizations;     //偏振情况
	                                 //0=水平
	                                 //1=垂直
	                                 //2=双线偏振
	                                 //3=园偏振
	                                 //4=其他
	unsigned short SideLobe;         //第一旁瓣，以0.01dBZ为计数单位
	long Power;                      //雷达脉冲峰值功率，以瓦为单位
	long WaveLength;                 //波长，以微米为计数单位
	unsigned short LogA;             //对数接收机动态范围，以0.01dBZ为计数单位
	unsigned short LineA;            //线性接收机动态范围，以0.01dBZ为计数单位
	unsigned short AGCP;             //AGC延迟量，以微秒为计数单位
	unsigned short LogMinPower;      //对数接收机最小可测功率，计数单位为0.01dBm
	unsigned short LineMinPower;     //线性接收机最小可测功率，计数单位为0.01dBm
	unsigned char ClutterT;          //杂波消除阈值，计数单位为0.01dB
	
	unsigned char VelocityP;         //速度处理方式
	                                 //0=无速度处理
	                                 //1=PPP
	                                 //2=FFT
	unsigned char FilterP;           //地物杂波消除方式
	                                 //0=无地物杂波消除
	                                 //1=地物杂波图扣除法
	                                 //2=地物杂波+滤波器处理
	                                 //3=滤波器处理
	                                 //4=谱分析处理
	unsigned char NoiseT;            //噪声消除阈值（0-255）
	unsigned char SQIT;              //SQI阈值，以0.01为计数单位
	unsigned char IntensityC;        //RVP强度值估算采用通道
	                                 //1=对数通道
	                                 //2=线性通道 
	unsigned char IntensityR;        //强度估算是否进行了距离订正
	                                 //0=无
	                                 //1=以进行了距离订正
	char Reserved[14];
}RADARPERFORMANCEPARAMAIR;


//各层圈的设置
typedef struct{
	unsigned char Ambiguousp;        //本层退数度模糊状态
	                                 //0=无数速度腿模糊状态
	                                 //1=软件退数度模糊
	                                 //2=双T退速度模糊
	                                 //3=批式退速度模糊
	                                 //4=双T+软件退速度模糊
	                                 //5=批式+软件退速度模糊
	                                 //6=双PPI退速度模糊
	                                 //9=其他方式
	unsigned char Arotate;          //本层天线转速，计数单位为: 转/分
	unsigned short PRF1;             //本层第一脉冲重复频率，计数单位为1/10Hz
	unsigned short PRF2;             //本层第二脉冲重复频率，计数单位为1/10Hz
	unsigned short SpluseW;           //本层的脉冲宽度，计数单位为微秒
	unsigned short MaxV;             //本层的最大可测速度，计数单位为厘米/秒
	unsigned short MaxL;             //本层的最大可测距离，以10米为计数单位
	unsigned short ZBinWidth;        //本层强度数据的库长，以1/10米为计数单位
	unsigned short VBinWidth;        //本层速度数据的库长，以1/10米为计数单位
	unsigned short WBinWidth;        //本层谱宽数据的库长，以1/10米为计数单位
	unsigned short ZbinNumber;       //本层扫描强度径向的库数
	unsigned short VbinNumber;       //本层扫描速度径向的库数
	unsigned short WbinNumber;       //本层扫描谱宽径向的库数
	unsigned short RecodeNumber;     //本层扫描径向个数
	short SwpAngles;                 //本层的仰角，计数单位为1/100度
	char DataForm;                   //本层径向中的数据排列方式：
	                                 //11 单要素排列 ConZ
	                                 //12 单要素排列 UnZ
	                                 //13 单要素排列 V
	                                 //14 单要素排列 W
	                                 //21 按要素排列 ConZ+UnZ
	                                 //22 按要素排列 ConZ+V+W
	                                 //23 按要素排列 UnZ+V+W
	                                 //24 按要素排列 ConZ+UnZ+V+W
	                                 //3X 双偏振多普勒雷达按要素排列模式
}LAYERPARAMAIR;


//探测参数
typedef struct{
	unsigned char SType;             //扫描方式
	                                 //1=RHI
	                                 //10=PPI
	                                 //1XX=VOL，XX为层数
	unsigned short SYear;            //观测记录开始时间的年（2000-）
	unsigned char SMonth;            //观测记录开始时间的月（1-12）
	unsigned char SDay;              //观测记录开始时间的日（1-31）
	unsigned char SHour;             //观测记录开始时间的时（00-23）
	unsigned char SMinute;           //观测记录开始时间的分（00-59）
	unsigned char SSecond;           //观测记录开始时间的秒（00-59）
	unsigned char TimeP;             //时间来源
	                                 //0=计算机时钟，但一天内未进行对时
	                                 //1=计算机时钟，一天内已进行对时
	                                 //2=GPS
	                                 //3=其他
	unsigned char Calibration;       //标校状态
	                                 //0=无标校
	                                 //1=自动标校
	                                 //2=一星期内人工标校
	                                 //3=一月内人工标校
	                                 //其他码不用
	unsigned char IntensityI;        //强度积分次数（32-128）
	unsigned char VelocityP;         //速度处理样本（31-255）（样本数减一）
	LAYERPARAMAIR LayerParam[30];//层参数结构（各层扫描状态设置）
	                   //当扫描方式位RHI或PPI时，只在第一个元素中填写，其他元素为0
	unsigned short RHIA;             //做RHI时的所在方位角，计数单位为1/100度，
	                                 //做PPI和立体扫描时为65535
	short RHIL;                      //做RHI时的最低仰角，计数单位为1/100度，
	                                 //做其他扫描时为-32768
	short RHIH;                      //做RHI时的最高仰角，计数单位为1/100度，
	                                 //做其他扫描时为-32768
	char Reserved[13];                //
}RADAROBSERVATIONPARAMAIR;

//原始文件头结构
typedef struct{
	RADARSITEAIR RadarSiteInfo;
	RADARPERFORMANCEPARAMAIR RadarPerformanceInfo;
	RADAROBSERVATIONPARAMAIR RadarObservationInfo;
	char Reserved[24];
}RADARDATAFILEHEADERAIR;

//原始文件数据区
typedef struct{
	short Elevation;                     //仰角，计数单位1/100度 
	unsigned short Azimuth;              //方位，计数单位1/100度
	unsigned char CorZ[1000];      //经过地物杂波消除的dBZ值=（CorZ-64）/2
	unsigned char UnZ[1000];       //不经过地物杂波消除的dBZ值=（UnZ-64）/2
	char V[1000];                  //速度值，计数单位为最大可测速度的1/127
	                               //正值表示远离雷达的速度，负值表示朝向雷达的速度
	                               //无回波时计-128
	unsigned char W[1000];         //谱宽值，计数单位为最大可测速度的1/256
	                               //无回波时计零
}LineDataBlockAIR;

#endif /*720STRUCT_H_*/
