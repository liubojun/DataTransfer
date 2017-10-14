/*************************************************************************
* Copyright(c) 2016,NRIET
* All Rights Reserved.
* @file		:	ginformation.cpp
* @brief	:	一体化雷达全局参数维护源文件实现.
* @version	:	V1.0
* @author	:	NRIET
* @date		:	未知
---------------------------------------------------------------------------
* @modify	:	规范代码风格，去除删除项.
* @brief	:	规范代码风格
* @author	:	NRIET....
* @date		:	2016-03-17
* @version	:	替换版本
**************************************************************************/

#include "ginformation.h"
#include "ctkPublicFun.h"
#include "ctkLog.h"
#include "pathbuilder.h"
#include "zlib.h"
#include <QCoreApplication>
#include <QSettings>
#include <QDir>
#include <QTextCodec>

#include <fstream>
#include <string.h>
#include <sstream>
#include <ctype.h>

using namespace std;

#pragma warning(disable:4996)	//禁止安全类警告

GPARAM *GPARAM::s_param = NULL;
QMutex GPARAM::m_oMutex;

GPARAM *GPARAM::getInstance()
{
    if (s_param == NULL)
    {
        QMutexLocker autolocker(&m_oMutex);
        if (s_param == NULL)
        {
            s_param = new GPARAM();
        }
    }
    return s_param;
}


GPARAM::GPARAM()
{
    init();
}

GPARAM::~GPARAM()
{
}

void GPARAM::init()
{
    AppPath  = QCoreApplication::applicationDirPath();

    if(genIndexTable() != 0)
    {
        QSLOG_DEBUG(QString::fromLocal8Bit("读配置文件[Config/datatable.system]失败!"));
        exit(-1);
    }

    //readParam();
    readCAPPIHeights();
    readPictureInfo();
    // readProductColor();
    readProductSet();

    primaryBaseInfo();
}

//void GPARAM::readParam()
//{
//    QSettings settings(AppPath + "/config/QClient.ini", QSettings::IniFormat, 0);
//    settings.setIniCodec("GBK");
//
//    WorkPath	= settings.value("PATH/WorkPath", "").toString();
//    SavePath	= settings.value("PATH/SavePath", "").toString();
//    PicSavePath = settings.value("PATH/PicSavePath", "").toString();
//    IPC			= settings.value("BROADCAST/IPC", "").toString();
//    TCP			= settings.value("BROADCAST/TCP", "").toString();
//
//    //ADD BY WEIKANGC 2016-01-19
//    m_qStrSyncPort	= settings.value("SYNCH/PORT", "").toString();
//    m_qStrSyncIPC	= settings.value("SYNCH/IPC", "").toString();
//    m_nSocketType	= settings.value("SOCKET/Type", "").toInt();
//    //END ADD.
//
//    QDir dir(WorkPath);
//
//    //if (!dir.exists())	//目录不存在
//    //{
//    //    bAutoStart = false;
//    //}
//    //else				//目录存在
//    //{
//    //    bAutoStart = true;
//    //}
//
//    //QDir dirSave(SavePath);
//
//    //if (!dirSave.exists())
//    //{
//    //    SavePath = AppPath + "/savedir";
//    //    settings.setValue("PATH/SavePath", SavePath);
//    //}
//
//    //MinFreeDisk		= settings.value("OPTION/MinFreeDisk", "400").toInt();
//    //MinFreeMemory	= settings.value("OPTION/MinMemory", "500").toInt();
//    //MaxCpu			= settings.value("OPTION/MaxCpu", "80").toFloat();
//}

void GPARAM::readCAPPIHeights()
{
    CAPPIHeights.clear();
    QSettings settings(QCoreApplication::applicationDirPath() + "/config/modparam.ini", QSettings::IniFormat, 0);	//临时

    int Num = settings.value("CAPPI/NUM", "0").toInt();

    for (int i=1; i<=Num; i++)
    {
        int height = settings.value("CAPPI/h"+QString().setNum(i), 500*i).toInt();
        CAPPIHeights.append(height);
    }
}

