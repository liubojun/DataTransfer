#ifndef DelFiles_h__
#define DelFiles_h__


#include <QObject>
#include <QtPlugin>
#include <QString>
#include <QStringList>
#include <BaseDatas.h>
#include <QRegExp>

#include "udplogsender.h"

class DelFiles:public QObject
{
    Q_OBJECT
public:
    DelFiles(BaseDatas &in_oData);
    //DelFiles(const QString &fullPath,const QString &style,const QString &freeSpace,const int &timeLine,const QString &regex);
    //DelFiles(const QString &fullPath,const QString &style,const QString &freeSpace,const int &timeLine,const QStringList &regexs);
    ~DelFiles();
public:
    bool delFiles();
    bool delByTime();
    bool delBySize();
    //************************************
    // Method:    delByTime
    // FullName:  ForEachFiles::delByTime
    // Access:    public
    // Returns:
    // Qualifier:
    // Parameter: QString & fullpath 全路径
    //************************************
    bool delByTime(const QString &fullpath);
    //************************************
    // Method:    delBySize
    // FullName:  ForEachFiles::delBySize
    // Access:    public
    // Returns:
    // Qualifier:
    // Parameter: QString & fullpath 全路径
    //************************************
    bool delBySize(const QString &fullpath);
protected:
    /**
     * @brief  获取指定目录下的所有数据文件
     * @param  const QString & plugindir：指定的目录
     * @param  QStringList & plugins：所有数据文件的全路径
     * @return QT_NAMESPACE::QStringList：所有数据文件的全路径（同plugins）
     */
    QStringList loaPlugins(const QString &plugindir, QStringList &plugins);
    double getDiskFreeSpace(const QString& sDirPath);
private:

    /**
     * @brief  简述函数功能
     * @param  const QString & sDirPath
     * @param  double & fFree
     * @return bool：描述返回值
     */
    bool getFreeTotalSpace(const QString& sDirPath, double& fFree);
private:
    QStringList m_fileList;
    //QString m_style;         //清楚方案
    QString m_strFullPath; //文件全路径
    //double m_freeSpace;		//剩余空间（单位：M）
    //int m_time;			//时间期限
    //QString m_match ;		//文件正则表达式
    //QList<BaseDatas> m_datas;
    QRegExp reg;
    //QStringList m_matches;
    BaseDatas &m_oData;

    CUdpLogSender m_oLogSender;
};

#endif // DelFiles_h__
