// 军队717格式雷达数据
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_H__19B591F5_51C3_11D5_9269_00C026AB1C9B__INCLUDED_)
#define AFX_DATA_H__19B591F5_51C3_11D5_9269_00C026AB1C9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma pack(1)
#define NEWRADARHEADERLENGTH 1024
#define RAWRECORDLENGTH 998
#define SINGLERAWRECORDLENGTH 992


typedef struct tagLAYERPARAM717{

	unsigned char Ambiguousp;  //本层退模糊状态
	//0=无退模糊
	//1=软件退模糊
	//2=双T退模糊
	//3=批式退模糊
	//4=双T+软件退模糊
	//5=批式+软件
	//6=双PPI退模糊
	//9=其他方式

	unsigned short Arotate;		//本层天线转速，记数单位：0。01度/秒
	unsigned short Prf1;		//本层的第一种脉冲重复频率，记数单位：1/10HZ
	unsigned short Prf2;		//本层的第二种脉冲重复频率，记数单位：1/10HZ
	unsigned short PulseW;		//本层的脉冲宽度，：微妙
	unsigned short MaxV;		//本层的最大可测速度，：厘米/秒
	unsigned short MaxL;		//本层的最大可测距离，：10米
	unsigned short BinWidth;	//本层数据的库长，：分米
	unsigned short BinNumber;	//本层扫描线水平方向的点数
	unsigned short RecordNumber;//本层扫描线垂直方向的点数
	short Swangles;				//本层的仰角，记数单位：1/1000度
} LAYERPARAM717;

typedef struct tagRADARSITE717{
	char Country[30];			//国家名,文本格式
	char Province[20];			//省,文本
	char Station[40];			//站名，文本
	char StationNumber[10];		//区站号,文本
	char RadarType[20];			//雷达型号，文本
	char Longitude[16];			//天线所在经度，文本格式输入,书写格式例:E 115" 30'12"
	char Latitude[16];			//天线所在纬度，文本格式输入,书写格式例:N 115" 30'12"
	int  LongitudeValue;	//天线所在经度的数值，以毫秒为记数单位(十进制)
	//东经(E)为正,西经(W)为负
	int  LantitudeValue;	//天线所在纬度的数值，以毫秒为记数单位(十进制)
	//北纬为正,南纬为负
	int Height;			//天线的海拔高度以毫米为记数单位
	short MaxAngle;				//测站四周地物阻挡的最大仰角(以秒为记数单位)
	short OpAngle;				//测站的最佳观测仰角(地物回波强度<10dbz,以秒为记数单位
	short MangFreq;				//磁控管频率(通过此频率可计算雷达波长)
}RADARSITE717;

