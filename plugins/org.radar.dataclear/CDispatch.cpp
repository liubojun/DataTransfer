#include "CDispatch.h"
#include <QString>
#include "DataCleartemp.h"
#include <QSharedPointer>
#include <QDebug>
#include <QTimer>
#include "QsLog/ctkLog.h"

CDispatch::CDispatch( const QString &fullPath,const QString &style,const QString &freeSpace,const int &timeLine,const QStringList &regex)
    :m_fullpath(fullPath),m_freeSpace(freeSpace),m_style(style),m_regex(regex),m_timeLine(timeLine)
{
    //m_pThreadSelf = new QThread(this);
    connect(this, SIGNAL(begin()), this, SLOT(beginProcess()));

    m_bHasFinished = true;
}

CDispatch::CDispatch(QList<BaseDatas> datas):m_datas(datas)
{
    m_bHasFinished = true;
}

CDispatch::~CDispatch()
{
    //qDebug() << "delete " << m_fullpath;
}

// 定时调度清理文件目录
void CDispatch::Dispatch(const QSharedPointer<TimerCallBackParam> &data)
{
    if (m_bHasFinished)
    {
        QTimer::singleShot(0, this, SLOT(dowork()));
    }
    else
    {
        QSLOG_DEBUG(QStringLiteral("之前任务还未结束，本次任务不做处理."));
    }

}

void CDispatch::doNow()
{
    emit begin();
}

void CDispatch::beginProcess()
{
    if (m_bHasFinished)
    {
        m_bHasFinished = false;
        emit clearBegin(m_oBaseData.m_fullPath);
        QSharedPointer<DateClearTemp> dataClear = QSharedPointer<DateClearTemp>(new DateClearTemp(m_oBaseData));
        dataClear->dataClear();
        emit clearEnd(m_oBaseData.m_fullPath);
        m_bHasFinished = true;
    }
    else
    {
        QSLOG_DEBUG(QStringLiteral("之前任务还未结束，本次任务不做处理."));
    }
}

void CDispatch::setParam(const BaseDatas &data)
{
    m_oBaseData = data;
}


void CDispatch::dowork()
{
    m_bHasFinished = false;
    emit clearBegin(m_oBaseData.m_fullPath);
    // BaseDatas *pbData = dynamic_cast<BaseDatas *>(data.data());
    QSharedPointer<DateClearTemp> dataClear = QSharedPointer<DateClearTemp>(new DateClearTemp(m_oBaseData));
    dataClear->dataClear();
    emit clearEnd(m_oBaseData.m_fullPath);
    m_bHasFinished = true;
}
