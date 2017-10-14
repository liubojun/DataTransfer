#ifndef struct_swan_h__
#define struct_swan_h__

#include <vector>
#include <QVector>

typedef struct tagRadar_domain_struction		// 组合反射率经纬度信息结构体
{
    float slat;	//起始纬度 deg
    float wlon; //起始经度
    float nlat;	//终止纬度
    float elon; //终止经度
    float clat;
    float clon;
    int   rows;
    int   cols;
    float dlat;
    float dlon;  // minimize = 0.01 deg
} LATLON_DOMAIN;

typedef struct tagLatLonGridDataFileHeaderStruct	// 组合反射率文件头结构体
{
    char  dataname[128];  // CREF_FZ
    char  varname[32];    // CREF
    char  units[16];      // dBZ
    unsigned short label; // 'L'>>8|'L'
    short unitlen ;       // 2=short
    LATLON_DOMAIN  domain; //
    float nodata;         // -32 for radar
    long  levelbytes;	  // data bytes of per-level
    short levelnum;       // level numbers of data
    short amp;            // amplify factor = 10;
    short compmode;       // 0 = LatLon grid; 1=sparse LatLon grid
    unsigned short dates;  // UTC dates
    int   seconds;         // UTC seconds
    short min_value;       // used in compress mode
    short max_value;       // used in compress mode
    short reserved[6];
} LLDF_HEADER;

// typedef struct tagLatLonGridDataBlockStruct
// {
// 	short *pData;	//指向数据区
// 	int nLen;		//short的个数
// } LLDF_DataBlock;

typedef struct diamond4		// 格点数据结构体
{
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short min;
    unsigned short sec;
    unsigned short forecast_time;
    float fInvalid;		///< 无效值
    int row;			//经向格点数（行数）
    int col;			//纬向格点数（列数）

    int realRow;
    int realCol;

    int startRow;
    int startCol;

    int endRow;
    int endCol;

    char colorname[16];		// 图例名
    QVector<short> buffer;	// 数据区（100倍）

    diamond4()
    {
        char *pMove = (char *)&year;
        memset(pMove, 0, (colorname+16)-(char *) &year);
    }

    diamond4(const diamond4 &da)
    {
        operator = (da);
    }

    diamond4 &operator = (const diamond4 &da)
    {
        if (this != &da)
        {
            year = da.year;
            month = da.month;
            day = da.day;
            hour = da.hour;
            min = da.min;
            sec = da.sec;
            fInvalid = da.fInvalid;
            forecast_time = da.forecast_time;
            row = da.row;
            col = da.col;
            realRow = da.realRow;
            realCol = da.realCol;
            startRow = da.startRow;
            startCol = da.startCol;
            endRow = da.endRow;
            endCol = da.endCol;
            memcpy(colorname, da.colorname, sizeof(colorname));
            buffer = da.buffer;
        }

        return *this;
    }

} DIAMOND4;

typedef std::vector<DIAMOND4> DIAMOND4_ARRAYS;

#endif // struct_swan_h__
