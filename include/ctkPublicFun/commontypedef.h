/**
  * @file   commontypedef.h
  * @brief  绘图相关信息定义.
  *
  *  这里是详细说明。
  * @author nriet
  * @par   Copyright (c):
  *          nriet.
  *         All Rights Reserved
  * @date   2016:04:11
  * @note   这里是注意事项。
  * @version 1.0
  */

#ifndef COMMONTYPEDEF_H
#define COMMONTYPEDEF_H

#include <QString>
#include <QList>
#include <QVector>
#include <QDateTime>
#include <QObject>
#include <QPointF>
//#include "org.radar.dataadapter/ProductDef.h"


#define PI 3.14159265
#define PRECISION 0.000001
#define MAXNAMER 10 //雷达文件名分割成二位数组的最大行数
#define MAXNAMEC 40 //雷达文件名分割成二位数组的最大列数

const double EARTH_RADIUS  = 6371.137;      //地球半径KM
#define deg_to_rad(x) (x*PI/180)
#define rad_to_deg(x) (x*180/PI)
#define cmin(x,y) ((x)<(y)? (x):(y))
#define cmax(x,y) ((x)>(y)? (x):(y))

// 雷达组网显示系统的常量界面尺寸
const int PreviewWidgetSize = 200;
const int PreviewPictureSize = 180;
const int CanvasFrameWidth = 20;
const int TimeCtrlHeight = 40;
const int AnimationHeight = 40;
const int AnimationWidth = 200;

///雷达文件信息结构体
typedef struct _RADAR_FILEINFO_
{
    QString   m_strFileName;///<文件名
    QString   m_strFullPathName;///<带路径的文件名
    long      m_size;///<文件大小
    QDateTime m_inTime;///<文件入库时间
    int       m_levelIndex;///高度层信息，单位米，如果没有高度层，填写0

    // add by weikangc [9/12/2016 Administrator]
    int		  m_productId;	///< 产品id

    // add by weikangc [9/22/2016 Administrator]
    _RADAR_FILEINFO_& operator=(const _RADAR_FILEINFO_& rfI)
    {
        if( this != &rfI )
        {
            this->m_strFileName    = rfI.m_strFileName;
            this->m_strFullPathName= rfI.m_strFullPathName;
            this->m_size		   = rfI.m_size;
            this->m_inTime		   = rfI.m_inTime;
            this->m_levelIndex	   = rfI.m_levelIndex;
            this->m_productId	   = rfI.m_productId;
        }

        return *this;
    }

    // add by weikangc [9/22/2016 Administrator] 尚不考虑补图的情况
    bool operator ==(const _RADAR_FILEINFO_& rfI)
    {
        if( this->m_strFileName == rfI.m_strFileName &&\
                this->m_strFullPathName == rfI.m_strFullPathName &&\
                this->m_inTime == rfI.m_inTime &&\
                this->m_levelIndex == rfI.m_levelIndex &&\
                this->m_productId == rfI.m_productId)
        {
            return true;
        }

        return false;
    }

    // add by weikangc [9/22/2016 Administrator] 尚不考虑补图的情况
    bool operator !=(const _RADAR_FILEINFO_& rfI)
    {
        if( this->m_strFileName != rfI.m_strFileName ||\
                this->m_strFullPathName != rfI.m_strFullPathName ||\
                this->m_inTime != rfI.m_inTime ||\
                this->m_levelIndex != rfI.m_levelIndex ||\
                this->m_productId != rfI.m_productId)
        {
            return true;
        }

        return false;
    }

} RadarFileInfo;

Q_DECLARE_METATYPE(_RADAR_FILEINFO_)

//产品类型
enum EProductType
{
    POLAR = 1,
    SQUARE = 2,
    VECTOR = 3,
    UNKNOWN = 4
};

// 临时
enum PupType
{
    MIC4 = 1
};

//图片路径
typedef struct s_Picpath
{
    char picpath[1024];
    char picname[1024];

    s_Picpath()
    {
        memset(picpath, 0, sizeof(picpath));
        memset(picname, 0, sizeof(picname));
    }
} Picpath;

//站点信息
typedef struct
{
    char  id[10];       //站号,如：Z9250
    char  name[40];     //站名,如：南京
    float latitude;     //经度,单位千分之一度
    float longitude;    //纬度,单位千分之一度
    float height;       //高度,单位千分之一米
} Station;

typedef struct
{
    QString productType;	//产品缩写
    QString productName;	//产品中文名
    int type;				//产品类型
    int dataFlag;			//是否出数据
    int picFlag;			//是否出图片
    QString productID;		//产品ID
} ProductSet;