int GPARAM::readPictureInfo()
{
    string filename = AppPath.toStdString() + "/config/PictureInfo.ini";
    FILE *fp = NULL;
    if((fp=fopen(filename.c_str(), "r")) == NULL)
    {
        //QSLOG_DEBUG(QString("打开配置文件[%1]失败！").arg(filename.c_str()));
        return -1;
    }

    QSharedPointer<FILE> autoClose(fp, fclose);

    //读PICTURE/Resolution值
    string picinfo;
    if(!readIniFileC(fp, "PICTURE", "Resolution", picinfo))
    {
        return -1;
    }

    int nPos = picinfo.find_first_of('\r');
    if (nPos > 0)
    {
        picinfo = picinfo.substr(0, nPos);
    }

    //////////////////////////////////////////////////////////////////////////
    //ADD BY WEIKANGC 2016-01-19

    vector<string> resolutionLst;
    if(!splitStr(picinfo, '|', resolutionLst))	// S,M,L
    {
        return -1;
    }

    string bUseInfo;
    if ( !readIniFileC(fp, "PICTURE", "FLAG", bUseInfo) )
    {
        return -1;
    }

    vector<string> FlagLst;
    if( !splitStr(bUseInfo, '|', FlagLst) )
    {
        return -1;
    }

    for (vector<string>::size_type nIndex = 0; nIndex < FlagLst.size(); ++nIndex )
    {
        QString sTemp = FlagLst[nIndex].c_str();
        if( sTemp.contains('1') )
        {
            m_bPicturelst.push_back( true );	//需要绘制此规格图片

            if ( nIndex < resolutionLst.size() )
            {
                m_ArrayPicProInfo[QString(resolutionLst[nIndex].c_str()).trimmed()] = true;
            }
        }
        else
        {
            m_bPicturelst.push_back( false );

            if ( nIndex < resolutionLst.size() )
            {
                m_ArrayPicProInfo[QString(resolutionLst[nIndex].c_str()).trimmed()] = false;
            }
        }
    }
    //END ADD.

    if(!readPictureInfo(fp, picinfo))
    {
        return -1;
    }

    return 0;
}

bool GPARAM::readPictureInfo(FILE *fp, string str)
{
    if(str.length()<1)
    {
        return false;
    }
    vector<string> resolutionLst;
    if(!splitStr(str, '|', resolutionLst))
    {
        return false;
    }
    vector<string>::iterator it;
    for(it=resolutionLst.begin(); it!=resolutionLst.end(); it++)
    {
        Picture pic;
        QString strSize((*it).c_str());
        strSize = strSize.trimmed();
        memset(&pic, 0, sizeof(Picture));
        //strcpy_s(pic.type, 5, strSize.toStdString().c_str());
        strncpy(pic.type, strSize.toStdString().c_str(), 5);

        if(!readIntDigitalVal(fp, pic.type, "Width", pic.width))
        {
            return false;
        }

        if(!readIntDigitalVal(fp, pic.type, "Height", pic.height))
        {
            return false;
        }

        if(!readIntDigitalVal(fp, pic.type, "SignalBlank", pic.signalBlank))
        {
            return false;
        }

        if(!readIntDigitalVal(fp, pic.type, "FontSize", pic.fontSize))
        {
            return false;
        }

        if(!readIntDigitalVal(fp, pic.type, "DrawCircle", pic.drawCircle))
        {
            return false;
        }

        //当画距离圈时，才读是否画距离圈上的文字的字段
        if(pic.drawCircle != 0)
        {
            if(!readIntDigitalVal(fp, pic.type, "DrawDistance", pic.drawDistance))
            {
                return false;
            }
        }

        if(!readIntDigitalVal(fp, pic.type, "DrawMap", pic.drawMap))
        {
            return false;
        }

        if(!readIntDigitalVal(fp, pic.type, "DrawInfo", pic.drawInfo))
        {
            return false;
        }

        if(!readIntDigitalVal(fp, pic.type, "DrawColor", pic.drawColor))
        {
            return false;
        }
        if(!readIntDigitalVal(fp, pic.type, "Transparent", pic.transparent))
        {
            return false;
        }

        //当图片不透明时，才读背景色
        if(pic.transparent == 0)
        {
            if(!readBackColor(pic, fp))
            {
                return false;
            }
        }

        pic.enable = m_ArrayPicProInfo[pic.type];
        Picturelst.push_back(pic);
    }

    if(Picturelst.size()<1)
    {
        return false;
    }

    return true;
}

