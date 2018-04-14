#include "curl/curl.h"
#include <QApplication>
#include <QDebug>
#include <QTextCodec>
#include <QFileInfo>
#include <QDateTime>
#include <QPluginLoader>
#include <QSharedMemory>
#include <QTranslator>
#include <QProcess>
#include <QThreadPool>
#include <QMessageBox>
#include <QSplashScreen>
#include <QDir>
#include "ctkLog.h"
#include "ICtkPluginManager.h"
#include "pathbuilder.h"
#include "mainwindow.h"
#include "change_name.h"
//#include "logprotocol.h"
#include "logprint.h"

#include <iostream>


//Qt5通过以下来支持中文显示
#ifdef WITH_QT5
#ifdef _WIN32
#pragma  execution_character_set("utf-8")
#endif
#endif

//控制程序直接异常退出，而不是产生错误弹窗

#define  CRASH_SILENTLY 1
#if defined(_MSC_VER) && CRASH_SILENTLY
#include <excpt.h>
#include <windows.h>
#include <stdio.h>
#define Q_TRY_BEGIN     __try {
#define Q_TRY_END       }
//EXCEPTION_EXECUTE_HANDLER
#define Q_EXCEPT        __except(EXCEPTION_EXECUTE_HANDLER) { \
	printf("fault err!"); \
	return 1; \
}
#else
#define Q_TRY_BEGIN
#define Q_TRY_END
#define Q_EXCEPT
#endif


//----异常相关----
#ifdef _WIN32
#include <tchar.h>
#include <Windows.h>
#include <ImageHlp.h>
#include <stdlib.h>
#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "WS2_32")
#endif
//----

//#pragma comment(linker,"/subsystem:windows /entry:mainCRTStartup")

#ifdef WITH_MEM_CHECK
#include "vld.h"
#endif
#ifdef _WIN32
//----捕获异常----
inline BOOL IsDataSectionNeeded(const WCHAR* pModuleName)
{
    if (pModuleName == 0)

    {

        return FALSE;

    }

    WCHAR szFileName[_MAX_FNAME] = L"";

    _wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);

    if (wcsicmp(szFileName, L"ntdll") == 0)
        return TRUE;

    return FALSE;
}

inline BOOL CALLBACK MiniDumpCallback(PVOID         pParam,
                                      const PMINIDUMP_CALLBACK_INPUT   pInput,
                                      PMINIDUMP_CALLBACK_OUTPUT        pOutput)

{
    if (pInput == 0 || pOutput == 0)
        return FALSE;

    switch (pInput->CallbackType)
    {
    case ModuleCallback:
    {
        if (pOutput->ModuleWriteFlags & ModuleWriteDataSeg)
            if (!IsDataSectionNeeded(pInput->Module.FullPath))
                pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);
    }
    break;
    case IncludeModuleCallback:
    case IncludeThreadCallback:
    case ThreadCallback:
    case ThreadExCallback:
        return TRUE;

    default:
        break;

    }
    return FALSE;
}

//创建Dump文件
inline void CreateMiniDump(EXCEPTION_POINTERS* pep, LPCTSTR strFileName)
{
    HANDLE hFile = CreateFile(strFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
    {
        QSLOG_DEBUG("CreateMiniDump");
        MINIDUMP_EXCEPTION_INFORMATION mdei;

        mdei.ThreadId = GetCurrentThreadId();

        mdei.ExceptionPointers = pep;

        mdei.ClientPointers = FALSE;

        MINIDUMP_CALLBACK_INFORMATION mci;

        mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback;

        mci.CallbackParam = 0;

        MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)0x0000ffff;

        bool bFlag = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &mdei, NULL, NULL/*&mci*/);
        // 		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal,
        // 			pep ? pep : NULL,
        // 			NULL,
        // 			NULL);

        CloseHandle(hFile);

        if (bFlag)
        {
            QSLOG_DEBUG("Finish to write dump.");
        }
    }
}

