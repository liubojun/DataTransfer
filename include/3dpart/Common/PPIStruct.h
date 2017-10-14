/*************************************************************************
* Copyright(c) 2016,NRIET
* All rights reserved.
* @file			:	PPIStruct.h
* @brief		:	体扫数据类型结构定义头文件
* @version	:	V1.0
* @author		:	Administrator
* @date		:	2016-03-02
------------------------------------------------------------------------------------------------------
* @modify	:	修改说明
* @brief		:	修改内容
* @author		:	修改者
* @date		:	修改日期
* @version	:	替换版本
**************************************************************************/

#ifndef _H_H_PPISTRUCT_H_H_
#define _H_H_PPISTRUCT_H_H_

#include <ctime>

#define SITE_NAME_LEN		20			//站点名称长度(字符)
#define SITE_TYPE_LEN		12			//雷达数据格式类型名长度(字符)
#define DATA_TYPE_LEN		4			//雷达数据类型字符串长度(如REF，VEL..)
#define MAX_SITES			50			//区域内最多雷达站数

#define MAX_HGT_LEVELS		40			//组网垂直方向的最多层数

#define CODE_INVALID		0				//No data
#define CODE_RANFOLD		1				//range fold
#define CODE_NO_OBSERV		2				//no observation
#define CODE_CSRED			3				//Censored data

#define VALUE_INVALID		-9900			//实际值中的特殊标记, 表示无有效观测数
#define VALUE_RANFOLD		-9901			//实际值中的特殊标记, 表示有距离模糊
#define VALUE_CLUTTER		-9902
#define VALUE_CLEARAIR		-9903
#define VALUE_SEACLUTTER	-9904
#define INVALID_STRING		"----"

#define MOSAIC_EXT_FILE		".dat"			//笛卡儿坐标单站雷达数据临时保存的文件
#define MOSAIC_SUBFOLDER	"MREF"			//组网数据保存的最底层目录
#define QC_SUBFOLDER 		"QCBaseData"	//Folder for QC processed data to be saved in
#define BD_SUBFOLDER		"BaseData"		//Folder for radar base data
#define CR_SUBFOLDER		"CR"			//Folder for CR product
#define ET_SUBFOLDER		"ET"			//Folder for ET product
#define VIL_SUBFOLDER		"VIL"			//Folder for VIL product
#define RF_SUBFOLDER        "RF"            //Folder for RF product  降雨产品
#define IC_SUBFOLDER        "IC"            //Folder for CV product  云识别产品

#define KM_TO_DEG  0.01						//中纬度，1公里大约相当于0。01度
#define AX 200.0
#define BX 1.6


#define C_Factor10  10		// 放大因子，10倍
#define C_Factor100  100	// 放大因子，100倍
#define C_Factor1000  1000	// 放大因子，1000倍

int const PATH_LEN = 256;

const short int STAT_FISRT_LEVEL =  0;		//第一层
const short int STAT_MID_LEVEL   =  1;		//中间层
const short int STAT_LAST_LEVEL  =  2;		//最后一层

struct COMMONTIME
{
    short year;
    char month;
    char day;
    char hor;
    char min;
    char sec;
};

//单一时间轴
struct tmAsix
{
    time_t st;			///< 起始时间
    time_t et;			///< 终止时间
    time_t matchtm;		///< 匹配时间
};

struct DATADIRECTORY
{
    char strBaseDataDir[PATH_LEN];			//基数据检索指示文件位置
    char strRainGageDir[PATH_LEN];			//雨量计数据位置
    char strProdDataDir[PATH_LEN];			//产品存储位置
};

//分辨率
struct HORIZRESPARAM
{
    float fLonRes;
    float fLatRes;
};

//在组网过程中要用到的单站相对于组网网格的位置
struct SITEPOSINMOSAIC
{
    int iMinLonGrid;						//单站数据在组网网格中的最小经向格点位置
    int iMaxLonGrid;						//单站数据在组网网格中的最大经向格点位置
    int iMinLatGrid;						//单站数据在组网网格中的最小纬向格点位置
    int iMaxLatGrid;						//单站数据在组网网格中的最大纬向格点位置
};