bool GPARAM::readBackColor(Picture &pic, FILE *fp)
{
    //读背景颜色值
    string backColor;
    if(!readIniFileC(fp, pic.type, "BackColor", backColor))
    {
        fclose(fp);
        return false;
    }
    vector<string> rgblst;
    if(!splitStr(backColor, '|', rgblst) || rgblst.size()!=3)
    {
        return false;
    }
    int valid = 0;
    for(int i=0; i<3; i++)
    {
        QString strRgb(rgblst[i].c_str());
        strRgb = strRgb.trimmed();
        if(isDigital(strRgb.toStdString()))
        {
            int val = atoi(rgblst[i].c_str());
            if(val>=0 && val<=255)
            {
                pic.backColor[i] = val;
                valid++;
            }
        }
    }
    if(valid != 3)
    {
        return false;
    }
    return true;
}

int GPARAM::readProductColor()
{
    //读取颜色表
    string filename = AppPath.toStdString() + "/config/productcolor.txt";

    FILE *fp = NULL;
    if ((fp=fopen(filename.c_str(), "r")) == NULL)
    {
        return -1;
    }
    ProductColorList.clear();
    const int bufsize = 256;
    char buf[bufsize];
    memset(buf, 0, bufsize);

    fseek(fp, 0, SEEK_SET);
    while (fgets(buf, bufsize, fp) != NULL)
    {
        if(strlen(buf)<1 || buf[0]== '\n' || buf[0]==';' || buf[0] == '#')
        {
            memset(buf, 0, bufsize);
            continue;
        }
        if (buf[0] == '[')
        {
            buf[strlen(buf) - 1] = '\0';
            string strLine = cTrim(buf);
            memset(buf, 0, bufsize);

            PRODUCTRGBCOLOR pcolor;
            int pos = strLine.find_first_of(']');
            pcolor.name = strLine.substr(1, pos-1).c_str();

            while((fgets(buf, bufsize, fp)) != NULL)
            {
                if(strlen(buf)<1 || buf[0]== '\n' || buf[0]==';')
                {
                    memset(buf, 0, bufsize);
                    continue;
                }

                buf[strlen(buf) - 1] = '\0';
                if(buf[0] == '[')
                {
                    int fpos = ftell(fp);
                    int buflen = strlen(buf);
                    fseek(fp, fpos-buflen-2, SEEK_SET);
                    memset(buf, 0, bufsize);
                    break;
                }

                strLine = cTrim(buf);
                memset(buf, 0, bufsize);
                vector<string> sl;
                if(!splitStr(strLine, ',', sl))
                {
                    continue;
                }
                if (sl.size() == 5)
                {
                    RGBCOLOR rgbColor;
                    rgbColor.r = atoi(sl[0].c_str());
                    rgbColor.g = atoi(sl[1].c_str());
                    rgbColor.b = atoi(sl[2].c_str());
                    rgbColor.value = atof(sl[3].c_str());
                    rgbColor.realValue = sl[4].c_str();
                    pcolor.rgbColorLst.push_back(rgbColor);
                }
            }
            ProductColorList.push_back(pcolor);
        }
    }
    fclose(fp);
    return 0;
}

