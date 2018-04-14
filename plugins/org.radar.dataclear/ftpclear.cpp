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
    using namespace FTP;
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
        QList<CFileInfo> oFileList = oFtp.list(m_oData.m_fullPath);


        QSharedPointer<CFtp> pFtp;
        for (int i = 0; i < oFileList.size(); ++i)
        {
            // 每个ftp连接对象管10个文件的删除,解决ftp连接超时问题
            if (i % 10 == 0)
            {
                pFtp.reset(new CFtp());
                pFtp->connectToHost(m_oData.ip, m_oData.port);
                pFtp->setTransferMode(m_oData.transfermode == 0 ? Active : Passive);
                if (0 != pFtp->login(m_oData.user, m_oData.password))
                {
                    i = i + 9;
                    QSLOG_DEBUG(pFtp->errorString());
                    continue;
                }
            }
            const CFileInfo &fi = oFileList.at(i);
            //qDebug() << fi.time << "--" << QDateTime::currentDateTime().addMSecs(-qint64(m_oData.m_time) * 1000);
            if (fi.type == FTP::FTP_FILE && fi.time < QDateTime::currentDateTime().addMSecs(-qint64(m_oData.m_time) * 1000))
            {
                if (0 != pFtp->remove(fi.path))
                {
                    QSLOG_DEBUG(QString::fromLocal8Bit("删除远程文件[%1]失败:%2").arg(fi.name).arg(pFtp->errorString()));
                }
                else
                {
                    m_oLogSender.sendClearMsg(m_oData.m_taskName, "", QString::fromLocal8Bit("删除文件:%1").arg(fi.name), 1);
                }
            }
        }
    }
    while (0);
    //m_oLogSender.sendClearMsg(m_oData.m_taskName, "", QString::fromLocal8Bit("目录[%1]清理结束").arg(CPathBuilder::getFtpUrl(m_oData.ip, m_oData.port, m_oData.m_fullPath)), 1);
}

