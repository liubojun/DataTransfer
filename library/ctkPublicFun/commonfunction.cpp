#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
#include <Windows.h>
#endif

#ifdef _WIN32
	#include <windows.h>
#else // linux stuff
	#include <sys/vfs.h>
	#include <sys/stat.h>
#endif // _WIN32

#include "commonfunction.h"
#include <math.h>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication>
#include <regex>
//#include "Qlog.h"

float DegToRad(float deg)
{
  return deg * PI / 180.0;
}

float RadToDeg(float rad)
{
	return rad * 180.0 / PI;
}
QString extractFileName(const QString & fileName)
{
	QString retStr=fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
	return retStr;
}

void deleteDirs(const QString & Path)
{
	QDir dir(Path);
	//先删除该目录下的文件
	QStringList FilesList = dir.entryList(QDir::Files);
	for (int j = 0; j < FilesList.count(); j++)
	{

		QString FileName = FilesList.at(j);
		FileName = dir.absolutePath() + "/" + FileName;
		QFile::remove(FileName);
	}

	//在进入子目录，删除里面的内容
	FilesList = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
	for (int j = 0; j < FilesList.count(); j++)
	{

		QString FilePath = FilesList.at(j);
		FilePath=dir.absolutePath()+"/"+FilePath;
		deleteDirs(FilePath);
		dir.rmdir(FilePath);
	}
}

bool FileIsValid(QString filename)
{
	if (filename.contains(".lck"))
		return false;

	QFile fileLck(filename+".lck");
	if (fileLck.exists())
		return false;
	QFile file(filename);
	if (!file.exists())
		return false;
	if (!file.open(QIODevice::ReadWrite))
	    return false;
	file.close();
	return true;
}

void createFileLocker(const QString& filename)
{
	QString fn=filename+".lck";
	QFile file(fn);
	file.open(QIODevice::WriteOnly);
	file.putChar('1');
	file.close();
}

void deleteFileLocker(const QString& filename)
{
	QString fn=filename+".lck";
	if (QFile::exists(fn))
	{
		QFile::remove(fn);
	}
}

bool DllFileExist(const QString& filename)
{
	QString fn;
#ifdef  WIN32
	fn=filename+".dll";
#else
	fn=filename+".so";
#endif
	return QFile::exists(fn);

}

void myMessageOutput(QtMsgType type, const char *msg)
{
	switch (type)
	{
	case QtDebugMsg:
		//fprintf(stderr, "Debug: %s\n", msg);
		//QSLOG_INFO(QString("Debug:%1").arg(msg));
		break;
	case QtWarningMsg:
		//fprintf(stderr, "Warning: %s\n", msg);
		//QSLOG_INFO(QString("Warning:%1").arg(msg));
		break;
	case QtCriticalMsg:
		//fprintf(stderr, "Critical: %s\n", msg);
		//QSLOG_INFO(QString("Critical:%1").arg(msg));
		break;
	case QtFatalMsg:
		//fprintf(stderr, "Fatal: %s\n", msg);
		//QSLOG_INFO(QString("Fatal:%1").arg(msg));
		abort();
	}
}

bool createDir(const QString& directory)
{
	QDir dir(directory);
	if (dir.exists())
		return false;
	else
		return dir.mkdir(directory);
}

bool DirExist(const QString& directory)
{
	QDir dir(directory);
	return dir.exists();
}

void readMem(char* mem, char*& buffer, unsigned long size)
{
	memcpy(mem, buffer, size);
	buffer=buffer+size;
}

QString IntToStrFixWidth(int i, int width)
{
	QString str;
	str=QString("%1").arg(i);
	int len=str.length();
	if (len>width)
		return QString("");
	if (len==width)
		return str;
	len=width-len;
	for (int i=0; i<len; i++)
	{
		str="0"+str;
	}
	return str;
}

void output(const QString& debugInfo)
{
	QTextStream txt(stdout);
	txt<<debugInfo<<"\n";
}

void output(const QString& debugCap, const float debugV)
{
	QTextStream txt(stdout);
	QString str;
	str=str.sprintf("%f", debugV);
	txt<<debugCap<<": "<<str<<"\n";
}

void output(const QString& debugCap,const QString& debugStr)
{
	QTextStream txt(stdout);
	txt<<debugCap<<": "<<debugStr<<"\n";
}

void output(const float debugV1, const float debugV2)
{
	QTextStream txt(stdout);
	QString str;
	str=str.sprintf("%f: %f", debugV1, debugV2);
	txt<<str<<"\n";
}