void GPARAM::readProductSet()
{
    productSetVec.clear();
    // 读取产品列表
    QString filename = AppPath + "/config/ProductSet.txt";
    QFile qfile(filename);
    if(!qfile.open(QFile::ReadOnly))
    {
        QSLOG_DEBUG(QString("Failed to open []").arg(filename));
        return;
    }
    char buf[1024] = {0};
    while (qfile.readLine(buf,sizeof(buf)) != -1)
    {
        QString strTemp(buf);
        strTemp = strTemp.trimmed();
        QStringList strList = strTemp.split(",");
        ProductSet ps;
        if (strList.size() != 6)
        {
            continue;
        }
        ps.productID = strList.at(0);
        ps.productName = strList.at(1);
        ps.type = strList.at(2).toInt();
        ps.dataFlag = strList.at(3).toInt();
        ps.picFlag = strList.at(4).toInt();
        ps.productType = strList.at(5);
        productSetVec.push_back(ps);
    }
    qfile.close();

    QSLOG_DEBUG(QString("Finished to readProductSet"));
}

//////////////////////////////////////////////////////////////////////////
//BEGIN ADD BY WEIKANGC 2016-01-25

//bool GPARAM::ReadProductColorCHName()
//{
//    bool bFlag = true;
//
//    QString qStrFileName = AppPath + "/config/colorname.txt";
//
//    QFile qFile( qStrFileName );
//
//    if ( qFile.exists() )
//    {
//        if ( qFile.open(QFile::ReadOnly) )
//        {
//            char chBuffer[256] =  { 0 };
//            while( -1 != qFile.readLine(chBuffer, sizeof(chBuffer)) )
//            {
//                QString qTempStr( chBuffer );
//
//                QStringList qStrList = qTempStr.split(",");
//
//                if ( 2 == qStrList.size() )
//                {
//                    m_ArrayColorCHName[qStrList.at(0)] = qStrList.at(1);
//                }
//                else
//                {
//                    bFlag = false;
//                }
//            }
//        }
//        else
//        {
//            bFlag = false;
//        }
//    }
//    else
//    {
//        bFlag = false;
//    }
//
//
//    return bFlag;
//}

//QString GPARAM::ReadCHNameByENName(const QString& qStrENName)
//{
//    return m_ArrayColorCHName.value(qStrENName)	;
//}

//bool GPARAM::ReadLastFileDateTime() //哈哈 走了个捷径.
//{
//    bool bFlag = true;
//
//// 	string strCfgPath = getExeFilePath("/config/latestFile.txt");
//// 	ifstream ifs(strCfgPath.c_str());
////
//// 	if (ifs)
//// 	{
//// 		string strLine; // 保存单行数据
//// 		pair<QString, QString> t_oElement;
//// 		while (getline(ifs, strLine))
//// 		{
//// 			size_t index;
//// 			if (string::npos != (index = strLine.find(c_stationid)))
//// 			{
//// 				t_oElement.first = QString( strLine.substr(c_stationid.length()).c_str() );
//// 				continue;
//// 			}
//// 			if (string::npos != (index = strLine.find(c_latesttime)))
//// 			{
//// 				t_oElement.second = QString( strLine.substr(c_latesttime.length()).c_str() );
//// 				m_ArrayReadFileDateTime.insert( t_oElement.first, t_oElement.second );
//// 				continue;
//// 			}
//// 		}
////
//// 		ifs.close();
//// 	}
//// 	else
//// 	{
//// 		bFlag = false;
//// 	}
//
//
//    return bFlag;
//}
//
//bool GPARAM::ReadScanSetInfo()
//{
//    bool bFlag = true;
//
//// 	string filename = AppPath.toStdString() + "/config/scanDirSet.ini";
//// 	FILE *fp = NULL;
//// 	if((fp=fopen(filename.c_str(), "r")) == NULL)
//// 	{
//// 		bFlag = false;
//// 	}
////
//// 	if ( bFlag )
//// 	{
//// 		string dirInfo;
//// 		if(readIniFileC(fp, "DIRECTORY", "DIRS", dirInfo))
//// 		{
//// 			ReadScanSetInfo( fp, dirInfo );
//// 		}
//// 		else
//// 		{
//// 			fclose(fp);
//// 			bFlag = false;
//// 		}
////
//// 		fclose(fp);
//// 	}
//
//    return bFlag;
//}

