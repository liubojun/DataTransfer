#include "LibCurlFtp.h"
#include <QDataStream>

using namespace FTP;
CFtp::CFtp()
{
	m_pCurlHandler = NULL;
	m_pCurlHandler = curl_easy_init();
	enableDebugLevel(true);
	prepare();

}

CFtp::~CFtp()
{
	close();
}

int CFtp::cd(const QString &dir)
{
	QString url = makeUrl(dir);
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_URL, url.toLocal8Bit().toStdString().c_str());
	m_iRetCode = curl_easy_perform(m_pCurlHandler);
	return m_iRetCode;
}

int CFtp::close()
{
	if (NULL != m_pCurlHandler)
	{
		curl_easy_cleanup(m_pCurlHandler);
		m_pCurlHandler = NULL;
	}
	return 0;
}

int CFtp::connectToHost(const QString &host, quint16 port /*= 21*/)
{
	QString url = makeUrl(host, port);
	
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_NOSIGNAL, 1L);
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_URL, url.toLocal8Bit().toStdString().c_str());
	return m_iRetCode;
}

QString CFtp::errorString()
{
	if (CURLE_OK != m_iRetCode)
	{
		return curl_easy_strerror(m_iRetCode);
	}
	return QString();
}


int CFtp::enableDebugLevel(bool flag /*= true*/)
{
	if (flag)
	{
		m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_VERBOSE, 1L);
	}
	else
	{
		m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_VERBOSE, 0);
	}
	return m_iRetCode;
}


int CFtp::get(const QString &file, const QString &dir, TransferType type /*= Binary*/)
{
	// 获知当前所在目录
	QString url = makeUrl("");
	url.append(file);
	struct FileData fileData;
	std::string localfilepath = (dir + "/" + file).toLocal8Bit().toStdString();
	fileData.filename = localfilepath.c_str();
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_WRITEFUNCTION, WriteInFileFun);
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_WRITEDATA, &fileData);
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_URL, url.toLocal8Bit().toStdString().c_str());
	if (Ascii == type)
	{
		m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_TRANSFERTEXT, 1L);
	}
	else
	{
		m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_TRANSFERTEXT, 0);
	}
	m_iRetCode = curl_easy_perform(m_pCurlHandler);
	// 关闭文件
	if (fileData.stream)
	{
		fclose(fileData.stream);
	}
	return m_iRetCode;
}


int FTP::CFtp::get(const QString &file, QIODevice *dev, TransferType type /*= Binary*/)
{
	// 获知当前所在目录
	QString url = makeUrl("");
	url.append(file);

	MemoryData oMemData;
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_WRITEFUNCTION, WriteInMemoryFun);
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_WRITEDATA, &oMemData);
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_URL, url.toLocal8Bit().toStdString().c_str());
	if (Ascii == type)
	{
		m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_TRANSFERTEXT, 1L);
	}
	else
	{
		m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_TRANSFERTEXT, 0);
	}
	m_iRetCode = curl_easy_perform(m_pCurlHandler);

	QDataStream stream(dev);
	stream.writeBytes(oMemData.memdata, oMemData.size);
	return m_iRetCode;
}


QList<CFileInfo> CFtp::list(const QString &dir /*= QString()*/)
{
	if (!dir.isEmpty())
	{
		QString url = makeUrl(dir);
		m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_URL, url.toLocal8Bit().toStdString().c_str());
	}
	
	MemoryData t_oListInfo;
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_WRITEDATA, (void *)&t_oListInfo);
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_CUSTOMREQUEST, "LIST");
	m_iRetCode = curl_easy_perform(m_pCurlHandler);
	QString strInfo(t_oListInfo.memdata);
	QList<CFileInfo> files;
	
	parseMlsdInfo(getCurrentUrl(), strInfo, files);
	return files;
}

int CFtp::login(const QString &user /*= QString()*/, const QString &password /*= QString()*/)
{
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_USERNAME, user.toLocal8Bit().toStdString().c_str());
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_PASSWORD, password.toLocal8Bit().toStdString().c_str());
	m_iRetCode = curl_easy_perform(m_pCurlHandler);
	return m_iRetCode;
}

int CFtp::mkdir(const QString &dir)
{
	return 0;
}

int CFtp::put(QIODevice * dev, const QString & file, TransferType type /*= Binary*/)
{
	return 0;
}

