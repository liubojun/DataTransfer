/**
  * @file   ctkPublicFun.h
  * @brief  公共方法类.
  *
  *  这里是详细说明。
  * @author nriet
  * @par   Copyright (c):
  *          nriet.
  *         All Rights Reserved
  * @date   2016:04:07
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef ctkPublicFun_h__
#define ctkPublicFun_h__

#include "../macro.h"
#include <string>
#include <vector>
#include <map>
using namespace std;
#include "commontypedef.h"
#include "../ctkProtocol/fileMsg.h"
#include <QString>
#include "../3dpart/Common/PPIStruct.h"
#include <QMap>
#include <QDateTime>
#include <QRectF>

DLL_EXPORT_DECL void GetAllDirTime(string rootdir, std::map<string,QDateTime>& dirTotime);

DLL_EXPORT_DECL void QuickSort(unsigned char* array, int L, int R);

DLL_EXPORT_DECL void QuickSort(char* array, int L, int R);

DLL_EXPORT_DECL void BinaryFindCloseValue(float value, const QVector<float> &group, int &pos);

DLL_EXPORT_C_DECL float cRound(float x);

DLL_EXPORT_C_DECL void strUpper(char *buf);

DLL_EXPORT_C_DECL bool splitStr(const string &str, char split, vector<string> &strLst);

DLL_EXPORT_DECL string cTrim(const string &str);

DLL_EXPORT_C_DECL bool readIniFileC(FILE *fp, const char *group, const char *name, string &value);

DLL_EXPORT_C_DECL void endBuf(char *buf, int bufsize);

DLL_EXPORT_C_DECL bool readIntDigitalVal(FILE *fp, const char *group, const char *name, int &value);

DLL_EXPORT_C_DECL bool isDigital(const string &str);

DLL_EXPORT_C_DECL bool isIntStr(const string &str,bool allowNegative = false);

DLL_EXPORT_C_DECL bool isValidIPv4Address(const string &str);

DLL_EXPORT_C_DECL bool olderThen(const FileInfo &f1, const FileInfo &f2);

/**
 * @brief  获取文件绝对路径
 * @param  const char * filePath：文件相对路径
 * @return DLL_EXPORT_DECL string：绝对路径
 * @note string为C++类，不能使用DLL_EXPORT_C_DECL导出
 */
DLL_EXPORT_DECL string getExeFilePath(const char* filePath);

//分割文件名，保存的buf里
DLL_EXPORT_C_DECL bool splitNameStr(const char *file, char split, char (*buf)[MAXNAMEC], int &row);

DLL_EXPORT_C_DECL void GetJulianDate(unsigned short &year, unsigned char &month,
                                     unsigned char &day, unsigned short JulianDate);

DLL_EXPORT_C_DECL void GetMilliTime(unsigned char &hour, unsigned char &min, unsigned char &sec,
                                    unsigned long liMilliSeconds);


/**
* @name			:	CreateDirByPath
* @brief		:	根据路径层级创建目录.
* @param [in]	:	strPath 目录层级路径，以"/"结尾.
* @return		:	bool 创建成功返回true, 创建失败返回false.
* @exception	:	无
*/
DLL_EXPORT_C_DECL bool CreateDirByPath(char *strPath);

/**
 * @brief: 创建路径
 * @param: QString path
 * @return: DLL_EXPORT_C_DECL bool: 创建成功返回true, 创建失败返回false.
*/
DLL_EXPORT_C_DECL bool CreatePath(QString path);

DLL_EXPORT_DECL void getRadarFileType(const string &str, RADAR_FILE_TYPE &type);

DLL_EXPORT_DECL void getFileStringType(string &str, const RADAR_FILE_TYPE &type);

