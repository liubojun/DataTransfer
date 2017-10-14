#include "ctkPublicFun.h"
#include <math.h>
#include <ctype.h>
#include <QCoreApplication>
#include <QDir>
#include "pathbuilder.h"
#include "ctkLog.h"
#include "zlib.h"
#include <QSettings>
#include <qmath.h>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <WinNetWk.h>
#endif
using namespace std;
#pragma warning(disable:4996)

#define radian(v) ((v)*PI/180.0)

////快速排序算法，对数据从左L个到右R个进行快速排序，抄的，知道用法即可
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

float cRound(float x)
{
    x += 0.5f;
    return floor(x);
}

void strUpper(char *buf)
{
    while (*buf != '\0')
    {
        *buf = toupper(*buf);
        buf++;
    }
}

bool splitStr(const string &str, char split, vector<string> &strLst)
{
    int to = 0;
    int from = 0;
    while((to=str.find(split, from))!= -1)
    {
        string tmp = str.substr(from, to-from);
        from = to + 1;
        if(!tmp.empty())
        {
            strLst.push_back(tmp);
        }
    }
    string val = str.substr(from,str.length()-from);
    val = cTrim(val);
    if(!val.empty())
    {
        strLst.push_back(val);
    }
    if(strLst.size() < 1)
    {
        return false;
    }
    return true;
}

string cTrim(const string &str)
{
    string tstr;
    QString qstr;
    qstr.sprintf("%s",str.c_str());
    qstr.trimmed();
    tstr = qstr.toStdString();
    return tstr;
}

bool readIniFileC(FILE *fp, const char *group, const char *name, string &value)
{
    if(fp == NULL)
    {
        return false;
    }
    //先清空，再写入
    value.clear();
    const int bufsize = 256;
    char buf[bufsize];
    memset(buf, 0, bufsize);

    fseek(fp, 0, SEEK_SET);
    while((fgets(buf, bufsize, fp)) != NULL)
    {
        if(strlen(buf)<1 ||buf[0]=='\n' || buf[0]==';')
        {
            memset(buf, 0, bufsize);
            continue;
        }

        if(buf[0]=='[')
        {
            buf[strlen(buf) -1] = '\0';
            string strLine = buf;
            memset(buf, 0, bufsize);
            int pos  = strLine.find_first_of(']');
            string gtmp = strLine.substr(1, pos-1);
            //找到群组
            if(strcmp(gtmp.c_str(), group)==0)
            {
                while((fgets(buf, bufsize, fp)) != NULL)
                {
                    if(strlen(buf)<1 || buf[0]=='[')
                    {
                        memset(buf, 0, bufsize);
                        break;
                    }
                    if(buf[0]=='\n' || buf[0]==';')
                    {
                        memset(buf, 0, bufsize);
                        continue;
                    }

                    endBuf(buf, bufsize);
                    strLine = buf;
                    memset(buf, 0, bufsize);
                    //找到字段
                    if(strLine.find(name)!= string::npos)
                    {
                        int pos = strLine.find_first_of('=');
                        value = strLine.substr(pos+1, strLine.length() - pos -1);
                        //去掉空格
                        value =cTrim(value);
                        if(value.length()<1)
                        {
                            return false;
                        }
                        return true;
                    }
                }

            }
        }
    }
    //没有找到字段值
    if(value.length()<1)
    {
        return false;
    }
    return true;
}

void endBuf(char *buf, int bufsize)
{
    if(buf[strlen(buf) -1] != '\n')
    {
        if((int)strlen(buf)<bufsize)
        {
            buf[strlen(buf)] = '\0';
        }
        return;
    }

    buf[strlen(buf) -1] = '\0';
}

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

