//#include <QFont>
#include <QVector>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include <string>
#include <math.h>
#include "commontypedef.h"
#include "macro.h"		//导入导出声明

#ifndef GINFORMATION_H_
#define GINFORMATION_H_

/**
  * @class <>
  * @brief  全局配置类.
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL GPARAM
{
public:
    static GPARAM *getInstance();

    std::vector<unsigned short> RMatrix;

    std::vector<unsigned short> AzMatrix;

    //加载datatable.system信息
    int genIndexTable();

    //显示操作相关
    void getPPIAzAndR(int X,int Y,int PicWidth,int DisWidth,float* R,float* AZ);
    //////////////////////////////////////////////////////////////////////////
public:
    ~GPARAM();

    void init();
public:
    //bool CanClose;
    //bool bAutoStart;
    //bool bDiskFull;			//磁盘是否满
    //int MaxCpu;				//Cpu利用率
    //ulong MinFreeDisk;		//磁盘最小空间 单位：M
    //ulong MinFreeMemory;	//最小内存

    QString AppPath;
    //QString IPC;
    //QString TCP;
    //QString WorkPath;		//当前产品路径
    //QString SavePath;		//当前产品保存路径
    //QString PicSavePath;	//图片保存路径

    //string MapPicPath;		//背景地图存放路径

    QList<int> CAPPIHeights;
    QVector<Picture> Picturelst;					//图片规格
    std::vector<PRODUCTRGBCOLOR> ProductColorList;	//颜色值
    QVector<ProductSet> productSetVec;

    //////////////////////////////////////////////////////////////////////////
    //BEGIN ADD BY WEIKANGC 2016-01-19
    //int		m_nSocketType;							//通信方式
    //QString m_qStrSyncPort;							//同步端口
    //QString m_qStrSyncIPC;							//同步IPC
    QVector<bool> m_bPicturelst;					//是否启用了图片规格设置.
    QMap<QString, bool> m_ArrayPicProInfo;			//图片产品信息.
    //QMap<QString, QString> m_ArrayColorCHName;		//图例中文名称.
    //QMap<QString, QString> m_ArrayReadFileDateTime;	//读取日期.
    //ARRAYSCANSETIINFO	   m_ArrayScanSetInfo;		//扫描设置.

private:
    GPARAM();		//私有构造
    // void readParam();
    void readCAPPIHeights();
    /**
     * @brief  读取产品设置（是否出产品，图片或数据等）
     * @return void：无
     */
    void readProductSet();
    int  readPictureInfo();
    int  readProductColor();						//读取色标文件
    bool readPictureInfo(FILE *fp, std::string str);	//获取图片的规格信息
    bool readBackColor(Picture &pic, FILE *fp);	//读配置文件中背景颜色字段

    //////////////////////////////////////////////////////////////////////////
    //BEGIN ADD BY WEIKANGC 2016-01-25
    // bool ReadProductColorCHName();
    //bool ReadLastFileDateTime();
    //bool ReadScanSetInfo();
    //bool ReadScanSetInfo(FILE* fp, string str);
    //bool ReadWprdConfigInfo();
    //bool ReadStartupConfig();


    /**
     * @brief  加载常规信息
     * @return void：无
     */
    void primaryBaseInfo();

public:
    //void ReadWprdLastTime(int nType, QString& strLastTime);
    //void WriteWprdLastTime(int nType, QString strLastTime);
    //void WriteWprdPath();
    //void WriteDispatcher();
    //void WriteScan();
    //void WriteStatup(int nType, int nStartup);
    //QString	ReadCHNameByENName(const QString& qStrENName);
    //END ADD.

    /// 格点最大值
    QMap<int,int> gMaxL;

    /// 系数
    QMap<int,int> gAAdV;

    /// 系数
    QMap<int,int> gmulV;

    /// 产品距离单位
    QMap<int,QString> gProductUnit;

    /// 产品色标名称
    QMap<int,QString> gColorName;

    /// 用途暂定?
    QMap<int,float> Resolution;

    /// 库长
    QMap<int,float> m_BinWidthmap;

private:
    static GPARAM *s_param;
    static QMutex m_oMutex;
};

#endif