//雷达站点信息数据结构的定义
struct RADARSITEINFO						// 52 Bytes
{
    int	 SiteID;								//Site index
    char SiteName[SITE_NAME_LEN];				//Site Name, the number of characters is not more than 20
    char SiteType[SITE_TYPE_LEN];				//Site type, whitch indicates data format type <=12 chars
    int	 SiteLon;								//Longtitude of the site(Degree*1000)
    int	 SiteLat;								//Latitude of the site(Degree*1000)
    int	 SiteHgt;								//Height(m) above sea level of the antenner
    int	 ObsRange;								//Observation range(km)//deg
};

const int NAZIM	=	512;
const int NGATE	=	1000;

//体扫的PPI数据结构的定义
struct PPISTRUCT
{
    char		radar_name[SITE_NAME_LEN];			//雷达名称编号
    char		data_type[SITE_TYPE_LEN];			//雷达数据类型
    char		data_name[DATA_TYPE_LEN];			//雷达数据名称REF
    short int 	year;								//观测年份
    short int	month;								//观测月份
    short int	day;								//观测天数
    short int	hour;								//观测小时数
    short int	minute;								//观测分钟数
    short int	second;								//观测秒钟数
    int			radlat;								//雷达所在经度，以1/1000度为计数单位		//*1000
    int			radlon;								//雷达所在纬度，以1/1000度为计数单位		//*1000
    short int	radhgt;								//雷达所在海拔高度，单位米
    short int	elev_angle;							//本次PPI仰角，以1/10度为计数单位 //*10
    short int	fstgatdis;							//???	//*10
    short int	nyq_vel;							//???  //*10
    short int	num_beam;							//本次体扫经向数
    short int	num_gate;							//本次体扫经向库数
    short int	gateWidth;							//本次体扫经向库长，单位米
    short int	elev[NAZIM];						//本次体扫每个经向上的仰角，以1/10度为计数单位		//*10
    short int	azim[NAZIM];						//本次体扫每个经向上的方位角，以1/10度为计数单位	//*10
    short int	field[NAZIM][NGATE];				//本次体扫数据.						//*10
};

//组网网格参数的定义
struct MOSAICGRIDPARAM
{
    int iStartLon;									//区域起始经度Degree*1000
    int iStartLat;									//区域起始纬度Degree*1000
    int iStartHgt;									//开始高度 (米)
    unsigned short iLonNumGrids;					//经向格点数
    unsigned short iLatNumGrids;					//纬向格点数
    unsigned short iHgtNumLevels;					//垂直格点数
    unsigned int iLonRes;							//经向分辨率Degree*10000
    unsigned int iLatRes;							//纬向分辨率Degree*10000
    unsigned short iHeights[MAX_HGT_LEVELS];		//垂直方向的高度
};

// 三维雷达拼图数据头
struct MOSAICDATAHEADER										// 112 Bytes
{
    unsigned short  iVersion;								//格式版本号 1，2，- 2字节
    unsigned short  iProductId;								//产品ID, added by liubojun
    unsigned short  iTemp[6];								//保留 -12字节 , modified by liubojun
    char			strZoneName[SITE_NAME_LEN];				//区域名称 - 20字节
    char			strDataType[DATA_TYPE_LEN];				//数据名  -4 Bytes
    unsigned short	iNumRadars;								//区域内雷达数 - 2字节
    unsigned short	iStYear;								//观测开始时间-年 2字节
    unsigned short	iStMonth;								//观测开始时间-月 2字节
    unsigned short	iStDay;									//观测开始时间-日 2字节
    unsigned short	iStHour;								//观测开始时间-时 2字节
    unsigned short	iStMinute;								//观测开始时间-分 2字节
    int				iFstLon;								//网格开始经度 (度*1000) - 4字节
    int				iFstLat;								//网格开始纬度 (度*1000) - 4字节
    int				iLstLon;								//网格结束经度 (度*1000) - 4字节
    int				iLstLat;								//网格结束纬度 (度*1000) - 4字节
    unsigned short	iFstHgt;								//网格开始高度 (米) - 2字节
    unsigned short	iLstHgt;								//网格结束高度 (米) - 2字节
    unsigned short	iLonGrids;								//经向格点数 - 2字节
    unsigned short	iLatGrids;								//纬向格点数 - 2字节
    unsigned short	iLevels;								//高度层数   - 2字节
    unsigned int	iLonRes;								//经向格点分辨率 (度*10000) - 4字节
    unsigned int	iLatRes;								//纬向格点分辨率 (度*10000) - 4字节
    short			iHgtRes;								//高度分辨率(米; -1表示高度间距不均匀) - 2字节
    unsigned int	dwRadInfoOffset;						//雷达站信息数据开始位置(相对于头末尾)  - 4字节
    unsigned int	dwLevHgtOffset;							//各层高度数据开始位置(相对于头末尾)    - 4字节
    unsigned int	dwObvDataOffset;						//数据(反射率\速度等)开始位置(相对于头末尾) - 4字节
    short			addV;									//偏移
    short			mulV;									//倍率 (放大10倍存储，原始值=mulV/10)
    short			temp[1];								//保留 -2字节
    unsigned short	iStSec;									//观测开始时间-秒 2字节
};