bool isDigital(const string &str)
{
    //遍历每个字符，发现不是数字0~9('48'~'57')或'.'46，即返回false
    int dotc = 0;
    string::const_iterator it;
    for(it=str.begin(); it!=str.end(); it++)
    {
        if(*it==46)
        {
            dotc++;
        }
        if((*it!=46) && (*it<48 || *it>57))
        {
            return false;
        }
    }
    //小数点的个数>=1个时，判断为非有效数字
    if(dotc>1)
    {
        return false;
    }
    return true;
}
bool isIntStr(const string &str,bool allowNegative)
{
    QString src = str.c_str();
    bool ok;
    int val = src.toInt(&ok);
    if (!ok)
        return false;
    if (!allowNegative && val < 0)
        return false;
    return true;

    //int iCount = str.length();
    //for (int i = 0; i < iCount; i++)
    //{
    //    if (i == 0)
    //    {
    //        if (!allowNegative && str[0] == '-')
    //            return false;
    //    }
    //    if (str[i] < '0' || str[i] > '9')
    //        return false;
    //}
    //return true;
}
bool isValidIPv4Address(const string &str)
{
    int iCount = str.length();
    vector<string> address;
    string ip;
    int iPointCount = 0;
    for (int i = 0; i < iCount ; i++)
    {
        if (str[i] != '.')
        {
            ip += str[i];
        }
        else
        {
            address.push_back(ip);
            ip.clear();
            iPointCount++;
        }
    }
    if (iPointCount != 3)
    {
        return false;
    }
    if (ip.length() > 0)
    {
        address.push_back(ip);
    }
    if (address.size() != 4 || address[0] == "0")
    {
        return false;
    }
    for (int i = 0; i < 4; i++)
    {
        if (isIntStr(address[i]))
        {
            int iNum = atoi(address[i].c_str());
            if (iNum < 0 || iNum > 255)
            {
                return false;
            }
        }
        else
            return false;

    }
    return true;
}
bool olderThen(const FileInfo &f1, const FileInfo &f2)
{
    return f1.strMdyTime < f2.strMdyTime;
}

string getExeFilePath(const char* filePath)
{
    string strPath;
    QString strFileName = QCoreApplication::applicationDirPath()+filePath;
    strPath = strFileName.toStdString();
    return strPath;
}

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

/**
* @name			:	CreateDirByPath
* @brief		:	根据路径层级创建目录.
* @param [in]	:	strPath 目录层级路径.
* @return		:	bool 创建成功返回true, 创建失败返回false.
* @exception	:	无
*/
bool CreateDirByPath(char *strPath)
{
//    string buf;
//    while(*strPath != '\0')
//    {
//        buf += *strPath;
//        if(*strPath == '/')
//        {
//            //目录不存在
//            if(access(buf.c_str(), 6) == -1)
//            {
//                //创建目录
//                if(mkdir(buf.c_str()) == -1)
//                {
//                    return false;
//                }
//            }

//        }
//        strPath++;
//    }

    return true;
}

bool CreatePath(QString path)
{
    QDir dir(path);
    if (!dir.exists())
    {
        return dir.mkpath(path);
    }
    return true;
}

void getRadarFileType(const string &str, RADAR_FILE_TYPE &type)
{
    if (str == "SA")
    {
        type = SA;
    }
    else if (str == "SB")
    {
        type = SB;
    }
    else if (str == "SC")
    {
        type = SC;
    }
    else if (str == "CA")
    {
        type = CA;
    }
    else if (str == "CC")
    {
        type = CC;
    }
    else if (str == "CD")
    {
        type = CD;
    }
    else if (str == "VTB_NEW")
    {
        type = VTB_NEW;
    }
    else if (str == "VTB_OLD")
    {
        type = VTB_OLD;
    }
    else if (str == "JD717")
    {
        type = JD717;
    }
    else
    {
        type = X_TYPE;
    }
}

DLL_EXPORT_DECL void getFileStringType(string &str, const RADAR_FILE_TYPE &type)
{
    switch (type)
    {
    case SA:
        str = "SA";
        break;
    case SB:
        str = "SB";
        break;
    case SC:
        str = "SC";
        break;
    case CA:
        str = "CA";
        break;
    case CC:
        str = "CC";
        break;
    case CD:
        str = "CD";
        break;
    case VTB_NEW:
        str = "VTB_NEW";
        break;
    case VTB_OLD:
        str = "VTB_OLD";
        break;
    case JD717:
        str = "JD717";
        break;
    default:
        str = "X_TYPE";
        break;
    }
}