typedef struct tagRADARPERFORMANCEPARAM717{

	int AntennaG;			//天线增益,以0.001db为记数单位

	unsigned short BeamH;		//垂直波束宽度，以微秒为记数单位
	unsigned short BeamL;		//水平波束宽度，以微秒为记数单位
	unsigned char Polarizations;/*极化状况
								0=水平
								1=垂直
								2=双偏振
								3=圆偏振
								4=其他*/
	char SideLobe;				//第一旁瓣记数单位：db(注意：输入负号)
	int Power;				//雷达脉冲峰值功率，以瓦为记数单位

	int WaveLength;		//波长，以微米为记数单位

	unsigned short LogA;		//对数接收机动态范围，以0.01db为记数单位
	unsigned short LineA;		//线性收机动态范围，以0.01db为记数单位

	unsigned short AGCP;		//AGC延迟量,以微秒为记数单位
	unsigned char ClutterT;		//杂波消除域值，记数单位0.01db
	unsigned char VelocityP;	/*速度处理方式
								0=无速度处理
								1=ppp
								2=fft*/
	unsigned char FilterP;		/*地物消除方式
								0=无地物消除
								1=地物杂波图扣除法
								2=地物杂波图+滤波器处理
								3=滤波器处理
								4=扑分析处理*/
	unsigned char NoiseT;		//噪声消除域值(0-255)
	unsigned char SQIT;			//SQI域值,以0.01为记数单位
	unsigned char IntensityC;
	/*rvp强度值估算采用的通道
	1=对数通道
	2=线性通道
	*/
	unsigned char IntensityR;
	/*强度估算是否进行了距离订正
	0=无
	1=以进行了距离订正*/
}RADARPERFORMANCEPARAM717;
typedef struct tagRADAROBSERVATIONPARAM717{
	unsigned char SType;		/*扫描方式
								1=RHI
								10=PPI
								1XX= VOL XX	为扫描圈数*/
	//unsigned char syear1;
	unsigned short SYear;		//观测记录开始时间的年的十位个位(19-20)
	unsigned char SMonth;		//观测记录开始时间的月(1-12)
	unsigned char SDay;			//观测记录开始时间的日(1-31)
	unsigned char SHour;		//观测记录开始时间的时(00-23)
	unsigned char SMinute;		//观测记录开始时间的分(00-59)
	unsigned char SSecond;		//观测记录开始时间的秒(00-59)
	unsigned char TimeP;		/*时间来源
								0=计算机时钟，但一天内未进行对时
								1=计算机时钟，但一天内以进行对时
								2=GPS
								3=其他*/
	unsigned int SMillisecond;//秒的小数位（记数单位微秒)
	unsigned char Calibration;	/*标校状态
								0=无标校
								1=自动标校
								2=1星期内人工标校
								3=1月内人工标校
								其他码不用*/
	unsigned char IntensityI;	//强度积分次数(32-128)
	unsigned char VelocityP;	//速度处理样本数（31-255)(样本数-1)
	//////////////
	LAYERPARAM717 LayerInfo[30];//各圈扫描状态设置
	unsigned short RHIA;		//作RHI时的所在方位角记数单位1/100度
								//作PPI和立体扫描时为65535
	short RHIL;					//作RHI时的最低仰角，记书丹为为1/100度 其他扫描时为-32768
	short RHIH;					//作RHI时的最高仰角，记书丹为为1/100度 其他扫描时为-32768
	unsigned short EYear;		//观测结束时间的年的十位个位(01-99)
	unsigned char EMonth;		//观测结束时间的月(1-12)
	unsigned char EDay;			//观测结束时间的日(1-31)
	unsigned char EHour;		//观测结束时间的时(00-23)
	unsigned char EMinute;		//观测结束时间的分(00-59)
	unsigned char ESecond;		//观测结束时间的秒(00-59)
	unsigned char ETenth;		//观测结束时间的1/100秒(00-59)
}RADAROBSERVATIONPARAM717;

typedef struct tagReserved717{
	char DataSign[30];
	char Reserved[133];
}Reserved717;

// 1024个字节
typedef struct tagNewRadarHeader717{
	struct tagRADARSITE717 SiteInfo;
	struct tagRADARPERFORMANCEPARAM717 PerformanceInfo;
	struct tagRADAROBSERVATIONPARAM717 ObservationInfo;
	struct tagReserved717 ReservedInfo;
}NewHeader717;

typedef struct tagRawData717
{
	/// <summary>
	/// 强度值，计数单位dBZ。有正负号，无回波时记为0.实际dBz=(CorZ-64)/2.0
	/// </summary>
	unsigned char CorZ; 

	/// <summary>
	/// 速度值，计数单位为最大可测速度的127分之一。 正值表示远离雷达的速度，负值表示朝向雷达的速度，无回波记为-128。
	/// </summary>
	unsigned char V;    

	/// <summary>
	/// 无杂伯抑制强度值，计数单位dBZ。   有正负号，无回波时记为0,实际dBz=(uncIntensity-64)/2.0;
	/// </summary>
	unsigned char UnZ; 

	/// <summary>
	/// 谱宽值：计数单位为最大可测速度的512分之一。无回波时为零。
	/// </summary>
	unsigned char W; 
}RawData717;

typedef struct tagDataRecord717
{
	/// <summary>
	/// 开始方位
	/// </summary>
	unsigned short StartAz;
	/// <summary>
	/// 开始仰角
	/// </summary>
	unsigned short StartElev;

	/// <summary>
	/// 结束方位
	/// </summary>
	unsigned short EndAz;

	/// <summary>
	/// 结束仰角
	/// </summary>
	unsigned short EndElev;

	/// <summary>
	/// 径向线上的数据点，每条线上998个数据点
	/// </summary>
	//[MarshalAs(UnmanagedType.ByValArray, SizeConst = 998)]
	RawData717 RawData[998];
}DataRecord717;

#pragma pack()
#endif // !defined(AFX_DATA_H__19B591F5_51C3_11D5_9269_00C026AB1C9B__INCLUDED_)
