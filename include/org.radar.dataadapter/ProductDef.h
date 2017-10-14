#ifndef PRODUCTDEF_H_
#define PRODUCTDEF_H_

//产品结构中，需要使用标准VTB结构
#include "struct_vtb.h"
#include <vector>
using namespace std;

#pragma pack(push,1)
//POLAR PRODUCT
typedef struct
{
    unsigned char NStep;             //体扫层次
    unsigned char LayerNumber;       //体扫描层数
    unsigned short SYear;            //观测记录开始时间的年（2000-）
    unsigned char SMonth;            //观测记录开始时间的月（1-12）
    unsigned char SDay;              //观测记录开始时间的日（1-31）
    unsigned char SHour;             //观测记录开始时间的时（00-23）
    unsigned char SMinute;           //观测记录开始时间的分（00-59）
    unsigned char SSecond;           //观测记录开始时间的秒（00-59）
    unsigned int  SMillisecond;      //秒的小数位（计数单位微秒）
    unsigned short MaxV;             //本层的最大可测速度，计数单位为厘米/秒
    unsigned short MaxL;             //本层的最大可测距离，以10米为计数单位
    unsigned short ZBinWidth;        //本层强度数据的库长，以1/10米为计数单位
    unsigned short VBinWidth;        //本层速度数据的库长，以1/10米为计数单位
    unsigned short WBinWidth;        //本层谱宽数据的库长，以1/10米为计数单位
    unsigned short ZBinNumber;       //本层扫描强度径向的库数
    unsigned short VBinNumber;       //本层扫描速度径向的库数
    unsigned short WBinNumber;       //本层扫描谱宽径向的库数
    unsigned short RecordNumber;     //本层扫描径向个数
    short SwpAngles;                 //本层的仰角，计数单位为1/100度，当扫描方式为RHI。不填此数组，当扫描方式为PPI时，
    //第一个元素为做PPI时的仰角，计数单位为1/100度，其它元素填写-32768
    unsigned short EYear;            //观测记录结束时间的年（2000-）
    unsigned char EMonth;            //观测记录结束时间的月（1-12）
    unsigned char EDay;              //观测记录结束时间的日（1-31）
    unsigned char EHour;             //观测记录结束时间的时（00-23）
    unsigned char EMinute;           //观测记录结束时间的分（00-59）
    unsigned char ESecond;           //观测记录结束时间的秒（00-59）
    unsigned char ETenth;            //观测记录结束时间的1/100秒（00-99）
    unsigned short   PRF1;           //本层第一重复频率 ，计数单位：1/10 Hz
    unsigned short   PRF2;           //本层第二重复频率，计数单位：1/10 Hz
    char Spare[255];                 //备用字节255个
} RADAROBSERVATIONPARAMPOLAR;

//SQUARE PRODUCT
typedef struct
{
    unsigned char NStep;             //体扫层次
    unsigned char LayerNumber;       //体扫描层数
    unsigned short SYear;            //观测记录开始时间的年（2000-）
    unsigned char SMonth;            //观测记录开始时间的月（1-12）
    unsigned char SDay;              //观测记录开始时间的日（1-31）
    unsigned char SHour;             //观测记录开始时间的时（00-23）
    unsigned char SMinute;           //观测记录开始时间的分（00-59）
    unsigned char SSecond;           //观测记录开始时间的秒（00-59）
    unsigned long SMillisecond;      //秒的小数位（计数单位微秒）
    unsigned short MaxV;             //本层的最大可测速度，计数单位为厘米/秒
    unsigned short MaxL;             //本层的最大可测距离，以10米为计数单位
    unsigned short EYear;            //观测记录结束时间的年（2000-）
    unsigned char EMonth;            //观测记录结束时间的月（1-12）
    unsigned char EDay;              //观测记录结束时间的日（1-31）
    unsigned char EHour;             //观测记录结束时间的时（00-23）
    unsigned char EMinute;           //观测记录结束时间的分（00-59）
    unsigned char ESecond;           //观测记录结束时间的秒（00-59）
    unsigned char ETenth;            //观测记录结束时间的1/100秒（00-99）

    unsigned short   PRF1;           //本层第一重复频率 ，计数单位：1/10 Hz
    unsigned short   PRF2;            //本层第二重复频率，计数单位：1/10 Hz
    short RHIL;                      //做RHI时的最低仰角，计数单位为1/100度，做其他扫描时为0
    short RHIH;                      //做RHI时的最高仰角，计数单位为1/100度，做其他扫描时为0
    char Spare[36];                  //备用36个字节
} RADAROBSERVATIONPARAMSQUARE;