void QStringToChar(const QString& str, char* c, int len)
{
	memset(c, 0, len);
	sprintf(c, "%s", qPrintable(str));
}

//快速排序算法，对数据从左L个到右R个进行快速排序，抄的，知道用法即可
void QuickSort(unsigned char* array, int L, int R)
{
	int I, J;
	unsigned char X, Y;

	I=L;
	J=R;
	X=array[((L+R)/2)];

	while (I <= J)
	{
		while (array[I] < X && I < R)
		{
			I = I + 1;
		}
		while (X < array[J] && J> L)
		{
			J = J - 1;
		}
		if (I <= J)
		{
			Y = array[I];
			array[I] = array[J];
			array[J] = Y;
			I = I + 1;
			J = J - 1;
		}
	}

	if (L<J)
	QuickSort(array, L, J);
	if (I<R)
	QuickSort(array, I, R);
}

void QuickSort(char* array, int L, int R)
{
	int I, J;
	char X, Y;

	I=L;
	J=R;
	X=array[((L+R)/2)];

	while (I <= J)
	{
		while (array[I] < X && I < R)
		{
			I = I + 1;
		}
		while (X < array[J] && J> L)
		{
			J = J - 1;
		}
		if (I <= J)
		{
			Y = array[I];
			array[I] = array[J];
			array[J] = Y;
			I = I + 1;
			J = J - 1;
		}
		//gIterations = gIterations + 1
	}

	if (L<J)
	QuickSort(array, L, J);
	if (I<R)
	QuickSort(array, I, R);
}

void GetJulianDate(unsigned short &year, unsigned char &month, unsigned char &day, unsigned short JulianDate)
{
	int JLDAYN = JulianDate+2440587;
	int L = JLDAYN + 68569;
	int N = 4 * L / 146097;
	L = L - (146097 * N + 3) / 4;
	int I = 4000 * (L + 1) / 1461001;
	L = L - 1461 * I / 4 + 31;
	int J = 80 * L / 2447;
	day = L - 2447 * J / 80;
	L = J / 11;
	month = J + 2 - 12 * L;
	year = 100 * (N - 49) + I + L;
}

void GetMilliTime(unsigned char &hour, unsigned char &min, unsigned char &sec, unsigned long liMilliSeconds)
{
	unsigned long int Seconds=liMilliSeconds/1000;
	hour = (unsigned char)(Seconds/3600);
	min= (unsigned char)((Seconds-hour*3600)/60) ;
	sec = (unsigned char)(Seconds -(60*hour+min)*60 );
}

float get2PointDistance(float long1, float lat1, float long2, float lat2)
{
	float radLat1, radLat2;
	float a, b, s;

	radLat1 = DegToRad(lat1);
	radLat2 = DegToRad(lat2);
	a = radLat1 - radLat2;
	b = DegToRad(long1) - DegToRad(long2);
	s = 2 * asin(sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2)));
	s = s * EARTH_RADIUS;
	return s;
}

bool getFreeTotalSpace(const QString& sDirPath,double& fTotal, double& fFree)
{
	float fKB=1024;
#ifdef _WIN32
	QString sCurDir = QDir::current().absolutePath ();
	QDir::setCurrent( sDirPath );

	ULARGE_INTEGER free,total;
	bool bRes = ::GetDiskFreeSpaceExA( 0 , &free , &total , NULL );
	if ( !bRes ) return false;

	QDir::setCurrent( sCurDir );

	fFree = static_cast<double>( static_cast<__int64>(free.QuadPart) ) / fKB;
	fTotal = static_cast<double>( static_cast<__int64>(total.QuadPart) ) / fKB;

#else //linux

	struct stat stst;
	struct statfs stfs;

	if ( ::stat(sDirPath.local8Bit(),&stst) == -1 ) return false;
	if ( ::statfs(sDirPath.local8Bit(),&stfs) == -1 ) return false;

	fFree = stfs.f_bavail * ( stst.st_blksize / fKB );
	fTotal = stfs.f_blocks * ( stst.st_blksize / fKB );

#endif // _WIN32
	return true;
}


int getCpuCount()
{
    int cpuCount = 1;

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
    {
    	SYSTEM_INFO    si;
    	GetSystemInfo(&si);
    	cpuCount = si.dwNumberOfProcessors;
    }
#elif defined(Q_OS_UNIX) && !defined(Q_OS_MACX)
    cpuCount = sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(Q_OS_MACX)
   kern_return_t		kr;
   struct host_basic_info	hostinfo;
   unsigned int			count;

   count = HOST_BASIC_INFO_COUNT;
   kr = host_info(mach_host_self(), HOST_BASIC_INFO, (host_info_t)&hostinfo, &count);
   if(kr == KERN_SUCCESS) {
	cpuCount = hostinfo.avail_cpus;
//	totalMemory = hostinfo.memory_size; 			 //in bytes
   }
#endif

    if( cpuCount < 1 )
	cpuCount = 1;

    return cpuCount;
}