bool readProductColor(QList<PRODUCTRGBCOLOR> &productColorLst)
{
    CPathBuilder pb;
    //读取颜色表
    QString fileName = pb.getConfigPath() + "/productcolor.txt";
    return readProductColorFile(fileName, productColorLst);
}

bool readProductColorDefault(QList<PRODUCTRGBCOLOR> &productColorLst)
{
    CPathBuilder pb;
    //读取颜色表
    QString fileName = pb.getConfigPath() + "/productcolor_default.txt";
    return readProductColorFile(fileName, productColorLst);
}

bool readProductColorFile(const QString fileName, QList<PRODUCTRGBCOLOR> &productColorLst)
{
    if (!QFile::exists(fileName))
    {
        QSLOG_ERROR(QString::fromLocal8Bit("没有找到雷达色标配置文件，请及时处理！[%1]").arg(fileName));
        return false;
    }

    QSLOG_INFO(QString::fromLocal8Bit("开始读雷达色标配置文件[%1]!").arg(fileName));

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QSLOG_ERROR(QString::fromLocal8Bit("读雷达色标配置文件[%1]失败!").arg(fileName));
        return false;
    }

    productColorLst.clear();
    char buf[1024];
    while (file.readLine(buf, sizeof(buf)) != -1)
    {
        QString str(buf);
        str = str.trimmed();

        if (str.startsWith("["))
        {
            PRODUCTRGBCOLOR pcolor;
            pcolor.name = str.mid(1,str.length()-2);

            while((file.readLine(buf, sizeof(buf)) != -1))
            {
                str = buf;
                str = str.trimmed();
                int from = 0;
                int to = 0;
                QVector<QString> sl;
                while (-1 != (to = str.indexOf(",", from)))
                {
                    sl.push_back(str.mid(from, to - from));
                    from = to + 1;
                }
                sl.append(str.mid(from));
                if (sl.count() == 5)
                {
                    RGBCOLOR rgbColor;
                    rgbColor.r = sl[0].toInt();
                    rgbColor.g = sl[1].toInt();
                    rgbColor.b= sl[2].toInt();
                    rgbColor.value = sl[3].toFloat();
                    rgbColor.realValue=sl[4];
                    pcolor.rgbColorLst.append(rgbColor);
                }
                sl.clear();
                if(str.startsWith("["))
                {
                    productColorLst.append(pcolor);
                    pcolor.rgbColorLst.clear();
                    pcolor.name = str.mid(1,str.length()-2);
                    continue;
                }
                if(str == "" )
                {
                    break;
                }
            }
            productColorLst.append(pcolor);
        }
    }
    file.close();

    QSLOG_INFO(QString::fromLocal8Bit("读雷达色标配置文件[%1]结束!").arg(fileName));
    return true;
}

DLL_EXPORT_DECL bool writeProductColor(const QList<PRODUCTRGBCOLOR> &productColorLst)
{
    CPathBuilder pb;
    //读取颜色表
    QString fileName = pb.getConfigPath() + "/productcolor.txt";

    QSLOG_INFO(QString::fromLocal8Bit("开始重写雷达色标配置文件[%1]!").arg(fileName));

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QSLOG_ERROR(QString::fromLocal8Bit("打开雷达色标配置文件[%1]失败!").arg(fileName));

        return false;
    }

    QTextStream out(&file);
    foreach(const PRODUCTRGBCOLOR &color, productColorLst)
    {
        out<<"["<<color.name<<"]"<<"\n";
        QVector<RGBCOLOR>::const_iterator it = color.rgbColorLst.begin();

        for (; it!=color.rgbColorLst.end(); it++)
        {
            out<<it->r<<","<<it->g<<","<<it->b<<","<<it->value<<","<<it->realValue<<"\n";
        }
    }

    file.close();
    QSLOG_INFO(QString::fromLocal8Bit("重写雷达色标配置文件[%1]结束!").arg(fileName));

    return true;
}


