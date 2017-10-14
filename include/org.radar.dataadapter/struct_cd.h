#ifndef STRUCT_CDSTRUCT
#define STRUCT_CDSTRUCT

#pragma pack(push, 1)

#define RAWRECORDLENGTH 998

typedef struct
{
    char Country[30]; // 国家名，文本格式输入
    char Province[20]; // 省名，文本格式输入
    char Station[40]; // 站名，文本格式输入
    char Stationnumber[10]; // 区站号，文本格式输入
    char Radartype[20]; // 雷达型号，文本格式输入
    char Longitude[16]; // 天线所在经度，文本格式输入.书写格式例：E 115°32	′12″
    char Latitude[16]; // 天线所在纬度，文本格式输入.书写格式例：N 35°30	′15″
    long int Longitudevalue; // 天线所在经度的数值，以1/100 度为计数单位（十进制）东经（E）为正，西经（W）为负
    long int Lantitudevalue; // 天线所在纬度的数值，以1/100 度为计数单位（十进制）北纬（N）为正，南纬（S）为负
    long int Height; // 天线的海拔高度以毫米为计数单位
    short Maxangle; // 测站四周地物阻挡的最大仰角（以1/100 度为计数单位）
    short Opangle; // 测站的最佳观测仰角（地物回波强度<10dBz，以1/100度为计数单位）
    short MangFreq; // 速调管频率（通过此频率可计算雷达波长）
} CD_RADARSITE;

typedef struct
{
    long int AntennaG;//天线增益，以0.001dB 为计数单位
    unsigned short BeamH;//垂直波束宽度，以1/100 度为计数单位
    unsigned short BeamL;//水平波束宽度，以1/100 度为计数单位
    unsigned char Polarizations;//极化状况0 = 水平1 = 垂直2 = 双偏振3 = 圆偏振4 = 其他
    char Sidelobe;//第一旁瓣计数单位：dB（注意：输入负号）
    long int Power;//雷达脉冲峰值功率，以瓦为计数单位
    long int Wavelength;//波长，以微米为计数单位
    unsigned short LogA;//对数接收机动态范围,以0.01dB 为计数单位
    unsigned short LineA;//线性接收机动态范围,以0.01 为计数单位
    unsigned short AGCP;//AGC 延迟量，以微秒为计数单位
    unsigned char ClutterT;//杂波消除阀值，计数单位0.01dB
    unsigned char VelocityP;//速度处理方式0 = 无速度处理1 = PPP2 = FFT
    unsigned char FilderP;//地物消除方式
    //0 = 无地物消除
    //1 = 地物杂波图扣除法
    //2 = 地物杂波图 + 滤波器处理
    //3 = 滤波器处理
    //4 = 谱分析处理
    unsigned char NoiseT;//噪声消除阀值 （0-255）
    unsigned char SQIT;//SQI 阀值，以厘米/秒为计数单位
    unsigned char IntensityC;//rvp 强度值估算采用的通道1 = 对数通道2 = 线性通道
    unsigned char IntensityR;//强度估算是否进行了距离订正0 = 无1 = 已进行了距离订正
} CD_RADARPERFORMANCEPARAM;

typedef struct
{
    unsigned char Ambiguousp;//本层退模糊状态
    //0 = 无退模糊状态
    //1 = 软件退模糊
    //2 = 双T 退模糊
    //3 = 批式退模糊
    //4 = 双T + 软件退模糊
    //5 = 批式 + 软件退模糊
    //6 = 双PPI 退模糊
    //9 = 其他方式
    unsigned short Arotate;//本层天线转速,计数单位:0.01 度/秒
    unsigned short Prf1;//本层的第一种脉冲重复频率,计数单位: 1/10 Hz
    unsigned short Prf2;//本层的第二种脉冲重复频率,计数单位: 1/10 Hz
    //(通过重复频率1、重复频率2 和速调管频率可计算最大速度)
    unsigned short SpulseW;//本层的脉冲宽度,计数单位: 微秒
    unsigned short MaxV;//本层的最大可测速度,计数单位: 厘米/秒
    unsigned short MaxL;//本层的最大可测距离，以10 米为计数单位
    unsigned short BinWidth;//本层数据的库长，以分米为计数单位
    unsigned short Binnumber;//本层每个径向的库数
    unsigned short Recordnumber;//本层径向数(记录个数)
    short Swangles;//本层的仰角，计数单位 ：1/100 度
} CD_LAYERPARAM;

