#ifndef COMMONFUNCTION_H_
#define COMMONFUNCTION_H_

#include <string>
#include <QString>
#include <vector>
#include "commontypedef.h"
using namespace std;
#include "ctkPublicFun.h"
#include "../ctkProtocol/fileMsg.h"

typedef unsigned char BYTE;

float DegToRad(float deg);
float RadToDeg(float rad);

void deleteDirs(const QString & Path);
QString extractFileName(const QString & fileName);

int getCpuCount();
bool getFreeTotalSpace(const QString& sDirPath,double& fTotal, double& fFree);

bool FileIsValid(QString filename);
bool DllFileExist(const QString& filename);

void myMessageOutput(QtMsgType type, const char *msg);

void readMem(char* mem,char*& buffer,unsigned long size);

// void QuickSort(unsigned char* array, int L, int R);
// void QuickSort(char* array, int L, int R);

// void GetJulianDate(unsigned short &year, unsigned char &month,
// 		unsigned char &day, unsigned short JulianDate);
// void GetMilliTime(unsigned char &hour, unsigned char &min, unsigned char &sec,
// 		unsigned long liMilliSeconds);

void QStringToChar(const QString& str,char* c,int len);

void output(const QString&);
void output(const QString&,const QString&);
void output(const QString&, const float);
void output(const float, const float);

QString IntToStrFixWidth(int i,int width);

bool createDir(const QString& directory);
bool DirExist(const QString& directory);
void createFileLocker(const QString& filename);
void deleteFileLocker(const QString& filename);

float get2PointDistance(float long1,float lat1,float long2,float lat2);
//float cRound(float x);

//string getExeFilePath(const char* filePath);
//bool splitStr(const string &str, char split, vector<string> &strLst);
//string cTrim(const string &str);
//bool isDigital(const string &str);
bool matchStr(const char *str,const char* strRegex);
//bool olderThen(const File_info_t &f1, const File_info_t &f2);
void getAllFiles(FileInfoList &fileList, QString strPath);
void getRdfFiles(FileInfoList &fileList,QString strPath);

//bool readIniFileC(FILE *fp, const char *group, const char *name, string &value);
//bool readIntDigitalVal(FILE *fp, const char *group, const char *name, int &value);
bool readFloatDigitalVal(FILE *fp, const char *group, const char *name, float &value);

//void endBuf(char *buf, int bufsize);
//分割文件名，保存的buf里
//bool splitNameStr(const char *file, char split, char (*buf)[MAXNAMEC], int &row);

//void strUpper(char *buf);
#endif /*COMMONFUNCTION_H_*/