float cRound(float x)
{
	x += 0.5f;
	return floor(x);
}

string getExeFilePath(const char* filePath)
{
	string strPath;
    QString strFileName = QCoreApplication::applicationDirPath()+filePath;
	strPath = strFileName.toStdString();
	return strPath;
}

// bool splitStr(const string &str, char split, vector<string> &strLst)
// {
// 	int to = 0;
// 	int from = 0;
// 	while((to=str.find(split, from))!= -1)
// 	{
// 		string tmp = str.substr(from, to-from);
// 		from = to + 1;
// 		if(!tmp.empty())
// 		{
// 			strLst.push_back(tmp);
// 		}
// 	}
// 	string val = str.substr(from,str.length()-from);
// 	val = cTrim(val);
// 	if(!val.empty())
// 	{
// 		strLst.push_back(val);
// 	}
// 	if(strLst.size() < 1)
// 	{
// 		return false;
// 	}
// 	return true;
// }

string cTrim(const string &str)
{
	string tstr;
	QString qstr;
	qstr.sprintf("%s",str.c_str());
	qstr.trimmed();
	tstr = qstr.toStdString();
	return tstr;
}

// bool isDigital(const string &str)
// {
// 	//遍历每个字符，发现不是数字0~9('48'~'57')或'.'46，即返回false
// 	int dotc = 0;
// 	string::const_iterator it;
// 	for(it=str.begin(); it!=str.end(); it++)
// 	{
// 		if(*it==46)
// 		{
// 			dotc++;
// 		}
// 		if((*it!=46) && (*it<48 || *it>57))
// 		{
// 			return false;
// 		}
// 	}
// 	//小数点的个数>=1个时，判断为非有效数字
// 	if(dotc>1)
// 	{
// 		return false;
// 	}
// 	return true;
// }
bool matchStr(const char *str,const char* strRegex)
{
	regex pattern(strRegex);
	return regex_match(str,pattern);
}
// bool olderThen(const File_info_t &f1, const File_info_t &f2)
// {
// 	return f1.strTimeInFN.toStdString() < f2.strTimeInFN.toStdString();
// }

void getAllFiles(FileInfoList &fileList,QString strPath)
{
	QDir qdir(strPath);
	QFileInfoList qstrList = qdir.entryInfoList();
	for (int i=0; i< qstrList.size();i++)
	{
		QFileInfo qfileInfo = qstrList.at(i);
		if (qfileInfo.fileName() == "." || qfileInfo.fileName() == "..")
		{
			continue;
		}       
		if (qfileInfo.isFile())
		{
			File_info_t fileinfo;
			fileinfo.strFilePath = qfileInfo.filePath();
			fileinfo.strFilePath.replace("\\","/");
			fileinfo.strFileName = qfileInfo.fileName();
			fileinfo.lastModifyTime = qfileInfo.lastModified();
			vector<string> vecTemp;
			string fileName = qfileInfo.fileName().toStdString();
			if (matchStr(fileName.c_str(),"VTB[0-9]{14}.[0-9]{3}.zip") || 
				matchStr(fileName.c_str(),"VTB[0-9]{14}.[0-9]{3}.[0-9]{5}"))
			{
				splitStr(fileName.c_str(),'.',vecTemp);
				if (vecTemp.size() != 3)
				{
					continue;
				}
				fileinfo.strTimeInFN = QString(vecTemp.at(0).substr(3,14).c_str());
				fileList.push_back(fileinfo);
				continue;
			}
		}
		else if(qfileInfo.isDir())
		{
			getAllFiles(fileList,qfileInfo.filePath());		   
		}
		else
		{
			continue;
		}
	}
}