typedef struct
{
    unsigned char Stype;//扫描方式
    //1 = RHI
    //10 = PPI
    //1XX = Vol XX为扫描圈数
    unsigned short Syear;//观测记录开始时间的年
    unsigned char Smonth;//观测记录开始时间的月（1-12）
    unsigned char Sday;//观测记录开始时间的日（1-31）
    unsigned char Shour;//观测记录开始时间的时（00-23）
    unsigned char Sminute;//观测记录开始时间的分（00-59）
    unsigned char Ssecond;//观测记录开始时间的秒（00-59）
    unsigned char Timep;//时间来源
    //0 = 计算机时钟，但一天内未进行对时
    //1 = 计算机时钟，但一天内已进行对时
    //2 = GPS
    //3 = 其他
    unsigned long int Smillisecond;//秒的小数位（计数单位微秒）
    unsigned char Calibration;//标校状态
    //0 = 无标校
    //1 = 自动标校
    //2 = 1星期内人工标校
    //3 = 1月内人工标校
    //其他码不用
    unsigned char IntensityI;//强度积分次数（32-128）
    unsigned char VelocityP;//速度处理样本数（31-255）(样本数-1）
    CD_LAYERPARAM LayerParam[30];//各圈扫描状态设置 注：当扫描方式为RHI 或PPI 时，只在第一个元素中填写，其他元素为0。
    unsigned short RHIA;//作RHI 时的所在方位角，计数单位为1/100 度 作PPI 和立体扫描时为65535
    short RHIL;//作RHI 时的最低仰角，计数单位为1/100 度 作其他扫描时为-32768
    short RHIH;//作RHI 时的最高仰角，计数单位为1/100 度 作其他扫描时为-32768
    unsigned short Eyear;//观测结束时间的年
    unsigned char Emonth;//观测结束时间的月（1-12）
    unsigned char Eday;//观测结束时间的日（1-31）
    unsigned char Ehour;//观测结束时间的时（00-23）
    unsigned char Eminute;//观测结束时间的分（00-59）
    unsigned char Esecond;//观测结束时间的秒（00-59）
    unsigned char Etenth;//观测结束时间的1/100 秒（00-59）
} CD_RADAROBSERVATIONPARAM;

typedef struct
{
    CD_RADARSITE  CDRadarSiteInfo;
    CD_RADARPERFORMANCEPARAM  CDRadarPerformanceInfo;
    CD_RADAROBSERVATIONPARAM  CDRadarObservationInfo;
    char CD_Reserved[163];
} CD_RADARDATAFILEHEADER;

typedef struct
{
    unsigned char Dbz;//强度值，实际dBZ=(dbz-64)/2;
    char Vel;//速度值实际Velocity=最大可测速度*(vel-128)/127.5。
    //正值表示远离雷达的速度，负值表示朝向雷达的速度
    //0: 无回波、无速度；  128: 有回波、无速度;
    unsigned char Undbz;//无订正强度值，实际dBZ=(undbz-64)/2;
    unsigned char Sw;//谱宽值，计数单位为最大可测速度的256分之一，无回波时为零
} CD_DATA;

//雷达数据格式I-V-W记录定义
//PPI文件共有360个帧记录（每隔1度1个记录）。
//RHI文件有帧记录数=RHI扫描范围/0.175
//体扫文件帧记录数=360*体扫层数。
//方位计算fz=startaz*360.0/4096.0;
//仰角计算el=startel*120.0/4096.0;
typedef struct
{
    unsigned short	Startaz,Startel,Endaz,Endel;//角度算法为： 实际角度=角度值*360.0/pow(2,16);
    CD_DATA	RawData[RAWRECORDLENGTH];
} CD_DATARECORD;

#pragma pack(pop)

#endif