bool getProductCode(PRODUCTCODE &productCode, const QString &groupName, const QString &productName)
{
    if (groupName.isEmpty() || productName.isEmpty())
    {
        return false;
    }

    CPathBuilder pb;
    //雷达产品名称色标对应配置文件
    QString fileName = pb.getConfigPath() + "/ProductCode.txt";
    if (!QFile::exists(fileName))
    {
        QSLOG_ERROR(QString::fromLocal8Bit("没有找到雷达产品名称色标对应配置文件，请及时处理！[%1]").arg(fileName));
        return false;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QSLOG_ERROR(QString::fromLocal8Bit("读雷达产品名称色标对应配置文件[%1]失败!").arg(fileName));
        return false;
    }

    char buf[1024] = {0};
    while (file.readLine(buf,sizeof(buf)) != -1)
    {
        QString str(buf);
        str = str.trimmed();
        if(str.startsWith(";"))
        {
            continue;
        }

        if (str.startsWith("["))
        {
            QString group = str.mid(1,str.length()-2);
            if (group == groupName)
            {
                while((file.readLine(buf, sizeof(buf)) != -1))
                {
                    str = buf;
                    str = str.trimmed();
                    int from = 0;
                    int to = 0;
                    QVector<QString> sl;
                    while (-1 != (to = str.indexOf(",", from)))
                    {
                        sl.push_back(str.mid(from, to - from));
                        from = to + 1;
                    }
                    sl.append(str.mid(from));


                    if (sl.count() == 4 && sl.at(1) == productName)
                    {
                        productCode.ProductID = sl.at(0).toInt();
                        productCode.ProductEnName = sl.at(1);
                        productCode.ColorName = sl.at(2);
                        productCode.ProductZhName = sl.at(3);
                        return true;
                    }
                    sl.clear();

                    if(str.startsWith("["))
                    {
                        break;
                    }
                }

            }
        }
    }

    file.close();
    //QSLOG_INFO(QString::fromLocal8Bit("读雷达产品名称色标对应配置文件[%1]结束，没有找到指定产品的对应信息!").arg(fileName));

    return false;
}

bool readProductCode(QString groupName, QList<PRODUCTCODE> &productCodeLst)
{
    if (groupName.isEmpty())
    {
        return false;
    }

    CPathBuilder pb;
    //雷达产品名称色标对应配置文件
    QString fileName = pb.getConfigPath() + "/ProductCode.txt";
    if (!QFile::exists(fileName))
    {
        QSLOG_ERROR(QString::fromLocal8Bit("没有找到雷达产品名称色标对应配置文件，请及时处理！[%1]").arg(fileName));
        return false;
    }

    //QSLOG_INFO(QString::fromLocal8Bit("开始读雷达产品名称色标对应配置文件[%1]!").arg(fileName));

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QSLOG_ERROR(QString::fromLocal8Bit("读雷达产品名称色标对应配置文件[%1]失败!").arg(fileName));
        return false;
    }

    char buf[1024] = {0};
    while (file.readLine(buf,sizeof(buf)) != -1)
    {
        QString str(buf);
        str = str.trimmed();
        if(str.startsWith(";"))
        {
            continue;
        }

        if (str.startsWith("["))
        {
            QString group = str.mid(1,str.length()-2);
            if (group == groupName)
            {
                productCodeLst.clear();
                while((file.readLine(buf, sizeof(buf)) != -1))
                {
                    str = buf;
                    str = str.trimmed();
                    int from = 0;
                    int to = 0;
                    QVector<QString> sl;
                    while (-1 != (to = str.indexOf(",", from)))
                    {
                        sl.push_back(str.mid(from, to - from));
                        from = to + 1;
                    }
                    sl.append(str.mid(from));

                    PRODUCTCODE pc;
                    if (sl.count() == 4)
                    {
                        pc.ProductID = sl.at(0).toInt();
                        pc.ProductEnName = sl.at(1);
                        pc.ColorName = sl.at(2);
                        pc.ProductZhName = sl.at(3);
                        productCodeLst.append(pc);
                    }
                    sl.clear();

                    if(str.startsWith("["))
                    {
                        break;
                    }
                }

            }

            if (productCodeLst.size() != 0)
            {
                break;
            }
        }
    }

    file.close();
    //QSLOG_INFO(QString::fromLocal8Bit("读雷达产品名称色标对应配置文件[%1]结束!").arg(fileName));

    return true;
}