// 程式异常捕获
long __stdcall ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    TCHAR szMbsFile[MAX_PATH] = { 0 };
    ::GetModuleFileName(NULL, szMbsFile, MAX_PATH);
    TCHAR* pFind = _tcsrchr(szMbsFile, '\\');

    if (pFind)
    {
        *(pFind + 1) = 0;

        _tcscat(szMbsFile, _T("CreateMiniDump.dmp"));

        CreateMiniDump(pException, szMbsFile);

    }

    // TODO: MiniDumpWriteDump
    // FatalAppExit(-1, _T("Fatal Error"));
    return EXCEPTION_CONTINUE_SEARCH;

    return EXCEPTION_EXECUTE_HANDLER;
}

long   __stdcall   callback(_EXCEPTION_POINTERS*   excp)
{
    //MessageBox(0, "Error", "error", MB_OK);
    //printf("Error   address   %x/n", excp->ExceptionRecord->ExceptionAddress);
    //printf("CPU   register:/n");
    //printf("eax   %x   ebx   %x   ecx   %x   edx   %x/n", excp->ContextRecord->Eax,
    //       excp->ContextRecord->Ebx, excp->ContextRecord->Ecx,
    //       excp->ContextRecord->Edx);
    return   EXCEPTION_EXECUTE_HANDLER;
}

LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
    return NULL;
}

BOOL PreventSetUnhandledExceptionFilter()
{
    HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));

    if (hKernel32 == NULL)
        return FALSE;

    void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");

    if (pOrgEntry == NULL)
        return FALSE;

    unsigned char newJump[100];

    DWORD dwOrgEntryAddr = (DWORD)pOrgEntry;

    dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far

    void *pNewFunc = &MyDummySetUnhandledExceptionFilter;

    DWORD dwNewEntryAddr = (DWORD)pNewFunc;
    DWORD dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;
    newJump[0] = 0xE9;  // JMP absolute
    memcpy(&newJump[1], &dwRelativeAddr, sizeof(pNewFunc));
    SIZE_T bytesWritten;
    BOOL bRet = WriteProcessMemory(GetCurrentProcess(), pOrgEntry, newJump, sizeof(pNewFunc)+1, &bytesWritten);
    return bRet;
}

void RunCrashHandler()
{
    SetUnhandledExceptionFilter(ApplicationCrashHandler);
    PreventSetUnhandledExceptionFilter();
}