// bool GPARAM::ReadScanSetInfo(FILE* fp, string str)
// {
// 	if(str.length()<1)	return false;
//
// 	vector<string> resolutionLst;
//
// 	QString stEx(  str.c_str() );
// 	QStringList strList = stEx.split(",");
//
// 	for ( int i = 0 ; i < strList.size(); ++i )
// 	{
// 		resolutionLst.push_back( strList[i].toStdString() );
// 	}
//
// 	vector<string>::iterator it=resolutionLst.begin();
//
// 	for(; it !=resolutionLst.end(); it++)
// 	{
// 		SCANSETINFO scanSetInfo;
// 		memset(&scanSetInfo, 0, sizeof(scanSetInfo));
//
// 		QString strSize((*it).c_str());
// 		strSize = strSize.trimmed();
//
// 		if( !readIniFileC( fp, strSize.toStdString().c_str(), "PATH", scanSetInfo.scanPath ) )
// 		{
// 			fclose(fp);
// 			return false;
// 		}
//
// 		string strTimer;
// 		if( !readIniFileC( fp, strSize.toStdString().c_str(), "TIMER", strTimer ) )
// 		{
// 			fclose(fp);
// 			return false;
// 		}
//
// 		QString temp( strTimer.c_str());
// 		temp.remove("\"");
// 		strTimer = temp.toStdString();
//
// 		vector<string> timerList;
// 		if(!splitStr(strTimer, ',', timerList))
// 		{
// 			fclose(fp);
// 			return false;
// 		}
//
// 		if (timerList.size() != 6)
// 		{
// 			fclose(fp);
// 			return false;
// 		}
//
// 		scanSetInfo.dTI.nSecond	=	timerList[0];
// 		scanSetInfo.dTI.nMinute	=	timerList[1];
// 		scanSetInfo.dTI.nHour		=	timerList[2];
// 		scanSetInfo.dTI.nDay		=	timerList[3];
// 		scanSetInfo.dTI.nMonth	=	timerList[4];
// 		scanSetInfo.dTI.nWeek		=	timerList[5];
//
//
// 		string strMatch;
// 		if( !readIniFileC( fp, strSize.toStdString().c_str(), "MATCH", strMatch ) )
// 		{
// 			fclose(fp);
// 			return false;
// 		}
//
// 		vector<string> matchList;
// 		if(!splitStr(strMatch, ',', matchList))
// 		{
// 			fclose(fp);
// 			return false;
// 		}
//
// 		vector<string>::size_type nIndex = 0 ;
// 		for ( ; nIndex < matchList.size(); ++nIndex )
// 		{
// 			QString temp( matchList[nIndex].c_str() );
// 			temp.trimmed();
// 			scanSetInfo.matchStr.push_back( temp.toStdString() );
// 		}
//
// 		scanSetInfo.dirName = strSize.toStdString();
// 		m_ArrayScanSetInfo.push_back( scanSetInfo );
// 	}
// 	return true;
// }

//bool GPARAM::ReadWprdConfigInfo()
//{
//    QSettings setting( AppPath + "/config/WprdParam.ini", QSettings::IniFormat, 0 );
//    setting.setIniCodec("GBK");
//
//    this->m_strWPRDPath = setting.value("Path/WorkPath", "").toString();
//    this->m_strProdPath	= setting.value("Path/SavePath", "").toString();
//
//    this->m_strRLastTime = setting.value("OPTION/lasttime_ROBS", "").toString();
//    this->m_strHLastTime = setting.value("OPTION/lasttime_HOBS", "").toString();
//    this->m_strOLastTime = setting.value("OPTION/lasttime_OOBS", "").toString();
//
//    this->m_strRDispatcher = setting.value("DISPATCH/ROBS", "").toString();
//    this->m_strHDispatcher = setting.value("DISPATCH/HOBS", "").toString();
//    this->m_strHDispatcher.replace("#", ",");
//    this->m_strODispatcher = setting.value("DISPATCH/OOBS", "").toString();
//
//    return true;
//}