double permissiveToDouble( QString string, bool &ok )
{
    //remove any thousands separators
    string.remove( QLocale::system().groupSeparator() );
    return QLocale::system().toDouble( string, &ok );
}

int permissiveToInt( QString string, bool &ok )
{
    //remove any thousands separators
    string.remove( QLocale::system().groupSeparator() );
    return QLocale::system().toInt( string, &ok );
}

void GetAllDirTime(string rootdir,map<string,QDateTime>& dirTotime)
{
    QDir oDir(rootdir.c_str());
    QFileInfo oFile(rootdir.c_str());
    dirTotime.insert(make_pair(oFile.absoluteFilePath().toStdString(),oFile.lastModified()));
    QFileInfoList oFiles = oDir.entryInfoList();
    foreach (const QFileInfo &file, oFiles)
    {
        if (file.fileName() == "." || file.fileName() == "..")
        {
            continue;
        }

        if (file.isDir())
        {
            dirTotime.insert(make_pair(file.absoluteFilePath().toStdString(),file.lastModified()));
            GetAllDirTime(file.absoluteFilePath().toStdString(),dirTotime);
            break;
        }
    }
    return;
}

int readMosaicFile(const QString &filename, MOSAICDATAHEADER &mosaicDataHeader, std::vector<RADARSITEINFO> &pSiteInfo,
                   std::vector<unsigned char> &pData, std::vector<short> &pDataStartPos)
{
    gzFile file = NULL;
    file = gzopen(filename.toLocal8Bit().data(), "rb");
    if( file == NULL ) return -1;

    int number = 0 ;
    int error = 0 ;
    number = gzread( file, &mosaicDataHeader, sizeof(MOSAICDATAHEADER) );
    gzerror(file, &error);
    if( error != Z_OK || number != sizeof(MOSAICDATAHEADER) )
    {
        gzclose(file);
        return -1;
    }
    int iNumSites = mosaicDataHeader.iNumRadars;
    int iXNum = mosaicDataHeader.iLonGrids;
    int iYNum = mosaicDataHeader.iLatGrids;

    // 读组网站点信息
    pSiteInfo.resize(iNumSites);


    number = gzread( file, pSiteInfo.data(), sizeof(RADARSITEINFO)*iNumSites );
    gzerror(file, &error);
    if( error != Z_OK || number != sizeof(RADARSITEINFO)*iNumSites )
    {
        gzclose(file);
        return -1;
    }

    // 读取各高度层的位置信息
    if (mosaicDataHeader.iLevels > 0)
    {
        pDataStartPos.resize(mosaicDataHeader.iLevels);

        number = gzread( file, pDataStartPos.data(), sizeof(short)*mosaicDataHeader.iLevels );
        gzerror(file, &error);
        if( error != Z_OK || number != sizeof(short)*mosaicDataHeader.iLevels )
        {
            gzclose(file);
            return -1;
        }
        pData.resize(iXNum * iYNum * mosaicDataHeader.iLevels);

        number = gzread( file, pData.data(), sizeof(unsigned char)*iXNum*iYNum*mosaicDataHeader.iLevels );
        gzerror( file, &error );
        if( error != Z_OK || number != sizeof(unsigned char)*iXNum*iYNum*mosaicDataHeader.iLevels )
        {
            gzclose(file);
            return -1;

        }
    }
    else
    {
        // 读没有高度层的组网格点数据
        pData.resize(iXNum * iYNum);


        number = gzread(file, pData.data(), sizeof(unsigned char)*iXNum*iYNum );
        gzerror( file, &error );
        if( error != Z_OK || number != sizeof(unsigned char)*iXNum*iYNum )
        {
            gzclose(file);
            return -1;
        }
    }
    gzclose(file);

    return 0;

#if 0
    FILE *fp = fopen(filename.toStdString().c_str(), "rb");

    if ( fp == NULL )
    {
        return -1;
    }
    QSharedPointer<FILE> autoClose(fp, fclose);

    // 读组网头文件信息
    if (fread(&mosaicDataHeader, sizeof(MOSAICDATAHEADER), 1, fp) != 1)
    {
        return -1;
    }

    int iNumSites = mosaicDataHeader.iNumRadars;
    int iXNum = mosaicDataHeader.iLonGrids;
    int iYNum = mosaicDataHeader.iLatGrids;

    // 读组网站点信息
    pSiteInfo.resize(iNumSites);
    if (fread(pSiteInfo.data(), sizeof(RADARSITEINFO)*iNumSites, 1, fp) != 1)
    {
        return -1;
    }

    // 读取各高度层的位置信息
    if (mosaicDataHeader.iLevels > 0)
    {
        pDataStartPos.resize(mosaicDataHeader.iLevels);
        if (fread(pDataStartPos.data(), sizeof(short)*mosaicDataHeader.iLevels, 1, fp) != 1)
        {
            return -1;
        }

        pData.resize(iXNum * iYNum * mosaicDataHeader.iLevels);
        if (fread(pData.data(), sizeof(unsigned char)*iXNum*iYNum*mosaicDataHeader.iLevels, 1, fp) < 1)
        {
            return -1;
        }
    }
    else
    {
        // 读没有高度层的组网格点数据
        pData.resize(iXNum * iYNum);
        if (fread(pData.data(), sizeof(unsigned char)*iXNum*iYNum, 1, fp) < 1)
        {
            return -1;
        }
    }
    //342968个字节
    //fseek(fp, 0, SEEK_CUR);
    //int len0 = ftell(fp);
    //fseek(fp, 0, SEEK_END);
    //int len1 = ftell(fp);
    //int len2 = sizeof(MOSAICDATAHEADER) +  sizeof(RADARSITEINFO)*iNumSites + sizeof(unsigned char)*iXNum*iYNum;
    //for(int j=0; j<iYNum; j++)
    //{
    //	for (int i=0; i<iXNum; i++)
    //	{
    //		QSLOG_DEBUG(QString("%1,%2:%3").arg(i).arg(j).arg(pData[j*iXNum + i]));
    //	}
    //}

    return 0;
#endif

}

