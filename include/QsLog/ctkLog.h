#ifndef ctkLog_h__
#define ctkLog_h__

#include <iostream>
#include <sstream>
#include <QString>
#include <qglobal.h>
#include <QMutex>
#include <QFile>
#include "QsLog.h"
#include "QsLogDest.h"
#include "../macro.h"

#ifndef WITH_QT5
#define QStringLiteral QString
#endif

// 输出日志函数指针
typedef void (*OutPutFunPtr)(const QString &);

class DLL_EXPORT_CLASS_DECL QLogger
{
private:
    QLogger(qint64 size = 10*1024*1024 , qint64 num = 10 );
//	MeteoGisLogger();

public:
    // 标识是否可以往函数中输出
    static QLogger *getInstance(bool useFunPrint = false);


    static void addOutFunPtr(OutPutFunPtr ptr);

    ~QLogger();

    //************************************
    // Method:    printDebug
    // FullName:  MeteoGisLogger::printDebug
    // Access:    public static
    // Returns:   void
    // Qualifier: 在控制台打印调试信息，不需要写入日志文件的信息
    // Parameter: const QString & msg
    // Parameter: const char * file
    // Parameter: const char * function
    // Parameter: int line
    //************************************
    void printDebug(const QString& msg, const char* file = NULL, const char* function = NULL, int line = -1 );

    //************************************
    // Method:    printInfo
    // FullName:  MeteoGisLogger::printInfo
    // Access:    public static
    // Returns:   void
    // Qualifier: 在控制台打印运行信息，需要写入日志文件的信息
    // Parameter: const QString & msg
    // Parameter: const char * file
    // Parameter: const char * function
    // Parameter: int line
    //************************************
    void printInfo(const QString& msg, const char* file = NULL, const char* function = NULL, int line = -1 );

    //************************************
    // Method:    printError
    // FullName:  MeteoGisLogger::printError
    // Access:    public static
    // Returns:   void
    // Qualifier: 在控制台打印错误信息，需要写入日志文件
    // Parameter: const QString & msg
    // Parameter: const char * file
    // Parameter: const char * function
    // Parameter: int line
    //************************************
    void printError(const QString& msg, const char* file = NULL, const char* function = NULL, int line = -1 );

    static void init(qint64 size = 10*1024*1024, qint64 num  = 10 );

private:

    //************************************
    // Method:    print
    // FullName:  MeteoGisLogger::print
    // Access:    private static
    // Returns:   void
    // Qualifier: 打印Debug、Info、Error级别的信息
    // Parameter: const QString & level
    // Parameter: const QString & msg
    // Parameter: const char * file
    // Parameter: const char * function
    // Parameter: int line
    //************************************
    void print(const QString &level, const QString& msg, const char* file = NULL, const char* function = NULL, int line = -1 );

	void backOldLogFile(const QString &in_strFilePath);

	// 获取日志文件大小
	inline qint64 getLogFileSize(const QString & in_strFile)
	{
		return QFile(in_strFile).size();
	}

private:
    // logging mechanism to file
    //QsLogging::Logger& logger;

	QMutex m_oLocker;

	QString m_strAppName;

	QString m_strLogPath;

	int m_iLogSize;

	int m_iLogNum;

    static QLogger *s_instance;
    static QList<OutPutFunPtr> m_printer;

};

// 设置函数输出指针
#define QSLOG_SET_PRINTFUNCTION(Ptr)  QLogger::setOutFunPtr(ptr)

#define QSLOG_DEBUG(str) QLogger::getInstance()->printDebug(QString(str), __FILE__, __FUNCTION__, __LINE__)
#define QSLOG_INFO(str)  QLogger::getInstance()->printInfo(QString(str), __FILE__, __FUNCTION__, __LINE__)
#define QSLOG_ERROR(str) QLogger::getInstance()->printError(QString(str), __FILE__, __FUNCTION__, __LINE__)

#define QSLOG_RELEASE \
	if (NULL != QLogger::getInstance()) \
		delete  QLogger::getInstance();


#endif // ctkLog_h__