typedef struct
{
    float m_Max;		//坐标最大值
    float m_Min;		//坐标最小值
    int m_Grid;			//是否显示网格 0 不显示 1 显示
    int m_GridSpace;	//网格数目
    int m_Ruler;		//是否显示标尺
    int m_RulerSpace;	//标尺数目
    char m_Label[50];	//标尺名称
    char m_Unit[20];	//标尺单位
} RADARAXISINFO;

typedef struct
{
    unsigned short ProductionID;       //产品名称
    //1=CAPPI
    //.......
    char ProductName[100];			//产品名称 new
    char ProductUnit[20];			//数值单位 new
    char ColorName[20];			//色标名称
    float XResolution;                //X网格距（单位为米）
    float YResolution;				  //Y网格距（单位为米）
    short XLength;                   //水平格点数
    short YLength;                   //垂直格点数
    short XRadar;                    //雷达位置X（X-左上角为零）
    short YRadar;                    //雷达位置Y（Y-左上角为零）
    short Vector;
    short Line;
    short Circle;
    short Text;
    short Triangle;
    short DoubleArc;
    short bAdjustColorTable;		//0不需要调整 1需要调整  new
    short bUnChar; 					//0-char;1-Unsigned Char;2-Short、Int;3-Float
    short bNeedMap;                 //是否显示地图 0-不显示  1-显示
    short BlankZero;  				//产品中空值的表示 new
    float addV;    	  				//产品中保存的为归一化值，变为真值后应该时=(V+addV)*mulV new
    float mulV;       				//产品中保存的为归一化值，变为真值后应该时=(V+addV)*mulV new
    unsigned short BinNumber;	    //产品的库数 new
    unsigned short BinWidth;		//产品的库长new
    float MaxValue;					//实际测到正的最大数值;new
    float MinValue;					//实际测到负的最小数值;new
    float Height;					//当Height==-1时则表示产品信息中不显示仰角和高度
    //当Height<360时则表示产品信息中显示仰角， 单位为度
    //当Height>360时则表示产品信息中显示高度，单位为Height-360 米;new
    short bNeedAxis;                //是否绘制X,Y轴.0-不显示XY轴，使用距离圈，1-显示XY轴,不使用距离圈
    RADARAXISINFO XAxis;                 //X轴信息
    RADARAXISINFO YAxis;                 //Y轴信息

    char WaringType[20];            //警报类型
    //HI TVS STORM RAINSTORM GUSTFRONT DOWNBURST
    char Spare[383];                 //备用403字节
} RADARPRODUCTBASEPARAM;

typedef struct
{
    unsigned short ProductionID;       //产品名称
    //1=CAPPI
    //.......
    char ProductName[100];			//产品名称 new
    char ProductUnit[20];			//数值单位 new
    char ColorName[20];			     //色标名称
    float XResolution;                //X网格距（单位为米）
    float YResolution;				  //Y网格距（单位为米）
    short XLength;                   //水平格点数
    short YLength;                   //垂直格点数
    short XRadar;                    //雷达位置X（X-左上角为零）
    short YRadar;                    //雷达位置Y（Y-左上角为零）
    short Vector;
    short Line;
    short Circle;
    short Text;
    short Triangle;
    short DoubleArc;
    short bAdjustColorTable;		//0不需要调整 1需要调整  new
    short bNeedMap;                 //是否显示地图 0-不显示  1-显示
    float Height;					//当Height==-1时则表示产品信息中不显示仰角和高度
    //当Height<360时则表示产品信息中显示仰角， 单位为度
    //当Height>360时则表示产品信息中显示高度，单位为Height-360 米;new
    short bNeedAxis;                //是否绘制X,Y轴.0-不显示XY轴，使用距离圈，1-显示XY轴,不使用距离圈
    RADARAXISINFO XAxis;            //X轴信息
    RADARAXISINFO YAxis;            //Y轴信息
    char WaringType[20];            //警报类型
    //HI TVS STORM RAINSTORM GUSTFRONT DOWNBURST
    char Spare[482];                 //备用502字节
} RADARPRODUCTVECTORPARAM;