bool connectToWinRemote(QString strURL,const QString& strUser,const QString& strPwd)
{
    //----解决本地文件夹，返回错误码67的问题----
    QDir qdir(strURL);
    if (qdir.exists())
    {
        return true;
    }
    //--------

#ifdef Q_OS_WIN
    strURL = strURL.replace("/", "\\");
    char cURL[1024] = {0};
    strncpy(cURL, strURL.toStdString().c_str(), sizeof(cURL) - 1);
    NETRESOURCEA rsc = {0};
    rsc.lpRemoteName = (LPSTR)cURL;
    rsc.dwType = RESOURCETYPE_DISK;

    DWORD ret = WNetAddConnection2A(&rsc, (LPCSTR)(strPwd.toStdString().c_str()), (LPCSTR)(strUser.toStdString().c_str()), 0);
    if (NO_ERROR == ret || ret == 1219)
    {
        return true;
    }
#else
#endif

    QSLOG_ERROR("connectToWinRemote failed.");
    return false;
}
QDateTime getRadarFileDateTime(const QString& r_fileName)
{
    QDateTime t_fileDateTime;

    MOSAICDATAHEADER t_header = {0};
    gzFile fp = gzopen(r_fileName.toStdString().c_str(), "rb");

    if (NULL == fp)
    {
        QSLOG_ERROR(QString::fromLocal8Bit("打开文件:%1失败").arg(r_fileName));
        return t_fileDateTime;
    }


    ///读文件头
    if (0 == gzread(fp, &t_header,sizeof(MOSAICDATAHEADER)))
    {
        QSLOG_ERROR(QString::fromLocal8Bit("读取文件:%1失败").arg(r_fileName));
        gzclose(fp);
        return t_fileDateTime;
    }
    int year   = t_header.iStYear;
    int month  = t_header.iStMonth;
    int day    = t_header.iStDay;
    int hour   = t_header.iStHour;
    int minute = t_header.iStMinute;
    int second = t_header.iStSec;
    QString dtString;
    dtString.sprintf("%04d%02d%02d%02d%02d%02d",year,month,day,hour,minute,second);
    t_fileDateTime = QDateTime::fromString(dtString,"yyyyMMddhhmmss");
    gzclose(fp);
    return t_fileDateTime;
}
int getProductLevels(const QString& r_fileName, vector<QString>& r_vLevels)
{
    QString msg;
    MOSAICDATAHEADER t_header = {0};
    gzFile fp = gzopen(r_fileName.toLocal8Bit().toStdString().c_str(), "rb");

    if (NULL == fp)
    {
        QSLOG_ERROR(QString::fromLocal8Bit("打开文件:%1失败").arg(r_fileName));
        return -1;
    }


    ///读文件头
    if (0 == gzread(fp, &t_header,sizeof(MOSAICDATAHEADER)))
    {
        QSLOG_ERROR(QString::fromLocal8Bit("读取文件:%1失败").arg(r_fileName));
        gzclose(fp);
        return -1;
    }

    int t_iLevels = t_header.iLevels;
    if (t_iLevels<0 || t_iLevels > 40 || t_header.iNumRadars==65535)
    {
        gzclose(fp);
        return -1;
    }

    if (0 == t_iLevels)
    {
        gzclose(fp);
        return 0;
    }
    size_t t_start = t_header.dwLevHgtOffset;
    //size_t t_end   = t_header.dwObvDataOffset;
    /////计算层数
    //int ilevels = (t_end - t_start)/sizeof(unsigned short);
    ///定位到高度层开始
    // fseek(fp,t_start,SEEK_CUR);
    gzseek(fp, t_start, SEEK_CUR);

    ///依次读取高度层信息
    for (int i = 0; i < t_iLevels ; i++)
    {
        unsigned short h;
        // fread(&h,sizeof(unsigned short),1,fp);
        if (0 == gzread(fp, &h, sizeof(unsigned short)))
        {
            gzclose(fp);
            return -1;
        }
        QString height;
        height.sprintf("%d",h);
        r_vLevels.push_back(height);
    }
    // fclose(fp);
    gzclose(fp);
    return t_iLevels;
}

