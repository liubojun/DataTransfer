#include "DataCleartemp.h"
#include "DelFiles.h"
#include "ftpclear.h"
#include <QString>
#include <QSharedPointer>

DateClearTemp::DateClearTemp()
{

}
DateClearTemp::DateClearTemp( const QString &fullPath,const QString &style,const QString &freeSpace,const int &timeLine,const QStringList &regex )
    :m_fullpath(fullPath),m_freeSpace(freeSpace),m_style(style),m_regexs(regex),m_timeLine(timeLine)
{
    //dataClear(style,freeSpace,timeLine);
}

DateClearTemp::DateClearTemp(QList<BaseDatas> datas):m_datas(datas)
{
    //dataClear();
}

DateClearTemp::DateClearTemp(BaseDatas &data) : m_data(data)
{
    m_fullpath = data.m_fullPath;
    m_style = data.m_style;
    m_freeSpace = data.m_freeSize;
    m_timeLine = data.m_time;
    m_regexs = data.m_regex;
}

DateClearTemp::~DateClearTemp()
{

}

bool DateClearTemp::dataClear(const QString &style,const double &freeSpace,const int &timeLine,const QString &regex )
{
    return true;
    //QSharedPointer<DelFiles> delFile = QSharedPointer<DelFiles>(new DelFiles(m_fullpath,m_style,m_freeSpace,m_timeLine,m_regexs));
    //bool flag = false;
    //if(style=="0")	//按周期清除
    //{
    //    flag = delFile->delByTime();
    //}
    //else if(style=="1")	//按空间清除
    //{
    //    flag = delFile->delBySize();
    //}
    //else
    //{
    //    bool temp = delFile->delByTime();
    //    bool temp_ = delFile->delBySize();
    //    flag = temp&temp_;
    //}
    //return flag;
}

bool DateClearTemp::dataClear()
{

    if (m_data.taskType == 1)
    {
        CFtpClear ftpClear(m_data);
        ftpClear.run();
        return true;
    }
    else
    {
        // QSharedPointer<DelFiles> delFile = QSharedPointer<DelFiles>(new DelFiles(m_fullpath, m_style, m_freeSpace, m_timeLine, m_regexs));
        //return delFile->delFiles();
        DelFiles dirClear(m_data);
        dirClear.delFiles();
        return true;
    }

}
