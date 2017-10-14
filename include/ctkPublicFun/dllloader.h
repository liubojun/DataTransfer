#ifndef DLLLOADER_H_
#define DLLLOADER_H_

#include "macro.h"		//导入导出声明
#include "commontypedef.h"
#include <QMap>
#include <QLibrary>
#include <QSharedPointer>
#include <QMutex>


class DLL_EXPORT_CLASS_DECL CDllLoader
{
public:
    static CDllLoader &getInstance();
    virtual ~CDllLoader();

    /**
     * @brief  根据产品名，获取对应的动态库的名称
     * @param  const QString & in_strProduct：产品名（如DBZ, VIL等）
     * @return const PRODUCTMODULE *：产品信息
     */
    const PRODUCTMODULE *getModule(const QString &in_strProduct);
public:
    // 动态库对外提供的接口，函数指针，<动态库dll名称，动态库dll指针>
    QMap<QString, ModFun> MakeProMap;
    QVector<PRODUCTMODULE> m_dllModuleLst;//配置文件里配置的所有需要加载的动态库
private:
    /**
     * @brief  读取产品及对应的算法配置文件
     * @return int：读取成功返回0
     */
    int readModuleList();

    //一次性加载所有产品算法库
    void loadAllProductLib();

    QMap<QString, QSharedPointer<QLibrary> > m_loadeddllLst;	 //已经加载过的dll集合, string:dll名称,void*:dll句柄
    QString m_libPath; //产品算法库路径

private:
    CDllLoader();
    static CDllLoader *m_dllLoader;
    //私有内嵌类，唯一工作是在析构函数中释放CDllLoader的单例
    class CGarbo
    {
    public:
        ~CGarbo()
        {
            if (CDllLoader::m_dllLoader != NULL)
            {
                delete CDllLoader::m_dllLoader;
            }
        }
    };
    static CGarbo Garbo;	//在程序结束时，系统自动调用它的析构

    static QMutex m_oMutex;
};

#endif