DLL_EXPORT_DECL QString getUnitByColorName(const QString colorName)
{
    if (colorName == "DBZ")
    {
        return "dBZ";
    }
    else if (colorName == "ET")
    {
        return "km";
    }
    else if (colorName == "V")
    {
        return "m/s";
    }
    else if (colorName == "VHS" || colorName == "VVS")
    {
        return "E-4/S";
    }
    else if (colorName == "VIL")
    {
        return "kg/m*m";
    }
    else if (colorName == "VWP")
    {
        return "RMS";
    }
    return "";
}

double get_distance(double lat1,double lng1,double lat2,double lng2)
{
    double radLat1 = radian(lat1);
    double radLat2 = radian(lat2);
    double a = radLat1 - radLat2;
    double b = radian(lng1) - radian(lng2);

    double dst = 2*asin((sqrt(pow(sin(a/2),2)+cos(radLat1)*cos(radLat2)*pow(sin(b/2),2) )));

    dst = dst * EARTH_RADIUS;
    dst = cRound(dst*10000)/10000;

    return dst;
}

QRectF getMapExtents(double lon0, double lat0, int maxl)
{
    //用计算北京南京的实际距离905km验证通过
    double s = maxl / EARTH_RADIUS;
    double a = rad_to_deg(s);
    double b = 2 * qAsin(qSin(s/2.0)/qCos(deg_to_rad(lat0)));
    b = rad_to_deg(b);

    QRectF rect(0, 0, 0, 0);
    rect.adjust(qMin(lon0+b, lon0-b), qMin(lat0+a, lat0-a), qMax(lon0+b, lon0-b), qMax(lat0+a, lat0-a));
    return rect;
}