//void GPARAM::ReadWprdLastTime(int nType, QString& strLastTime)
//{
//    switch( nType )
//    {
//    case 0:
//    {
//        strLastTime = m_strRLastTime;
//    }
//    break;
//    case 1:
//    {
//        strLastTime = m_strHLastTime;
//    }
//    break;
//    case 2:
//    {
//        strLastTime = m_strOLastTime;
//    }
//    break;
//    default:
//    {
//        strLastTime = m_strRLastTime;
//    }
//    break;
//    }
//}

//void GPARAM::WriteWprdLastTime(int nType, QString strLastTime)
//{
//    QMutexLocker mlocker(&m_mLock);
//
//    QString strFlag = "OPTION/lasttime_";
//    switch( nType )
//    {
//    case 0:
//    {
//        m_strRLastTime = strLastTime;
//        strFlag += "ROBS";
//    }
//    break;
//    case 1:
//    {
//        m_strHLastTime = strLastTime;
//        strFlag += "HOBS";
//    }
//    break;
//    case 2:
//    {
//        m_strOLastTime = strLastTime;
//        strFlag += "OOBS";
//    }
//    break;
//    default:
//        break;
//    }
//
//    QSettings setting(AppPath + "/config/WprdParam.ini", QSettings::IniFormat, 0);
//    setting.setIniCodec("GBK");
//    setting.setValue(strFlag, strLastTime);
//}

//void GPARAM::WriteWprdPath()
//{
//    QSettings setting( AppPath + "/config/WprdParam.ini", QSettings::IniFormat, 0 );
//    setting.setIniCodec("GBK");
//
//
//    setting.setValue( "Path/WorkPath", this->m_strWPRDPath );
//    setting.setValue( "Path/SavePath" , this->m_strProdPath );
//}

//void GPARAM::WriteDispatcher()
//{
//    QSettings setting( AppPath + "/config/WprdParam.ini", QSettings::IniFormat, 0 );
//    setting.setIniCodec("GBK");
//    setting.setValue( "DISPATCH/ROBS", this->m_strRDispatcher.replace(",", "#") );
//    setting.setValue( "DISPATCH/HOBS", this->m_strHDispatcher.replace(",", "#") );
//    setting.setValue( "DISPATCH/OOBS", this->m_strODispatcher.replace(",", "#") );
//}

//void GPARAM::WriteScan()
//{
//    //QSettings setting( AppPath + "/config/WprdParam.ini", QSettings::IniFormat, 0 );
//    //setting.setIniCodec("GBK");
//
//    //setting.setValue( "OPTION/lasttime_ROBS", this->m_strRLastTime );
//    //setting.setValue( "OPTION/lasttime_HOBS", this->m_strHLastTime );
//    //setting.setValue( "OPTION/lasttime_OOBS", this->m_strOLastTime );
//
//    WriteWprdLastTime( 0, this->m_strRLastTime );
//    WriteWprdLastTime( 1, this->m_strHLastTime );
//    WriteWprdLastTime( 2, this->m_strOLastTime );
//}

//void GPARAM::WriteStatup(int nType, int nStartup)
//{
//    QMutexLocker mLocker(&m_mLock);
//
//    QSettings setting( AppPath + "/config/startup.ini", QSettings::IniFormat, 0 );
//    setting.setIniCodec("GBK");
//
//    QString strFlag;
//
//    switch( nType )
//    {
//    case 0:
//    {
//        strFlag = "MOSAIC";
//    }
//    break;
//    case 1:
//    {
//        strFlag = "WEATHER";
//    }
//    break;
//    case 2:
//    {
//        strFlag = "WPRD";
//    }
//    break;
//    default:
//        break;
//    }
//
//    strFlag +="/start";
//
//    setting.setValue(strFlag, nStartup);
//}