typedef struct
{
    unsigned short Production;       //产品名称
    //1=CAPPI
    //.......
    int HeightAngle;			    //< 2<<24  RHI时为方位角，单位1/100度 Luo
    //< 2<<24 PPI时为仰角值，单位1/100度
    //> 2<<24CAPPI时为高度值，单位为1/100米
    short Range;                     //有效探测距离（单位为公里）
    short Resolution;                //网格距（单位为米）
    short XLength;                   //水平格点数
    short YLength;                   //垂直格点数
    short XRadar;                    //雷达位置X（X-左上角为零）
    short YRadar;                    //雷达位置Y（Y-左上角为零）
    short DAmp;                      //数值放大倍数
    long NRecord;                   //记录数目
    //占用字节数，百位表示是否有符号
    //0xx：表示无符号位；1xx表示有符号位
    short DZero;                     //无数据的代码
    char DataType;                   //资料类型
    //1=强度值（dBZ）
    //2=径向速度值（米/秒）
    //3=速度谱宽值（米/秒）
    char StationID[2];               //台站代码
    unsigned short Year;             //数据产品生成时间的年（2000-）
    unsigned char Month;             //数据产品生成时间的月（1-12）
    unsigned char Day;               //数据产品生成时间的日（1-31）
    unsigned char Hour;              //数据产品生成时间的时（00-23）
    unsigned char Minute;            //数据产品生成时间的秒（00-23）
    short Vector;
    short Line;
    short Circle;
    short Text;
    short Triangle;
    short DoubleArc;
    char Spare[712];                 //备用726字节
} RADARPRODUCTMERGEPARAM;

typedef struct
{
    unsigned short ProductionID;       //产品名称  Luo
    //1=CAPPI
    //.......
    char ProductName[100];			//产品名称
    char ProductUnit[20];			//数值单位
    char ColorName[20];			//色标名称
    short Vector;
    short Line;
    short Circle;
    short Text;
    short Triangle;
    short DoubleArc;
    short bAdjustColorTable;		//0不需要调整 1需要调整
    short bUnChar; 					//0是char;1是Unsigned Char;2是Short、float
    short BlankZero;  				//产品中空值的表示
    float addV;    	  		//产品中保存的为归一化值，变为真值后应该时=(V+addV)*mulV
    float mulV;       			//产品中保存的为归一化值，变为真值后应该时=(V+addV)*mulV
    unsigned short BinNumber;	    //产品的库数
    unsigned short BinWidth;		//产品的库长
    float MaxValue;					//实际测到正的最大数值;
    float MinValue;					//实际测到负的最小数值;
    float Height;					//当Height==-1时则表示产品信息中不显示仰角和高度
    //当Height<360时则表示产品信息中显示仰角， 单位为度
    //当Height>360时则表示产品信息中显示高度，单位为Height-360 米;
    char WaringType[20];            //警报类型
    //HI TVS STORM RAINSTORM GUSTFRONT DOWNBURST
    char Spare[360];                //备用字节380个
} RADAROTHERINFORMATIONPOLAR;

//PRODUCT
typedef struct
{
    char FileID[4];                 //雷达数据标识（原始数据标识符，‘RD’为雷达原
    //始数据，‘GD‘为雷达衍生数据等等）
    float VersionNo;                //表示数据格式的版本号
    int FileHeaderLength;          //表示文件头的长度
    RADARSITE SiteInfo;             //站址基本情况
    RADAROBSERVATIONPARAMPOLAR ObservationInfo; //观测参数
    RADAROTHERINFORMATIONPOLAR OtherInfo;       //其他信息参数
} PolarProduct;

typedef struct
{
    char FileID[4];                 //雷达数据标识（原始数据标识符，‘RD’为雷达原
    //始数据，‘GD‘为雷达衍生数据等等）
    float VersionNo;                //表示数据格式的版本号
    int FileHeaderLength;          //表示文件头的长度
    RADARSITE SiteInfo;             //站址基本情况
    RADAROBSERVATIONPARAMSQUARE ObservationInfo;//观测参数
    RADARPRODUCTBASEPARAM BaseInfo;             //其他信息参数
} SquareProduct;

