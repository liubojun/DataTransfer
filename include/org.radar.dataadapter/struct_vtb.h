#ifndef VTBSTRUCT_H_
#define VTBSTRUCT_H_

//标准VTB，用于程序内部处理

#pragma pack(push,1)

#define MAXLAYER 32		                //可以容纳的最大体扫层数 32层
#define LINEDATANUMBER (MAXLAYER*360)   //最大径向记录数,1度分辨率

#define MAXBINDOTS  1000                //最大库数
#define PMAXBINDOTS 1000
//#define PREFILLZ    255			        //强度预设值
//#define PREFILLV    -128		        //速度预设值
//#define PREFILLW    0			        //谱宽预设值
//#define PREFILLDP   -128		        //偏振量预设值
//#define PREFILLKDP  -128		        //

#define PREFILLZ    0			        //强度预设值
#define PREFILLV    0					//速度预设值
#define PREFILLW    0			        //谱宽预设值
#define PREFILLDP   0					//偏振量预设值
#define PREFILLKDP  0					//

#define MAXZDR   15.0f			        //差分反射率预设值
#define MAXPHDP  180.0f			        //差分相移预设值
#define MAXLDRH  35.0f			        //退极化比预设值
#define MAXROHV  1.0f			        //相关系数预设值
#define MAXKDP   10.0f

//const unsigned char PREFILLVALUE = 255;	///< 预设值
// 组网的新算法，在使用255为预设值时，有许多大的孤立值不好过滤，所以使用0。Fanghua Guo 2016-12-27
const unsigned char PREFILLVALUE = 0;		///< 预设值

const short S_FillValue = -32768;			///< short型预设值

//产品中保存的为归一化值，变为真值后应该时=(V+addV)*mulV new
// #define ADDV    0
// #define MULV    0.01f
#define  ADDV	-64
#define  MULV   0.5f

typedef	struct
{
    char Country[30];                //国家名，文本格式输入
    char Province[20];               //省名，文本格式输入
    char Station[40];                //站名，文本格式输入
    char StationNumber[10];          //区站号，文本格式输入
    char RadarType[20];              //雷达型号，文本格式输入
    char Longitude[16];              //天线所在经纬度，文本格式输入
    char Latitude[16];               //天线所在纬度，文本格式输入
    int  LongitudeValue;             //天线所在经度的数值，以1/1000度为计数单位
    //东经（E）为正，西经（W）为负
    int  LatitudeValue;              //天线所在纬度的数值，以1/1000度为计数单位
    //北纬（N）为正，南纬（S）为负
    int  Height;                     //天线海拔高度，以毫米为计数单位
    short MaxAngle;                  //测站周围地物最大遮挡仰角，以1/100度为计数单位
    short OptiAngle;                 //测站的最佳观测仰角（地物回波强度<10dBZ），
    //以1/100度为计数单位
} RADARSITE;

typedef struct
{
    int AntennaG;                   //天线增益，以0.001dBZ为计数单位
    unsigned short VerBeamW;         //垂直波束宽度，以1/100度为计数单位
    unsigned short HorBeamW;         //水平波束宽度，以1/100度为计数单位
    unsigned char Polarizations;     //偏振情况
    //0=水平
    //1=垂直
    //2=双线偏振
    //3=园偏振
    //4=其他
    unsigned short SideLobe;         //第一旁瓣，以0.01dBZ为计数单位
    int Power;                      //雷达脉冲峰值功率，以瓦为单位
    int WaveLength;                 //波长，以微米为计数单位
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
    //1=地物杂波扣除法
    //2=地物杂波+滤波器处理
    //3=滤波器处理
    //4=谱分析处理
    //5=其他处理法
    unsigned char NoiseT;            //噪声消除阈值（0-255）
    unsigned char SQIT;              //SQI阈值，以0.01为计数单位
    unsigned char IntensityC;        //RVP强度值估算采用通道
    //1=对数通道
    //2=线性通道
    unsigned char IntensityR;        //强度估算是否进行了距离订正
    //0=无
    //1=以进行了距离订正
} RADARPERFORMANCEPARAM;