//bool GPARAM::ReadStartupConfig()
//{
//    QSettings setting( AppPath + "/config/startup.ini", QSettings::IniFormat, 0 );
//    setting.setIniCodec("GBK");
//
//    this->m_nMosaic = setting.value("MOSAIC/start", 0).toInt();
//    this->m_nWeather= setting.value("WEATHER/start", 1).toInt();
//    this->m_nWprd	= setting.value("WPRD/start", 0).toInt();
//
//    return true;
//}

int GPARAM::genIndexTable()
{
    string fileName;

    CPathBuilder cpath;
    fileName = cpath.getConfigPath().toStdString() + "/datatable.system";

    QSLOG_DEBUG(QString("Begin to read file [%1]!").arg(fileName.c_str()));

    RMatrix.resize(5000 * 5000);
    AzMatrix.resize(5000 * 5000);

    if (QFile::exists(fileName.c_str()))
    {
        // 文件存在，读取内容
        gzFile file = NULL;
        file = gzopen(fileName.c_str(), "rb");
        if (file == NULL)
        {
            QSLOG_DEBUG("Fail to open [Config/datatable.system]!");
            return -1;
        }
        else
        {
            gzread(file, RMatrix.data(), 5000*5000*sizeof(unsigned short));
            gzread(file, AzMatrix.data(), 5000*5000*sizeof(unsigned short));
            gzclose(file);
        }
    }
    else
    {
        // 文件不存在，重新创建
        QSLOG_DEBUG("[datatable.system] is not exist, now generate it!");
        for (int i = 0; i < 5000; i++)
        {
            for (int j = 0; j < 5000; j++)
            {
                float r, az;
                getPPIAzAndR(i, j, 5000, 5000, &r, &az);
                RMatrix[i * 5000 + j] = qRound(r * 10);
                AzMatrix[i * 5000 + j] =qRound(az * 100);
            }
        }

        gzFile file = NULL;
        file = gzopen(fileName.c_str(), "wb");
        if (file == NULL)
        {
            QSLOG_DEBUG("Fail to write [Config/datatable.system]!");
            return -1;
        }
        gzwrite(file, RMatrix.data(), 5000*5000*sizeof(unsigned short));
        gzwrite(file, AzMatrix.data(), 5000*5000*sizeof(unsigned short));
        gzclose(file);

        QSLOG_DEBUG("Succeed to write [datatable.system]!");
    }

    QSLOG_DEBUG(QString("Finish to read [%1]!").arg(fileName.c_str()));
    return 0;
}

void GPARAM::getPPIAzAndR(int X,int Y,int PicWidth,int DisWidth,float* R,float* AZ)
{
    int CenterX, CenterY, MaxR;
    CenterX = qRound((qreal)PicWidth/ 2);
    CenterY = qRound((qreal)PicWidth/ 2);
    MaxR = qMin(qRound((qreal)DisWidth/ 2),qRound((qreal)PicWidth/ 2));
    X = X - CenterX;
    Y =CenterY - Y;

    if ((X == 0) && (Y == 0))
    {
        *R = 0.0;
        *AZ = 0.0;
        return;
    }

    if ((X != 0) && (Y == 0))
    {
        if (X> 0)
        {
            *R = X;
            *AZ = 90.0;
        }
        else
        {
            *R = 0-X;
            *AZ = 270.0;
        }
    }

    *R = sqrt(float (X * X + Y * Y));
    if (*R> MaxR)
    {
        *R = 0.0;
        *AZ = 0.0;
        return;
    }
    float Tmp;
    Tmp = atan2(float(X), float(Y))*180.0/PI;
    if (Tmp < 0)
    {
        *AZ = 360 + Tmp;
    }
    else
    {
        *AZ = Tmp;
    }
}

