#ifndef DataClear_h__
#define DataClear_h__

#include "IDataClear.h"
#include "IDispatchTimer.h"
#include "ctkProtocol/dataclearSet.h"
#include <QString>
#include <QMap>

class CDispatch;

class DataClear : public IDataClear
{
    Q_OBJECT
    Q_INTERFACES(IDataClear)
    Q_INTERFACES(IBaseInterface)

public:
    DataClear();
    virtual ~DataClear();

public:
    virtual bool start(const BaseDatas &data);

    virtual bool start(const QList<BaseDatas> &m_datas, QSharedPointer<QThread> pThread);

    virtual void stop(const BaseDatas &m_datas);

    virtual void stop(const QList<BaseDatas> &m_datas);

    virtual const QList<BaseDatas> &get();

    //virtual bool ReadClearSet(DataClearSetList &allsets, QString &strPath);
    //virtual void AddClearSet(const DataClearSet &set);
    //virtual void DelClearSet(const string &strPath);
    //virtual void MdyClearSet(const DataClearSet &set);

public slots:
    virtual void doNow(const QString &dir);

private:
    QString m_info;                     //调度规则
    QString m_fullpath;
    QString m_style;
    QString m_timeLine;
    double m_freeSpace;
    QSharedPointer<QObject> m_pTimerObject;
    QList<BaseDatas> m_oBaseDatas;
    QList<QSharedPointer<CDispatch> > m_oProcessors;

    // 一个目录对应一个Dispathcer和一个Timer
    QMap<QString, QSharedPointer<CDispatch> > m_mapDispatchers;
    QMap<QString, QSharedPointer<QObject> > m_mapTimers;

    QString m_strClearCfgPath;
    DataClearSetList m_lstClearSet;

    QSharedPointer<QThread> m_pThread;

    QMap<QString, QSharedPointer<QThread> > m_pWorkThread;
};

#endif // DataClear_h__