typedef struct
{
    unsigned char DataType;          //本层观测要素
    //1=单强度
    //2=三要素 单PRF
    //3=三要素 双PRF
    //4=双线偏振
    //5=双线偏振多普勒
    //6=双波长（不共天线）
    //7=双波长（共用天线）
    //8=单PRF:V+W
    unsigned char Ambiguousp;        //本层退数度模糊状态
    //0=无数速度腿模糊状态
    //1=软件退数度模糊
    //2=双T退速度模糊
    //3=批式退速度模糊
    //4=双T+软件退速度模糊
    //5=批式+软件退速度模糊
    //6=双PPI退速度模糊
    //9=其他方式
    unsigned short DPbinNumber;
    //原始基数据中
    //本层天线转速，计数单位为0.01度/秒，当扫描方式为
    //RHI或PPI时，只在第一个元素中填写，其他为0
    //预处理基数据中
    //本层代表偏振量径向的库数

    unsigned short PRF1;             //本层第一脉冲重复频率，计数单位为1/10Hz
    unsigned short PRF2;             //本层第二脉冲重复频率，计数单位为1/10Hz
    unsigned short PluseW;           //本层的脉冲宽度，计数单位为微秒
    unsigned short MaxV;             //本层的最大可测速度，计数单位为厘米/秒
    unsigned short MaxL;             //本层的最大可测距离，以10米为计数单位
    unsigned short ZBinWidth;        //本层强度数据的库长，以1/10米为计数单位
    unsigned short VBinWidth;        //本层速度数据的库长，以1/10米为计数单位
    unsigned short WBinWidth;        //本层谱宽数据的库长，以1/10米为计数单位
    unsigned short ZbinNumber;       //本层扫描强度径向的库数
    unsigned short VbinNumber;       //本层扫描速度径向的库数
    unsigned short WbinNumber;       //本层扫描谱宽径向的库数
    unsigned short RecordNumber;     //本层扫描径向个数
    short SwpAngles;                 //本层的仰角，计数单位为1/100度，当扫描方式为RHI。不填此数组，当扫描方式为PPI时，
    //第一个元素为做PPI时的仰角，计数单位为1/100度，其它元素填写-32768
    char DataForm;                   //本层径向中的数据排列方式：
    //11 单要素排列 ConZ
    //12 单要素排列 UnZ
    //13 单要素排列 V
    //14 单要素排列 W
    //21 按要素排列 ConZ+UnZ
    //22 按要素排列 ConZ+V+W
    //23 按要素排列 UnZ+V+W
    //24 按要素排列 ConZ+UnZ+V+W
    //25 按要素排列 V+W
    //5x 双偏振多普勒按要素排列模式(不含UnZ)
    //51 ConZ+V+W+Zdr
    //52 ConZ+V+W+Ldr
    //53 ConZ+V+W+Zdr+PHdp+Kdp+ROhv
    //54 ConZ+V+W+PHdp+Kdp+ROhv+Ldr
    //55 ConZ+V+W+Zdr+PHdp+Kdp+ROhv+Ldr
    //6x 双偏振多普勒按要素排列模式
    //61 ConZ+UnZ+V+W+Zdr
    //62 ConZ+UnZ+V+W+Ldr
    //63 ConZ+UnZ+V+W+Zdr+PHdp+Kdp+ROhv
    //64 ConZ+UnZ+V+W+PHdp+Kdp+ROhv+Ldr
    //65 ConZ+UnZ+V+W+Zdr+PHdp+Kdp+ROhv+Ldr
    unsigned int DBegin;            //本层数据记录开始位置（字节数）
} LAYERPARAM;

typedef struct
{
    short Code;                      //变库长结构代码
    short Begin;                     //开始库的距离，以10米为计数单位
    short End;                       //结束库的距离，以10米为计数单位
    short BinLength;                 //库长，以1/10米为计数单位
} BINPARAM;