void getRdfFiles(FileInfoList &fileList,QString strPath)
{
	QDir qdir(strPath);
	QFileInfoList qstrList = qdir.entryInfoList();
	for (int i=0; i< qstrList.size();i++)
	{
		QFileInfo qfileInfo = qstrList.at(i);
		if (qfileInfo.fileName() == "." || qfileInfo.fileName() == "..")
		{
			continue;
		}       
		if (qfileInfo.isFile())
		{
			File_info_t fileinfo;
			fileinfo.strFilePath = qfileInfo.filePath();
			fileinfo.strFilePath.replace("\\","/");
			fileinfo.strFileName = qfileInfo.fileName();
			fileinfo.lastModifyTime = qfileInfo.lastModified();
			vector<string> vecTemp;
			QString strfileName = qfileInfo.fileName();
			QStringList qfileList =  strfileName.split("_");
			if (qfileList.length() == 6 && strfileName.endsWith(".rdf"))
			{
                fileinfo.strTimeInFN = qfileList.at(0) + qfileList.at(1);
				fileList.push_back(fileinfo);
				continue;
			}
		}
		else if(qfileInfo.isDir())
		{
			getRdfFiles(fileList,qfileInfo.filePath());		   
		}
		else
		{
			continue;
		}
	}
}

// bool readIniFileC(FILE *fp, const char *group, const char *name, string &value)
// {
// 	if(fp == NULL)
// 	{
// 		return false;
// 	}
// 	//先清空，再写入
// 	value.clear();
// 	const int bufsize = 256;
// 	char buf[bufsize];
// 	memset(buf, 0, bufsize);
// 
// 	fseek(fp, 0, SEEK_SET);
// 	while((fgets(buf, bufsize, fp)) != NULL)
// 	{
// 		if(strlen(buf)<1 ||buf[0]=='\n' || buf[0]==';')
// 		{
// 			memset(buf, 0, bufsize);
// 			continue;
// 		}
// 
// 		if(buf[0]=='[')
// 		{
// 			buf[strlen(buf) -1] = '\0';
// 			string strLine = buf;
// 			memset(buf, 0, bufsize);
// 			int pos  = strLine.find_first_of(']');
// 			string gtmp = strLine.substr(1, pos-1);
// 			//找到群组
// 			if(strcmp(gtmp.c_str(), group)==0)
// 			{
// 				while((fgets(buf, bufsize, fp)) != NULL)
// 				{
// 					if(strlen(buf)<1 || buf[0]=='[')
// 					{
// 						memset(buf, 0, bufsize);
// 						break;
// 					}
// 					if(buf[0]=='\n' || buf[0]==';')
// 					{
// 						memset(buf, 0, bufsize);
// 						continue;
// 					}
// 
// 					endBuf(buf, bufsize);
// 					strLine = buf;
// 					memset(buf, 0, bufsize);
// 					//找到字段
// 					if(strLine.find(name)!= string::npos)
// 					{
// 						int pos = strLine.find_first_of('=');
// 						value = strLine.substr(pos+1, strLine.length() - pos -1);
// 						//去掉空格
// 						value =cTrim(value);
// 						if(value.length()<1)
// 						{
// 							return false;
// 						}
// 						return true;
// 					}
// 				}
// 
// 			}
// 		}
// 	}
// 	//没有找到字段值
// 	if(value.length()<1)
// 	{
// 		return false;
// 	}
// 	return true;
// }
bool readIntDigitalVal(FILE *fp, const char *group, const char *name, int &value)
{
	string strval;
	if(!readIniFileC(fp, group, name, strval) || !isDigital(strval))
	{
		return false;
	}

	value = atoi(strval.c_str());
	return true;
}
bool readFloatDigitalVal(FILE *fp, const char *group, const char *name, float &value)
{
	string strval;
	if(!readIniFileC(fp, group, name, strval) || !isDigital(strval))
	{
		return false;
	}

	value = (float)atof(strval.c_str());
	return true;
}
// void endBuf(char *buf, int bufsize)
// {
// 	if(buf[strlen(buf) -1] != '\n')
// 	{
// 		if((int)strlen(buf)<bufsize)
// 		{
// 			buf[strlen(buf)] = '\0';
// 		}
// 		return;
// 	}
// 
// 	buf[strlen(buf) -1] = '\0';
// }

bool splitNameStr(const char *file, char split, char (*buf)[MAXNAMEC], int &row)
{
	if (!buf)
	{
		return false;
	}
	for (; *file != '\0' && row < MAXNAMER; row++)
	{
		const char *pEnd = strchr(file, split);
		if (0 == pEnd)
		{
			strcpy(buf[row++], file);
			break;
		}
		strncpy(buf[row], file, pEnd - file);

		file = pEnd + 1;
	}

	if(row >= MAXNAMER)
	{
		return false;
	}
	return true;
}

void strUpper(char *buf)
{
	while (*buf != '\0')
	{
		*buf = toupper(*buf);
		buf++;
	}
}