/**
 * @brief: 读雷达色标配置文件
 * @param: QList<PRODUCTRGBCOLOR> & productColorLst
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool readProductColor(QList<PRODUCTRGBCOLOR> &productColorLst);
DLL_EXPORT_DECL bool readProductColorDefault(QList<PRODUCTRGBCOLOR> &productColorLst);
DLL_EXPORT_DECL bool readProductColorFile(const QString fileName, QList<PRODUCTRGBCOLOR> &productColorLst);

/**
 * @brief: 把修改后的雷达色标重新写入配置文件中
 * @param: const QList<PRODUCTRGBCOLOR> & productColorLst
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool writeProductColor(const QList<PRODUCTRGBCOLOR> &productColorLst);

/**
 * @brief: 读雷达产品名称色标配置文件
 * @param: QList<PRODUCTCODE> & productCodeLst
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool readProductCode(QString groupName, QList<PRODUCTCODE> &productCodeLst);

/**
 * @brief: 读雷达产品名称色标配置文件,根据产品英文名获取其色标名称、产品ID、产品中文名等信息
 * @param: PRODUCTCODE & productCode
 * @param: const QString & groupName
 * @param: const QString & productName
 * @return: DLL_EXPORT_DECL bool: 描述返回值
*/
DLL_EXPORT_DECL bool getProductCode(PRODUCTCODE &productCode, const QString &groupName, const QString &productName);

DLL_EXPORT_DECL bool readAllStationInfo(QVector<Station> &vecStation);

/** Converts a string to a double in a permissive way, eg allowing for incorrect
	* numbers of digits between thousand separators
	* @param string string to convert
	* @param ok will be set to true if conversion was successful
	* @returns string converted to double if possible
*/
DLL_EXPORT_DECL double permissiveToDouble( QString string, bool& ok );

/** Converts a string to an integer in a permissive way, eg allowing for incorrect
* numbers of digits between thousand separators
* @param string string to convert
* @param ok will be set to true if conversion was successful
* @returns string converted to int if possible
*/
DLL_EXPORT_DECL int permissiveToInt( QString string, bool& ok );

/**
 * @brief  连接共享目录
 * @param  const QString & strURL：共享目录路径
 * @param  const QString & strUser：用户名
 * @param  const QString & strPwd：密码
 * @return bool：连接成功返回true，否则返回false
 */
DLL_EXPORT_DECL bool connectToWinRemote(QString strURL,const QString& strUser,const QString& strPwd);

DLL_EXPORT_DECL int readMosaicFile(const QString &filename, MOSAICDATAHEADER &mosaicDataHeader, std::vector<RADARSITEINFO> &pSiteInfo,
                                   std::vector<unsigned char> &pData, std::vector<short> &pDataStartPos);

/**
 * @brief: 获取雷达组网产品文件中的产品高度层信息
 * @param: const QString & r_fileName：产品文件的全路径
 * @param: vector<QString> & r_vLevels：存放高度数值
 * @return: DLL_EXPORT_DECL int: 高度的层数
*/
DLL_EXPORT_DECL int getProductLevels(const QString& r_fileName, vector<QString>& r_vLevels);
/**
 *@brier 获得雷达文件的组网时间
 *@param const QString & r_fileName 全路径文件名
 *@return DLL_EXPORT_DECL QDateTime 返回日期时间
 */
DLL_EXPORT_DECL QDateTime getRadarFileDateTime(const QString& r_fileName);
/**
 * @brief: 获取色标名称对用单位
 * @param: const QString colorName：色标名称
 * @return: DLL_EXPORT_DECL QString: 色标单位
*/
DLL_EXPORT_DECL QString getUnitByColorName(const QString colorName);

/**
 * @brief 求两个经纬度之间的距离
 * @param double lat1
 * @param double lng1
 * @param double lat2
 * @param double lng2
 * @return DLL_EXPORT_DECL：无
 */
DLL_EXPORT_DECL double get_distance(double lat1,double lng1,double lat2,double lng2);

/**
 * @brief  根据雷达中心点的经纬度，雷达的探测距离，计算最大最小经纬度
 * @param  double lon0：经度（°）
 * @param  double lat0：纬度（°）
 * @param  int maxl：探测范围（km）
 * @return DLL_EXPORT_DECL QRectF：无
 */
DLL_EXPORT_DECL QRectF getMapExtents(double lon0, double lat0, int maxl);

/**
 * @brief: 根据区域编码的英文名获取中文名
 * @param: const QString & enName: 区域编码的英文名
 * @return: DLL_EXPORT_DECL QString: 区域编码的中文名
*/
DLL_EXPORT_DECL QString getAreaCodeZhName(const QString &enName);

#endif // ctkPublicFun_h__