typedef struct
{
    char FileID[4];                 //雷达数据标识（原始数据标识符，‘RD’为雷达原
    //始数据，‘GD‘为雷达衍生数据等等）
    float VersionNo;                //表示数据格式的版本号
    int FileHeaderLength;          //表示文件头的长度
    RADARSITE SiteInfo;             //站址基本情况
    RADAROBSERVATIONPARAMSQUARE ObservationInfo;//观测参数
    RADARPRODUCTVECTORPARAM BaseInfo;             //其他信息参数
} VectorProduct;

typedef struct
{
    char FileID[4];                 //雷达数据标识（原始数据标识符，‘RD’为雷达原
    //始数据，‘GD‘为雷达衍生数据等等）
    float VersionNo;                //表示数据格式的版本号
    int FileHeaderLength;          //表示文件头的长度
    RADARSITE SiteInfo;             //站址基本情况
    RADARPERFORMANCEPARAM PerformanceInfo;      //性能参数
    RADAROBSERVATIONPARAMSQUARE ObservationInfo;//观测参数
    RADARPRODUCTMERGEPARAM BaseInfo;             //其他信息参数
} MergeProduct;

typedef struct
{
    unsigned short AZ;            //方位，计数单位1/100度
    char VALUE[MAXBINDOTS];       //产品的
} PolarDataBuffer;

// typedef struct
// {
// 	unsigned short AZ;            //方位，计数单位1/100度
// 	short VALUE[PMAXBINDOTS];      //产品的
// } NewPolarDataBuffer;


typedef struct
{
    unsigned short AZ;            //方位，计数单位1/100度
    char VALUE[MAXBINDOTS];       //产品的
} PolarDataBufferCh;

typedef struct
{
    float AZ;              //矢量角度
    float Val;             //矢量大小
    float x;                        //矢量位置X
    float y;                        //矢量位置Y
    float z;                        //矢量位置Z
    short Style;                    //点，箭头，线
    char  ColorR;             //颜色R
    char  ColorG;             //颜色G
    char  ColorB;             //颜色B
    char  ColorA;             //颜色Alpha
    char  Appendix[6];
    char  Spare[8];
} VectorDataStruct;

typedef struct
{
    float StartX;             //线条开始X
    float StartY;             //线条开始Y
    float EndX;               //线条结束X
    float EndY;               //线条结束Y
    unsigned char  ColorR;             //颜色R
    unsigned char  ColorG;             //颜色G
    unsigned char  ColorB;             //颜色B
    unsigned char  ColorA;             //颜色Alpha
    char  Spare[20];
} LineDataStruct;

typedef struct
{
    float CenterX;             //圆心X
    float CenterY;             //圆心Y
    float Radius;              //半径
    char  ColorR;             //颜色R
    char  ColorG;             //颜色G
    char  ColorB;             //颜色B
    char  ColorA;             //颜色Alpha
    char  Spare[24];
} CircleDataStruct;

typedef struct
{
    float CenterX;             //三角中心X
    float CenterY;             //三角中心Y
    float Radius;              //边长
    unsigned char  ColorR;             //颜色R
    unsigned char  ColorG;             //颜色G
    unsigned char  ColorB;             //颜色B
    unsigned char  ColorA;             //颜色Alpha
    char  Direction;			//方向 0 向上三角 1 向下三角
    char  Spare[23];
} TriangleDataStruct;

typedef struct
{
    float CenterX;             //X
    float CenterY;             //Y
    float Radius;              //弧半径
    unsigned char  ColorR;     //颜色R
    unsigned char  ColorG;     //颜色G
    unsigned char  ColorB;     //颜色B
    unsigned char  ColorA;     //颜色Alpha
    char  Spare[24];
} DoubleArcDataStruct;

typedef struct
{
    float StartX;             //文字开始X
    float StartY;             //文字开始Y
    unsigned char  ColorR;    //颜色R
    unsigned char  ColorG;    //颜色G
    unsigned char  ColorB;    //颜色B
    unsigned char  ColorA;    //颜色Alpha
    char  Text[60];
    char  Spare[8];
} TextDataStruct;

typedef union
{
    VectorDataStruct Vector;
    LineDataStruct   Line;
    CircleDataStruct Circle;
    TextDataStruct   Text;
    TriangleDataStruct Triangle;
    DoubleArcDataStruct DoubleArc;
} VectorDataBuffer;

typedef struct
{
    float value;
    float longitude;
    float latitude;
    float altitude;
} POSITIONDATA;

#pragma pack(pop)

#endif /*PRODUCTDEF_H_*/
