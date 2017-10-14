#include "dllloader.h"
#include "ctkPublicFun.h"
#include "commontypedef.h"
#include "pathbuilder.h"
#include <string.h>
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
//#include "Qlog.h"
#include "ctkLog.h"
#include "pathbuilder.h"
using namespace std;

// 多线程安全的单例模式
CDllLoader *CDllLoader::m_dllLoader=NULL;
QMutex CDllLoader::m_oMutex;

CDllLoader &CDllLoader::getInstance()
{
    if(m_dllLoader == NULL)
    {
        QMutexLocker autolocker(&m_oMutex);
        if (m_dllLoader == NULL)
        {
            m_dllLoader = new CDllLoader();
        }
    }
    return *m_dllLoader;
}

CDllLoader::CDllLoader()
{
    //QSLOG_INFO("Begin to load algorithm libs...");
    if( readModuleList()!=0 )
    {
        QSLOG_ERROR("Fail to load algorithm libs!");
        exit(-1);
    }
    loadAllProductLib();
    //QSLOG_INFO("Success to load algorithm libs!");
}


CDllLoader::~CDllLoader()
{
    //卸载所有的产品算法库
    QMap<QString, QSharedPointer<QLibrary> >::Iterator it;
    for(it = m_loadeddllLst.begin(); it != m_loadeddllLst.end(); it++)
    {
        if (it.value()->isLoaded())
        {
            it.value()->unload();
        }
    }
    m_loadeddllLst.clear();
    MakeProMap.clear();
    m_dllModuleLst.clear();
    m_dllLoader = NULL;
}

int CDllLoader::readModuleList()
{
    //获取插件算法所在的路径
    CPathBuilder pd;
    string filename = pd.getConfigPath().toStdString() + "/ModuleList.ini";
    QSLOG_DEBUG(QString("begin to read param file [%1]").arg(filename.c_str()));

    //m_libPath = QCoreApplication::applicationDirPath();		//待改??
    m_libPath = pd.getAlgorithmPath();

    FILE *fp = NULL;
    if ((fp=fopen(filename.c_str(), "r")) == NULL)
    {
        QSLOG_DEBUG(QString("读配置文件[%1]失败!:%2").arg(filename.c_str()).arg(strerror(errno)));
        return -1;
    }

    const int bufsize = 256;
    char buf[bufsize];
    memset(buf, 0, bufsize);
    //从[ModList]下面一行开始读
    while (fgets(buf, bufsize, fp) != NULL)
    {
        if(strlen(buf)<1 || buf[0]== '\n' || buf[0]==';' || buf[0]=='[' || buf[0]=='#')
        {
            memset(buf, 0, bufsize);
            continue;
        }
        buf[strlen(buf) - 1] = '\0';

        QString strLine(buf);
        memset(buf, 0, bufsize);
        QStringList slList = strLine.split(',');
        if (slList.size() != 4)
        {
            continue;
        }

        PRODUCTMODULE pm;
        pm.ProductID = slList.at(0).trimmed().toInt();
        pm.ProductName = slList.at(1).trimmed();
        pm.ModuleName = slList.at(2).trimmed();
        pm.type = slList.at(3).trimmed().toInt();
        m_dllModuleLst.push_back(pm);
    }
    fclose(fp);
    //QSLOG_DEBUG(QString("读配置文件[%1]结束!").arg(filename.c_str()));
    return 0;
}

void CDllLoader::loadAllProductLib()
{
    QSLOG_DEBUG("Begin to loadAllProductLib...");
    //逐个加载
    QVector<PRODUCTMODULE>::Iterator it;
    for(it=m_dllModuleLst.begin(); it!=m_dllModuleLst.end(); it++)
    {
        const PRODUCTMODULE &opm = *it;
        //已经加载过的不再重复加载
        if(m_loadeddllLst.find(opm.ModuleName) != m_loadeddllLst.end())
        {
            continue;
        }
        //加载的lib全路径
        QString slib = m_libPath + "/" + opm.ModuleName +".dll";
        QSLOG_DEBUG(QString("Now load [%1]!").arg(slib));
        QSharedPointer<QLibrary> lib = QSharedPointer<QLibrary>(new QLibrary(slib));
        if (!lib->load())
        {
            QSLOG_ERROR(QString("Fail to load [%1] !").arg(slib));
            continue;
        }
        DLLFUN_MakeProduction makePro = (DLLFUN_MakeProduction) lib->resolve("MakeProduction");
        if(makePro == NULL)
        {
            QSLOG_DEBUG(QString("Fail to call MakeProduction of [%1]!").arg(slib));
            exit(-1);
        }
        DLLFUN_Init init = (DLLFUN_Init) lib->resolve("Init");
        if(init == NULL)
        {
            QSLOG_DEBUG(QString("Fail to call Init of [%1]!").arg(slib));
            exit(-1);
        }
        DLLFUN_UnInit unInit = (DLLFUN_UnInit) lib->resolve("UnInit");
        if(unInit == NULL)
        {
            QSLOG_DEBUG(QString("Fail to call UnInit of [%1]!").arg(slib));
            exit(-1);
        }
        ModFun moFun;
        moFun.init = init;
        moFun.makeProduct = makePro;
        moFun.unInit = unInit;
        //动态库名与加载成功句柄的关系
        m_loadeddllLst[opm.ModuleName] = lib;
        //动态库名称与动态库接口间的关系
        MakeProMap[opm.ModuleName] = moFun;
    }
    QSLOG_DEBUG("Finish to loadAllProductLib!");
}

const PRODUCTMODULE *CDllLoader::getModule(const QString &in_strProduct)
{
    const PRODUCTMODULE *pRetModule = NULL;
    QVector<PRODUCTMODULE>::Iterator it;
    for(it=m_dllModuleLst.begin(); it!=m_dllModuleLst.end(); it++)
    {
        if((*it).ProductName.compare(in_strProduct) == 0)
        {
            pRetModule = &(*it);
            break;
        }
    }
    return pRetModule;
}