int CFtp::put(const QByteArray & data, const QString & file, TransferType type /*= Binary*/)
{
	return 0;
}

int CFtp::rename(const QString &oldname, const QString &newname)
{
	return 0;
}


int CFtp::remove(const QString &file)
{
	return 0;
}


int CFtp::rmdir(const QString &dir)
{
	return 0;
}

int CFtp::setTransferMode(TransferMode mode)
{
	if (mode == Active)
	{
		m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_FTPPORT, "-");
	}
	return m_iRetCode;
}

QString CFtp::makeUrl(const QString &host, quint16 port)
{
	m_urlBody.append(QString("ftp://%1:%2").arg(host).arg(port));
	return makeUrl("");
}

QString FTP::CFtp::makeUrl(const QString &dir)
{
	QStringList dirbodies = dir.split("/");
	if (dir.startsWith("/"))
	{
		// 先进入到root初始目录
		while (m_urlBody.size() > 1)
		{
			m_urlBody.pop_back();
		}

	}
	foreach(QString str ,dirbodies)
	{
		if (str.trimmed().isEmpty())
		{
			continue;
		}
		if (str == "..")
		{
			// 回退一层目录
			m_urlBody.pop_back();
			if (m_urlBody.size() < 1)
			{
				qDebug() << "error";
			}
		}
		else
		{
			// append一层目录
			m_urlBody.append(str);
		}
	}
	QString url;
	for (int i = 0; i < m_urlBody.size(); ++i)
	{
		url.append(m_urlBody.at(i)).append("/");
	}
	return url;
}

QString FTP::CFtp::getCurrentUrl()
{
	QString url("/");
	for (int i = 1; i < m_urlBody.size(); ++i)
	{
		url.append(m_urlBody.at(i)).append("/");
	}
	return url;
}

void CFtp::prepare()
{
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_WRITEDATA, (void *)&listInfo);
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_WRITEFUNCTION, WriteInMemoryFun);
}

void CFtp::parseMlsdInfo(const QString &currentDir, const QString &info, QList<CFileInfo> &fileList)
{
	QStringList lstLines = info.split("\n");
	for (int i = 0; i < lstLines.size(); ++i)
	{
		QString strLine = lstLines.at(i).trimmed();
		if (strLine.isEmpty())
		{
			continue;
		}

		CListInfo oneInfo;
		QStringList lstParts;

		lstParts = strLine.split(" ");
		QStringList tmpParts;
		foreach(QString str, lstParts)
		{
			if (!str.isEmpty())
			{
				tmpParts.append(str);
			}
		}
		lstParts = tmpParts;

		// 日期 时间 大小 文件名
		if (lstParts.size() == 4)
		{
			//QSLOG_ERROR(QString::fromLocal8Bit("发现特殊ftp返回..."));

			process4Ftp(oneInfo, lstParts);
		}
		// -rw-r--r--    1 0        0        61456824 May 10 06:59 Z_NAFP_C_BABJ_2017050912_P_CAMS-GRAPES-ACHN-03.grd
		else if (lstParts.size() == 9)
		{
			// 4 是大小， 5是月份，6是日期，7是时分，8是文件名
			process9Ftp(oneInfo, lstParts);
		}

		if (oneInfo.nType == 1)
		{
			CFileInfo fInfo;
			fInfo.name = oneInfo.strFileName;
			fInfo.size = oneInfo.nFileSize;
			fInfo.path = currentDir + oneInfo.strFileName;
			fInfo.type = FILE;
			fileList.push_back(fInfo);
			
		}
		else if (oneInfo.nType == 2)
		{
			if (oneInfo.strFileName == "." || oneInfo.strFileName == "..")
			{
				continue;
			}
			CFileInfo fInfo;
			fInfo.name = oneInfo.strFileName;
			fInfo.size = oneInfo.nFileSize;
			fInfo.path = currentDir + oneInfo.strFileName;
			fInfo.type = DIR;
			fileList.push_back(fInfo);
		}
	}
}