void BinaryFindCloseValue(float value, const QVector<float> &group, int &pos)
{
    int start = 0;
    int end = group.size() - 1;
    pos = 0;
    while (start+1 < end)
    {
        pos = (start + end) / 2;
        if (group[pos] > value)
        {
            end = pos;
        }
        else
        {
            start = pos;
        }
    }

    if (pos < group.size() - 2)
    {
        float a = abs(long(group[pos] - value));
        float b = abs(long(group[pos+1] - value));
        if (b < a)
        {
            pos = pos + 1;
        }
    }
}

bool readAllStationInfo(QVector<Station> &vecStation)
{
//     CPathBuilder pb;
//     QString fileName = pb.getConfigPath() + "/StationInfo.ini";
//     if (!QFile::exists(fileName))
//     {
//         QSLOG_ERROR(QString::fromLocal8Bit("没有找到文件[%1]！").arg(fileName));
//         return false;
//     }
//
//     QFile file(fileName);
//     if (!file.open(QFile::ReadOnly | QFile::Text))
//     {
//         QSLOG_ERROR(QString::fromLocal8Bit("打开文件[%1]失败!").arg(fileName));
//         return false;
//     }
//
//     vecStation.clear();
//     char buf[1024] = {0};
//     while (file.readLine(buf,sizeof(buf)) != -1)
//     {
//         QString str(buf);
//         str = str.trimmed();
//         if(str.startsWith(";"))
//         {
//             continue;
//         }
//
//         if (str.startsWith("["))
//         {
//             QString group = str.mid(1,str.length()-2);
//             Station stainfo;
//             sprintf(stainfo.id, "%s", qPrintable(group));
//
//             while((file.readLine(buf, sizeof(buf)) != -1))
//             {
//                 str = buf;
//                 str = str.trimmed();
// 				if (str.)
// 				{
// 				}
//                 int from = 0;
//                 int to = 0;
//                 QVector<QString> sl;
//                 while (-1 != (to = str.indexOf("=", from)))
//                 {
//                     sl.push_back(str.mid(from, to - from));
//                     from = to + 1;
//                 }
//                 sl.append(str.mid(from));
//
//                 PRODUCTCODE pc;
//                 if (sl.count() == 4)
//                 {
//                     pc.ProductID = sl.at(0).toInt();
//                     pc.ProductEnName = sl.at(1);
//                     pc.ColorName = sl.at(2);
//                     pc.ProductZhName = sl.at(3);
//                     productCodeLst.append(pc);
//                 }
//                 sl.clear();
//
//                 if(str.startsWith("["))
//                 {
//                     break;
//                 }
//             }
//
//             if (productCodeLst.size() != 0)
//             {
//                 break;
//             }
//         }
//     }
//
//     file.close();
//     //QSLOG_INFO(QString::fromLocal8Bit("读雷达产品名称色标对应配置文件[%1]结束!").arg(fileName));

    return true;
}

DLL_EXPORT_DECL QString getAreaCodeZhName(const QString &enName)
{
    if (enName == "")
    {
        return "未知";
    }

    CPathBuilder pb;
    //组网区域编码的中英文配置文件
    QString fileName = pb.getConfigPath() + "/AreaCodeDefine.ini";
    if (!QFile::exists(fileName))
    {
        QSLOG_ERROR(QString::fromLocal8Bit("组网区域编码的中英文配置文件[%1]不存在!").arg(fileName));
        return false;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QSLOG_ERROR(QString::fromLocal8Bit("读组网区域编码的中英文配置文件[%1]失败!").arg(fileName));
        return false;
    }

    char buf[1024] = {0};
    while (file.readLine(buf,sizeof(buf)) != -1)
    {
        QString str(buf);
        str = str.trimmed();
        int index = str.indexOf("=");
        if (index > -1)
        {
            QString en = str.left(index);
            if (en == enName)
            {
                QString zh = str.right(str.length() - index - 1);
                return zh;
            }
        }
    }

    return enName;
}