#endif
//----
#include <QDebug>
#include <QProcess>
#include <QStringList>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv)
{
//#ifdef _WIN32
//    RunCrashHandler();
//#endif
    /*Q_TRY_BEGIN*/


    QApplication app(argc, argv);

    // 查询基本信息
    int threadnum = 0;
    int logPort = 0;
    bool enableLog = false;
    DataBase::getInstance()->queryBaseInfo(threadnum, logPort, enableLog);

    // 设置是否启用日志文件输出
    QLogger::getInstance()->enableLog(enableLog);

    CChangeName::init_rename_rules((app.applicationDirPath() + "/config/rename.rule").toStdString().c_str());
    // CChangeName::change_name("H08_B03_R030_20171112_1700.AWX", "KuiHua");

    CPathBuilder::setChildProcessEnv();

    // 设置工作的线程池
    QThreadPool::globalInstance()->setMaxThreadCount(threadnum);

    QSLOG_INFO(QString("START DATA TRANSFER PROGRAM, max thread count %1").arg(QThreadPool::globalInstance()->maxThreadCount()));
    //std::cout << "START DATA TRANSFER PROGRAM" << std::endl;
    // 控制程序运行一个唯一实例
//    QSharedMemory mem("DataTransfer");
//    if (!mem.create(1))
//    {
//        QMessageBox box(QMessageBox::Information, QStringLiteral("提示"), "");
//        box.setStandardButtons(QMessageBox::Ok);
//        box.setButtonText(QMessageBox::Ok, QStringLiteral("确认"));
//        box.setText(QStringLiteral("数据传输系统已运行！"));
//        box.exec();
//        return 0;
//    }
//////////////////////////////////////////////////////////////////////////

//用于支持中文路径
    QTextCodec *codec = QTextCodec::codecForLocale();
    QTextCodec::setCodecForLocale(codec);

//Qt4通过以下两句来支持中文显示
#ifndef WITH_QT5
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif

//////////////////////////////////////////////////////////////////////////
// 测试
//QString strPath = QStringLiteral("2");
//////////////////////////////////////////////////////////////////////////

// libcurl初始化,一个app只能init一次
    curl_global_init(CURL_GLOBAL_ALL);

    QSplashScreen startScreen(QPixmap(":/startPic.png"));
    startScreen.show();
    startScreen.setFont(QFont("微软雅黑", 13));

    CPathBuilder oPathBuilder;

// 加载翻译文件
    QTranslator translator;
    QSLOG_INFO(qApp->applicationName());
    if (!translator.load(oPathBuilder.getTranslatorFilePath(ZH)))
    {
        QSLOG_ERROR(QObject::tr("load translate file failure."));
    }
    else
    {
        qApp->installTranslator(&translator);
        QSLOG_INFO(QObject::tr("load translate file success."));
    }

// qt内部翻译
    QTranslator tl;
    if (tl.load(":/qt_zh_CN.qm"))
    {
        qApp->installTranslator(&tl);
    }

#ifndef _WIN32
    QPluginLoader loader(oPathBuilder.getPluginManagerPath() + "/libctkPluginManager.so");
#else
    QPluginLoader loader(oPathBuilder.getPluginManagerPath() + "/ctkPluginManager.dll");
    // QPluginLoader loader("F:\\新建文件夹\\bin\\plugins\\ctkPluginManager\\ctkPluginManager.dll");
#endif

    loader.setLoadHints(QLibrary::ExportExternalSymbolsHint);
    if (loader.load()) //其实此处load()方法的调用，在instance()调用的地方也会调用load().只是发现有调用过一次将不在继续调用load了. 目前这种写法会比较清晰.
    {
        QSLOG_INFO(QObject::tr("load success"));
    }
    else
    {
        QSLOG_ERROR(QObject::tr("load failure, error string:")  + loader.errorString() );
        return -1;
    }
    startScreen.showMessage(QStringLiteral("正在加载插件..."), Qt::AlignCenter, Qt::white);

    QObject *pLoaderInstance = loader.instance();
    if (NULL == pLoaderInstance)
    {
        QSLOG_ERROR("plugin instance is null");
        return -1;
    }
    ICtkPluginManager *plugin = dynamic_cast<ICtkPluginManager *>(loader.instance());
    QSLOG_DEBUG("dynamic_cast");
    if (NULL != plugin)
    {
        plugin->init();
    }
    else
    {
        return -1;
    }
    QSLOG_INFO(QObject::tr("load all plugins successful"));

//plugin->startRelatedServices();

    //QSharedMemory shared;
    //shared.setKey(QString("DataTransfer"));
    //if(!shared.attach())
    //{
    //    return -1;
    //}
    //if(!shared.create(1))
    //{
    //    QSLOG_ERROR(shared.errorString());
    //    return -1;
    //}

    startScreen.showMessage(QStringLiteral("所有加载插件成功..."), Qt::AlignCenter, Qt::white);

    startScreen.showMessage(QStringLiteral("开始清理本地缓存..."), Qt::AlignCenter, Qt::white);
    QDir dir(qApp->applicationDirPath() + "/work/download");
    dir.removeRecursively();
    startScreen.showMessage(QStringLiteral("清理本地缓存完毕..."), Qt::AlignCenter, Qt::white);

    MainWindow *wnd = new MainWindow(plugin);
    LogPrintImpl oLogPrint(wnd);
    //oLogPrint.run();
    QCoreApplication::processEvents();

    wnd->initialize();

    startScreen.showMessage(QStringLiteral("正在初始化界面..."), Qt::AlignCenter, Qt::white);
    wnd->show();
    startScreen.showMessage(QStringLiteral("界面初始化完成！"), Qt::AlignCenter, Qt::white);
    startScreen.finish(0);

//StartEvtReporting();
    //int *a = 0;
    //*a = 0;
    int ret=app.exec();
    //wnd->close();
    // 等待所有子线程结束

    oLogPrint.stop();
    delete wnd;
    QThread::sleep(2);
    //卸载所有插件
    plugin->uninit();

    loader.unload();	//只在最后一个应用实例unload有效.

    curl_global_cleanup();	// 清理libcurl资源

    QSLOG_RELEASE

    if( 825 == ret ) QProcess::startDetached(qApp->applicationFilePath());
    /*Q_TRY_END*/
    /*Q_EXCEPT*/
    // return ret;

}

