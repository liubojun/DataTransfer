#include "ftpclear.h"
#include "BaseDatas.h"
CFtpClear::CFtpClear(BaseDatas &in_oData) : m_oData(in_oData)
{

}

CFtpClear::~CFtpClear()
{

}

void CFtpClear::run()
{
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
		
		foreach(const CFileInfo &fi, oFileList)
		{
			//qDebug() << fi.time << "--" << QDateTime::currentDateTime().addMSecs(-qint64(m_oData.m_time) * 1000);
			if (fi.type == FTP::FILE && fi.time < QDateTime::currentDateTime().addMSecs(-qint64(m_oData.m_time) * 1000))
			{
				if (0 != oFtp.remove(fi.name))
				{
					QSLOG_DEBUG(oFtp.errorString());
				}
			}
		}
	} while (0);

}