//图片信息
typedef struct s_Picture
{
    char type[5];		//图片规格:XL,L,M,S
    int width;			//图片像素宽度
    int height;			//图片像素高度,(Web端)图片的高度=图片的宽度 或 (手机端)图片的高度>=图片的宽度+18*FontSize
    int signalBlank;    //图片单边空白,单位像素,Web端图片应该设为0,手机端时有效
    int fontSize;       //字体大小
    int drawCircle;     //是否画距离圈,0-不画，1-画
    int drawDistance;	//是否画距离圈上的文字,0-不画，1-画,当画距离圈时有效
    int drawMap;		//是否画背景地图,0-不画，1-画
    int drawInfo;       //是否画站点信息,0-不画,1-画
    int drawColor;      //是否画色标,0-不画,1-画
    int transparent;	//是否透明,0-不透明,1-透明
    int backColor[3];   //背景颜色(r,g,b),如：238,248,247
    bool enable;		//是否启用

    s_Picture()			//初始化结构体技巧
    {
        char *pStart = type;
        memset(pStart, 0, (char *)&enable+1-pStart);
        enable = false;
    }
} Picture;

//产品信息
typedef struct
{
    int  id;        //产品ID
    char enName[10];//产品英文缩写
    char zhName[40];//产品中文名
    int  maxl;      //探测距离KM
    int  distStep;  //距离圈距离KM
} Product;

//雷达绘图色标信息
typedef struct
{
    int r;              //RGB值
    int g;
    int b;
    float value;        //色标值
    QString realValue;   //色标名称
} RGBCOLOR;

typedef struct
{
    QString name;   //产品色标名称
    QVector<RGBCOLOR> rgbColorLst;
} PRODUCTRGBCOLOR;
//以上为雷达绘图色标信息

//产品算法结构信息
typedef struct
{
    int ProductID;			//产品ID
    QString ProductName;	//产品缩写
    QString ModuleName;		//算法名
    int type;				//数据类型
} PRODUCTMODULE;

// 雷达产品名称色标对应信息
typedef struct
{
    int ProductID;			//产品ID
    QString ProductEnName;	//产品英文缩写
    QString ColorName;		//色标名称
    QString ProductZhName;	//产品中文名称
} PRODUCTCODE;

//产品算法接口参数定义
typedef long QLPARAM;
typedef unsigned int QWPARAM;
//产品算法接口定义
typedef void* (*DLLFUN_Init)(void*, void*, const char *);
typedef void (*DLLFUN_UnInit)(void*);
typedef void (*DLLFUN_MakeProduction)(QLPARAM lParam, QWPARAM wParam, void*);

typedef struct
{
    DLLFUN_Init init;
    DLLFUN_UnInit unInit;
    DLLFUN_MakeProduction makeProduct;
} ModFun;

typedef struct productInfo
{
    int productId;      //产品ID
    QString stationId;   //站点ID
    QString productType; //产品类型R、V、CR等
    QString zhName;
    float evelation;    //仰角
    int width;          //图片像素宽度
    int height;         //图片像素高度,(Web端)图片的高度=图片的宽度 或 (手机端)图片的高度>=图片的宽度+18*FontSize
    int maxl;           //图片单边空白,单位像素,Web端图片应该设为0,手机端时有效
    int distStep;      //距离圈距离KM
    int flag;          //实时画图1
    int signalBlank;
    int fontSize;      //字体大小
    int drawCircle;//是否画距离圈,0-不画，1-画
    int drawDistance;//是否画距离圈上的文字,0-不画，1-画,当画距离圈时有效
    int drawMap;   //是否画背景地图,0-不画，1-画
    int drawInfo;  //是否画站点信息,0-不画,1-画
    int drawColor;  //是否画色标,0-不画,1-画
    int transparent;//是否透明,0-不透明,1-透明
    int backColor[3];//背景颜色(r,g,b),如：238,248,247
    productInfo()
    {
        productId = 0;      //产品ID
        stationId = "";   //站点ID
        productType = ""; //产品类型R、V、CR等
        zhName = "";
        evelation = 0;    //仰角
        width = 800;          //图片宽度
        height = 1000;         //图片高度
        maxl=230;           //探测距离KM
        distStep = maxl*100/460;      //距离圈距离KM
        flag = 1;           //实时画图1
        signalBlank = 0;
        fontSize=12;
        drawCircle=1;
        drawDistance=1;
        drawMap=0;
        drawInfo=0;
        drawColor=0;
        transparent=1;
        backColor[0] = 0;
        backColor[1] = 0;
        backColor[2] = 0;
    }
} PRODUCTINFO;

//////////////////////////////////////////////////////////////////////////

const QString VtbType = "vtb";
const QString MosaciType = "mosaic";
const QString PupType = "pup";
const QString HighType = "high";