void GPARAM::primaryBaseInfo()
{
    gMaxL.insert(19,230);
    gMaxL.insert(20,460);
    gMaxL.insert(26,115);
    gMaxL.insert(27,230);
    gMaxL.insert(37,230);
    gMaxL.insert(38,460);
    gMaxL.insert(41,230);
    gMaxL.insert(56,230);
    gMaxL.insert(57,230);
    gMaxL.insert(78,230);
    gMaxL.insert(79,230);
    gMaxL.insert(80,230);
    gMaxL.insert(110,230);

    gProductUnit.insert(19,"dbz");
    gProductUnit.insert(20,"dbz");
    gProductUnit.insert(26,"m/s");
    gProductUnit.insert(27,"m/s");
    gProductUnit.insert(37,"dbz");
    gProductUnit.insert(38,"dbz");
    gProductUnit.insert(41,"km");
    gProductUnit.insert(48,"RMS");
    gProductUnit.insert(56,"m/s");
    gProductUnit.insert(57,"kg/m2");
    gProductUnit.insert(78,"mm");
    gProductUnit.insert(79,"mm");
    gProductUnit.insert(80,"mm");
    gProductUnit.insert(110,"dbz");

    gColorName.insert(19,"DBZ");
    gColorName.insert(20,"DBZ");
    gColorName.insert(26,"V");
    gColorName.insert(27,"V");
    gColorName.insert(37,"DBZ");
    gColorName.insert(38,"DBZ");
    gColorName.insert(41,"ET");
    gColorName.insert(48,"VWP");
    gColorName.insert(56,"SRM");
    gColorName.insert(57,"VIL");
    gColorName.insert(78,"OHP");
    gColorName.insert(79,"THP");
    gColorName.insert(80,"STR");
    gColorName.insert(110,"DBZ");

    gAAdV.insert(19,-2);
    gAAdV.insert(20,-2);
    gAAdV.insert(26,-2);
    gAAdV.insert(27,-2);
    gAAdV.insert(37,-2);
    gAAdV.insert(38,-2);
    gAAdV.insert(41,-1);
    gAAdV.insert(48,0);
    gAAdV.insert(56,1);
    gAAdV.insert(57,1);
    gAAdV.insert(78,1);
    gAAdV.insert(79,1);
    gAAdV.insert(80,1);
    gAAdV.insert(110,-2);

    gmulV.insert(19,5);
    gmulV.insert(20,5);
    gmulV.insert(26,5);
    gmulV.insert(27,5);
    gmulV.insert(37,5);
    gmulV.insert(38,5);
    gmulV.insert(41,1.5f);
    gmulV.insert(48,0);
    gmulV.insert(56,10);
    gmulV.insert(57,5);
    gmulV.insert(78,10);
    gmulV.insert(79,10);
    gmulV.insert(80,10);
    gmulV.insert(110,5);

    m_BinWidthmap.insert(19,1);
    m_BinWidthmap.insert(20,2);
    m_BinWidthmap.insert(26,0.5);
    m_BinWidthmap.insert(27,1);
    m_BinWidthmap.insert(37,1);
    m_BinWidthmap.insert(38,4);
    m_BinWidthmap.insert(41,4);
    m_BinWidthmap.insert(56,1);
    m_BinWidthmap.insert(57,4);
    m_BinWidthmap.insert(78,2);
    m_BinWidthmap.insert(79,2);
    m_BinWidthmap.insert(80,2);
    m_BinWidthmap.insert(110,1);

    Resolution.insert(19,1);
    Resolution.insert(20,2);
    Resolution.insert(26,0.5f);
    Resolution.insert(27,1);
    Resolution.insert(37,1);
    Resolution.insert(38,4);
    Resolution.insert(41,4);
    Resolution.insert(56,1);
    Resolution.insert(57,4);
    Resolution.insert(78,2);
    Resolution.insert(79,2);
    Resolution.insert(80,2);
    Resolution.insert(110,1);
}

//END ADD.