//////////////////////////////////////////////////////////////////////////
// PPI数据头结构
typedef struct tagCommPPIHeader
{
    char	            site_name[SITE_NAME_LEN];		//Radar site name < 20 characters
    char				radar_type[SITE_TYPE_LEN];		//radar type < 12 characters
    char				data_name[DATA_TYPE_LEN];		//data name "REF" 'VEL"...< 4 characters
    unsigned short		vcp;
    unsigned short		year;
    unsigned short		month;
    unsigned short		day;
    unsigned short		hour;
    unsigned short		minute;
    unsigned short		second;
    long				radlat;		//DEG*1000
    long				radlon;		//DEG*1000
    short				radhgt;		// meters
    short				elev_angle;	//*10
    short				fstgatdis;	//*10
    short				nyq_vel;	//M/S*10
    unsigned short		elev_idx;	//elevation index
    unsigned short		num_beam;
    unsigned short		beam_width;	//Width of a beam( 0.1Deg)
    unsigned short		num_bin;
    unsigned short		bin_width ;	//m*10
    unsigned short		ppi_stat;	//PPI status 3: First elevation
    //           4: Last elevaion
    //           1: Middle elevation
    //           5: PPI elevation
    //           6: RHI elevation
} CommPPIHeader;

// PPI数据结构体
typedef struct tagCommonPPIData
{
    CommPPIHeader	header;
    short  		    elevation[NAZIM];		//*100  //short  -32767 -- +32768
    unsigned short	azimuth[NAZIM]; 		//*100  //Unsigned short 0 -- +65536
    short		    data[NAZIM][NGATE];		//*100  //short  -32767 -- +32768*/
    double          vdata[NAZIM][NGATE];
} CommonPPIData;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//地方雷达组网数据头
typedef struct MOSAICHEAD
{
    char ZonName[12];			// diamond 131 12个字节
    char DataName[38];			//数据说明(例如 2008年5月19日雷达三维拼图)38个字节
    char	Flag[8];			// 文件标志，"SWAN"
    char	Version[8];			// 数据版本号，"1.0"
    unsigned short year;		//2008 两个字节
    unsigned short month;		//05两个字节
    unsigned short day;			//19两个字节
    unsigned short hour;		//14两个字节
    unsigned short minute;		//31两个字节
    unsigned short interval;	//两个字节
    unsigned short XNumGrids;	//1000 两个字节
    unsigned short YNumGrids;	//1000 两个字节
    unsigned short ZNumGrids;	//21  两个字节
    int RadarCount;				//拼图雷达数 四个字节
    float StartLon;				//网格开始经度（左上角） 四个字节
    float StartLat;				//网格开始纬度（左上角） 四个字节
    float CenterLon;			//网格中心经度 四个字节
    float CenterLat;			//网格中心纬度 四个字节
    float XReso;				//经度方向分辨率 四个字节
    float YReso;				//纬度方向分辨率 四个字节
    float ZhighGrids[40];		//垂直方向的高度（单位km）数目根据ZnumGrids而得（最大40层） 160个字节。
    char RadarStationName[20][16];  //雷达站点名称,	20*16字节
    float  RadarLongitude[20];		//雷达站点所在经度，单位：度， 4*20字节
    float  RadarLatitude[20];		//雷达站点所在纬度，单位：度， 4*20字节
    float  RadarAltitude[20];		//雷达所在海拔高度，单位：米， 4*20字节
    unsigned char MosaicFlag[20];//该雷达数据是否包含在本次拼图中，未包含:0，包含:1, 20字节
    char	Reserved[172];
} MosaicHead;

#endif	//_H_H_PPISTRUCT_H_H_