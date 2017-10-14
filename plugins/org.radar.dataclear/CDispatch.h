#ifndef CDispatch_h__
#define CDispatch_h__

#include "DispatchInterface.h"
#include <QString>
#include "BaseDatas.h"

/**
  * @class <CDispatch>
  * @brief  时间调度清除数据.
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	DispatchInterface
 */
class CDispatch : public DispatchInterface
{
    Q_OBJECT
public:
    CDispatch(QList<BaseDatas> datas);
    CDispatch(const QString &fullPath, const QString &style, const QString &freeSpace, const int &timeLine, const QStringList &regex);
    ~CDispatch();

    void setParam(const BaseDatas &data);
    void doNow();


signals:
    void begin();

    void clearBegin(const QString &dir);
    void clearEnd(const QString &dir);
public slots:
    /**
     * @brief  定时清理数据
     * @param  const QSharedPointer<TimerCallBackParam> & data
     * @return void
     */
    virtual void Dispatch(const QSharedPointer<TimerCallBackParam> &data);

    void beginProcess();

    void dowork();
private:
    //QThread *m_pThreadSelf;
    QString m_fullpath;
    QString m_style;
    int m_timeLine;
    QStringList m_regex;
    QString m_freeSpace;
    QList<BaseDatas> m_datas;	///< 存储每个监控目录的设置

    BaseDatas m_oBaseData;
    bool m_bHasFinished;

};
#endif // CDispatch_h__