typedef struct filenameRule
{
    QString		type;		//数据类型（*）
    QString		dTime;		//年月日时分秒14位（*）
    QString		station;	//站名
    QString		area;		//区域名
    bool		isPic;		//true表示图片，fasle表示数据
    QString		picSize;	//图片尺寸
    QString		product;	//产品ID
    QString		param;		//参数（仰角、预测分钟等）

    filenameRule()
    {
        isPic = false;
        param = "000";
    }
} FileNameRule;


enum LayerTypeFlag
{
    ProductLayer = 0, // 产品图层
    BaseLayer,		  // 基础地理信息图层
    MaskOutLayer      // 掩膜图层
};

// 图形类型枚举
enum ShapeType
{
    PointShape = 0,
    LineShape,
    PolygonShape
};

typedef struct PubBaseInfo
{
    int productCode;
    int maxl;
    QString productUnit;
    QString colorName;
    int aadv;
    float mulv;
    int xMapmap;
    int yMapmap;
    float resolution;
    float binWinthmap;
    PubBaseInfo()
    {
        aadv = 0;
        mulv = 1;
        maxl = 0;
        productUnit = "";
        colorName = "";
    }
} PUBBASEINFO;



#define BaseLayerFlag "BaseLayer"
#define MaskOutLayerFlag "MaskOutLayer"
#define ProductLayerFlag "ProductLayer"

typedef struct Rectangle2D
{
    double Xmin;
    double Ymin;
    double Xmax;
    double Ymax;

    Rectangle2D() {};
    Rectangle2D(const QPointF &p1, const QPointF &p2)
    {
        Xmin = p1.x();
        Ymin = p1.y();
        Xmax = p2.x();
        Ymax = p2.y();
    };

    Rectangle2D(double xMinimum, double yMinimum, double xMaximum, double yMaximum)
    {
        Xmin = xMinimum;
        Ymin = yMinimum;
        Xmax = xMaximum;
        Ymax = yMaximum;
    };

} RECTANGLE2D;

//////////////////////////////////////////////////////////////////////////

struct UserInfo
{
    QString		userID;
    QString		userName;
    int			sendType;	///< 分发方式：0-FILE，1-FTP
    QString     sendSuffix;	// 传输过程中的临时文件后缀名
    QString		rootPath;
    int			timebaserule;	///< 时间变量基于的时间规则：0无，1-基于收集目录时间，2-基于文件名时间，3-基于系统时间
    QString		lgUser;
    QString		lgPass;
    QString		ip;
    int			port;
    int			keepDir;
    int			compress;
    int			encrypt;
    int			conput;
    int			tryCount;

    UserInfo ()
    {
        conput = 0;
        tryCount = 3;
    }
};

struct CollectUser
{
    QString		rltvPath;
    UserInfo	user;
};

Q_DECLARE_METATYPE(CollectUser)

struct TaskUser
{
    QString				taskID;
    QList<CollectUser>	lstUser;
};

struct CollectTask
{
    QString dirID;
    QString dirName;
    int		enable;
    int		collectType;	///< 0-共享，1-FTP
    int     ftp_transferMode;	// 传输模式，0为binary,1为ascii
    int     ftp_connectMode;	// 连接模式，0为被动，1为主动
    QString	rltvPath;
    QString dispatch;
    QString fileTemplate;
    int		subdirFlag;
    int		moveFlag;
    int     col_timerange;	///< 收集时间范围，单位：分钟
    QString	loginUser;
    QString	loginPass;
    QString ip;
    int		port;
    bool    recordLatestTime;	// 是否记录最后收集时间，一旦记录了最后收集时间，下次启动收集时，就会从比最后收集时间新的文件开始收集

    CollectTask()
    {
        enable = 1;
    }
};

struct TransTask
{
    CollectTask		collectSet;
    QList<UserInfo> userInfo;
    QList<QString> dstFilePath;
    QString fileName;
    QString srcFileFullPath;
};

enum TaskState
{
    SYSTEM = 0,
    SUCCESS,
    FAIL
};



struct ClearTask
{
    QString taskName;	// 清理任务名
    QString taskDir;	// 任务目录
    QString quartzRule;	// 清理任务规则
    QString matchRule;	// 删除文件匹配规则
    int clearUnit;		// 匹配文件最后修改时间规则 单位 0：分钟 1：小时 2：天 3 月
    int clearValue;		// 匹配文件最后修改时间规则
    int activeFlag;		// 启用标识

    // 根据清理的unit + value确定清理的时间
    int computeSecond() const
    {

        // 0:分钟， 1:小时, 2：天 3: 月
        int step = 1;

        switch (clearUnit)
        {
        case 0:
            step = 60;
            break;
        case 1:
            step = 60*60;
            break;
        case 2:
            step = 24*60*60;
            break;
        case 3:
            step = 30*24*60*60;
            break;
        default:
            step = 1;
            break;
        }

        return clearValue * step;
    }
};

Q_DECLARE_METATYPE(ClearTask)

#endif