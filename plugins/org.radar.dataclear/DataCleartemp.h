#ifndef DataCleartemp_h__
#define DataCleartemp_h__

#include <QObject>
#include <QString>
#include "BaseDatas.h"


class DateClearTemp:public QObject
{
    Q_OBJECT
public:
    DateClearTemp();
    DateClearTemp(QList<BaseDatas> datas);
    DateClearTemp(const QString &fullPath,const QString &style,const QString &freeSpace,const int &timeLine,const QStringList &regex);
    DateClearTemp(BaseDatas &data);
    ~DateClearTemp();
public:
    bool dataClear();
    //************************************
    // Method:    dataClear
    // FullName:  DateClearTemp::dataClear
    // Access:    public
    // Returns:   bool
    // Qualifier:
    // Parameter: const QString & style 删除类型
    // Parameter: const double & freeSpace 剩余空间
    // Parameter: const QString & timeLine 时间
    //************************************
    bool dataClear(const QString &style,const double &freeSpace,const int &timeLine,const QString &regex);
private:
    QString m_fullpath;
    QString m_style;
    int m_timeLine;
    QStringList m_regexs;
    QString m_freeSpace;
    QList<BaseDatas> m_datas;
    BaseDatas m_data;
};
#endif // DataCleartemp_h__
