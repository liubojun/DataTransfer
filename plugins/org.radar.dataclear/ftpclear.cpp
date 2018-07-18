#include "ftpclear.h"
#include "BaseDatas.h"
#include "pathbuilder.h"

#include <QSharedPointer>
CFtpClear::CFtpClear(BaseDatas &in_oData) : m_oData(in_oData)
{

}

CFtpClear::~CFtpClear()
{

}

void CFtpClear::run()
{
    //m_oLogSender.sendClearMsg(m_oData.m_taskName, "", QString::fromLocal8Bit("开始目录[%1]清理").arg(CPathBuilder::getFtpUrl(m_oData.ip, m_oData.port, m_oData.m_fullPath)), 1);
    CFtp oFtp;
    do
    {
        if (0 != oFtp.connectToHost(m_oData.ip, m_oData.port))
        {
            break;
        }
        if (0 != oFtp.setTransferMode(m_oData.transfermode == 0 ? Active : Passive))
        {
            break;
        }
        if (0 != oFtp.login(m_oData.user, m_oData.password))
        {
            break;
        }
        QList<CFileInfo> oFileList = oFtp.listRecursion(m_oData.m_fullPath);


        QSharedPointer<FtpBase> pFtp;
        pFtp.reset(new CFtp());
        pFtp->connectToHost(m_oData.ip, m_oData.port);
        pFtp->setTransferMode(m_oData.transfermode == 0 ? Active : Passive);
        if (0 != pFtp->login(m_oData.user, m_oData.password))
        {
            QSLOG_DEBUG(pFtp->errorString());
            break;
        }
        for (int i = 0; i < oFileList.size(); ++i)
        {

            const CFileInfo &fi = oFileList.at(i);
            //qDebug() << fi.time << "--" << QDateTime::currentDateTime().addMSecs(-qint64(m_oData.m_time) * 1000);
            if (fi.type == FTP_FILE && fi.time < QDateTime::currentDateTime().addMSecs(-qint64(m_oData.m_time) * 1000))
            {
                if (0 != pFtp->remove(fi.path))
                {
                    QSLOG_DEBUG(QString::fromLocal8Bit("删除远程文件[%1]失败:%2").arg(fi.name).arg(pFtp->errorString()));
                }
                else
                {
                    m_oLogSender.sendClearMsg(m_oData.m_taskName, "", QString::fromLocal8Bit("删除文件:%1").arg(fi.name), 1);
                    QSLOG_DEBUG(QString::fromLocal8Bit("删除远程文件[%1]成功").arg(fi.name));
                }
            }
            if (fi.type == FTP_DIR && fi.time < QDateTime::currentDateTime().addMSecs(-qint64(m_oData.m_time) * 1000))
            {
                if (0 != pFtp->rmdir(fi.path))
                {
                    QSLOG_DEBUG(QString::fromLocal8Bit("删除远程目录[%1]失败:%2").arg(fi.name).arg(pFtp->errorString()));
                }
                else
                {
                    m_oLogSender.sendClearMsg(m_oData.m_taskName, "", QString::fromLocal8Bit("删除空目录:%1").arg(fi.name), 1);
                }
            }
        }
    }
    while (0);
    //m_oLogSender.sendClearMsg(m_oData.m_taskName, "", QString::fromLocal8Bit("目录[%1]清理结束").arg(CPathBuilder::getFtpUrl(m_oData.ip, m_oData.port, m_oData.m_fullPath)), 1);
}