void CFtp::process4Ftp(CListInfo &oneInfo, const QStringList &lstParts)
{
	if (lstParts[2].toUpper() != "<DIR>")
	{

		oneInfo.strFileName = lstParts[3];
		oneInfo.nFileSize = lstParts[2].toLongLong();
		oneInfo.nType = 1;
		//05-07-17 12:23AM

		// oneInfo.strMdfyTime = QDateTime::fromString(lstParts[0] + lstParts[1] , "MM-dd-yyhh:mm").toString("yyyyMMddhhmmss");
		if (lstParts[1].length() >= 8 && lstParts[1].mid(5, 2).toUpper() == "AM")
		{

			oneInfo.strMdfyTime = QDateTime(QDate((QString::number(QDate::currentDate().year()).mid(0, 2) + lstParts[0].mid(6, 2)).toInt(), lstParts[0].mid(0, 2).toInt(), lstParts[0].mid(3, 2).toInt()), QTime(lstParts[1].mid(0, 2).toInt(), lstParts[1].mid(3, 2).toInt(), 0)).toString("yyyyMMddhhmmss");
		}
		else
		{
			if (lstParts[1].length() >= 8 && lstParts[1].mid(0, 2).toInt() == 12)
			{
				// 如果到下午12点，正好为PM
				oneInfo.strMdfyTime = QDateTime(QDate((QString::number(QDate::currentDate().year()).mid(0, 2) + lstParts[0].mid(6, 2)).toInt(), lstParts[0].mid(0, 2).toInt(), lstParts[0].mid(3, 2).toInt()), QTime(lstParts[1].mid(0, 2).toInt(), lstParts[1].mid(3, 2).toInt(), 0)).toString("yyyyMMddhhmmss");
			}
			else
			{
				oneInfo.strMdfyTime = QDateTime(QDate((QString::number(QDate::currentDate().year()).mid(0, 2) + lstParts[0].mid(6, 2)).toInt(), lstParts[0].mid(0, 2).toInt(), lstParts[0].mid(3, 2).toInt()), QTime(lstParts[1].mid(0, 2).toInt() + 12, lstParts[1].mid(3, 2).toInt(), 0)).toString("yyyyMMddhhmmss");
			}

		}

	}
	else
	{
		oneInfo.strFileName = lstParts[3];
		oneInfo.nType = 2;
		//QSLOG_ERROR(QString::fromLocal8Bit("ftp 目前暂时不考虑子目录问题."));
	}
}

void CFtp::process9Ftp(CListInfo &oneInfo, const QStringList &lstParts)
{
	//1 = "Jan"
	//2 = "Feb"
	//3 = "Mar"
	//4 = "Apr"
	//5 = "May"
	//6 = "Jun"
	//7 = "Jul"
	//8 = "Aug"
	//9 = "Sep"
	//10 = "Oct"
	//11 = "Nov"
	//12 = "Dec"
	// -rw-r--r--    1 0        0        61456824 May 10 06:59 Z_NAFP_C_BABJ_2017050912_P_CAMS-GRAPES-ACHN-03.grd
	// 4 是大小， 5是月份，6是日期，7是时分，8是文件名
	oneInfo.strFileName = lstParts[8];
	oneInfo.nFileSize = lstParts[4].toLongLong();

	if (lstParts[0].startsWith("d"))
	{
		oneInfo.nType = 2;
	}
	else
	{
		oneInfo.nType = 1;
	}


	int month = 0;
	if (lstParts[5] == "Jan")
	{
		month = 1;
	}
	else if (lstParts[5] == "Feb")
	{
		month = 2;
	}

	else if (lstParts[5] == "Mar")
	{
		month = 3;
	}
	else if (lstParts[5] == "Apr")
	{
		month = 4;
	}
	else if (lstParts[5] == "May")
	{
		month = 5;
	}
	else if (lstParts[5] == "Jun")
	{
		month = 6;
	}
	else if (lstParts[5] == "Jul")
	{
		month = 7;
	}
	else if (lstParts[5] == "Aug")
	{
		month = 8;
	}
	else if (lstParts[5] == "Sep")
	{
		month = 9;
	}
	else if (lstParts[5] == "Oct")
	{
		month = 10;
	}
	else if (lstParts[5] == "Nov")
	{
		month = 11;
	}
	else if (lstParts[5] == "Dec")
	{
		month = 12;
	}
	QString dt = QString("%1-%2-%3 %4").arg(QDate::currentDate().year()).arg(month).arg(lstParts[6]).arg(lstParts[7]);
	oneInfo.strMdfyTime = QDateTime::fromString(dt, "yyyy-M-dd hh:mm").toString("yyyyMMddhhmmss");

}