typedef struct
{
    unsigned char SType;             //扫描方式
    //1=RHI
    //10=PPI
    //1XX=VOL，XX为体扫层数
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
    unsigned int SMillisecond;      //秒的小数位（计数单位微秒）
    unsigned char Calibration;       //标校状态
    //0=无标校
    //1=自动标校
    //2=一星期内人工标校
    //3=一月内人工标校
    //其他码不用
    unsigned char IntensityI;        //强度积分次数（32-128）
    unsigned char VelocityP;         //速度处理样本（31-255）（样本数减一）
    unsigned short ZStartBin;        //强度有效数据开始库
    unsigned short VStartBin;        //速度有效数据开始库
    unsigned short WStartBin;        //谱宽有效数据开始库
    LAYERPARAM LayerInfo[32];        //层参数结构（各层扫描状态设置）
    unsigned short RHIA;             //做RHI时的所在方位角，计数单位为1/100度，做PPI和立体扫描时为65535
    short RHIL;                      //做RHI时的最低仰角，计数单位为1/100度，做其他扫描时为-32768
    short RHIH;                      //做RHI时的最高仰角，计数单位为1/100度，做其他扫描时为-32768
    unsigned short EYear;            //观测记录结束时间的年（2000-）
    unsigned char EMonth;            //观测记录结束时间的月（1-12）
    unsigned char EDay;              //观测记录结束时间的日（1-31）
    unsigned char EHour;             //观测记录结束时间的时（00-23）
    unsigned char EMinute;           //观测记录结束时间的分（00-59）
    unsigned char ESecond;           //观测记录结束时间的秒（00-59）
    unsigned char ETenth;            //观测记录结束时间的1/100秒（00-99）
    unsigned short ZBinByte;         //原始强度数据中库长无变化填0
    //原始强度数据中库长有变化填占用字节数
    BINPARAM BinRange1[5];           //5个8字节（强度库长无变化为空字节）
    unsigned short VBinByte;         //原始速度数据中库长无变化填0
    //原始速度数据中库长有变化填占用字节数
    BINPARAM BinRange2[5];           //5个8字节（速度库长无变化为空字节）
    unsigned short WBinByte;         //原始谱宽数据中库长无变化填0
    //原始谱宽数据中库长有变化填占用字节数
    BINPARAM BinRange3[5];           //5个8字节（谱宽库长无变化为空字节）
} RADAROBSERVATIONPARAM;

typedef struct
{
    char StationID[2];               //台站代码
    char Spare[560];                 //备用字节560个//在内部数据格式中，保存原始数据名
} RADAROTHERINFORMATION;

typedef struct
{
    char FileID[4];                 //雷达数据标识（原始数据标识符，‘RD’为雷达原
    //始数据，‘GD‘为雷达衍生数据等等）
    float VersionNo;                //表示数据格式的版本号
    //如果是预处理过的基数据，则版本号为(-2.5)
    int FileHeaderLength;           //表示文件头的长度
    RADARSITE SiteInfo;             //站址基本情况
    RADARPERFORMANCEPARAM PerformanceInfo;  //性能参数
    RADAROBSERVATIONPARAM ObservationInfo;  //观测参数
    RADAROTHERINFORMATION OtherInfo;        //其他信息参数
} RadarDataHead;

typedef struct
{
    short Elev;                     //仰角，计数单位1/100度
    unsigned short Az;              //方位，计数单位1/100度
    unsigned char Hh;               //时
    unsigned char Mm;               //分
    unsigned char Ss;               //秒
    unsigned int Min;               //秒的小数
} LINEDATAINFO;

typedef struct
{
    LINEDATAINFO LineDataInfo;
    unsigned char CorZ[MAXBINDOTS];      //经过地物杂波消除的dBZ值=（CorZ-64）/2
    unsigned char UnZ[MAXBINDOTS];       //不经过地物杂波消除的dBZ值=（UnZ-64）/2
    char          V[MAXBINDOTS];         //速度值，计数单位为最大可测速度的1/127  （MaxV/12700 m/s）
    //正值表示远离雷达的速度，负值表示朝向雷达的速度
    //无回波时计-128
    unsigned char W[MAXBINDOTS];         //谱宽值，计数单位为最大可测速度的1/512
    //无回波时计零
    char  ZDR[MAXBINDOTS];    			// 反射率差值
    char  PHDP[MAXBINDOTS];				// 相移差值
    char  KDP[MAXBINDOTS];				// 没有
    char  LDRH[MAXBINDOTS];				// 退极化比值(只记水平)
    char  ROHV[MAXBINDOTS];				// 相关系数值

} LineDataBlock;

typedef struct
{
    LINEDATAINFO LineDataInfo;
    unsigned char CorZ[MAXBINDOTS];      //经过地物杂波消除的dBZ值=（CorZ-64）/2
    unsigned char UnZ[MAXBINDOTS];       //不经过地物杂波消除的dBZ值=（UnZ-64）/2
    char          V[MAXBINDOTS];         //速度值，计数单位为最大可测速度的1/127
    //正值表示远离雷达的速度，负值表示朝向雷达的速度
    //无回波时计-128
    unsigned char W[MAXBINDOTS];         //谱宽值，计数单位为最大可测速度的1/512
    //无回波时计零
} StandardLineDataBlock;

#pragma pack(pop)

#endif /*INTERVTB_H_*/
