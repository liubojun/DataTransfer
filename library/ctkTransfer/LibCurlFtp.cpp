#include "LibCurlFtp.h"
#include <QDataStream>
#include <stdio.h>

using namespace FTP;
CFtp::CFtp()
{
    m_pCurlHandler = NULL;
    m_pCurlHandler = curl_easy_init();
	m_pSecCurlHandler = curl_easy_init();
    enableDebugLevel(true);

}

CFtp::~CFtp()
{
    close();
}

int CFtp::cd(const QString &dir)
{
    QString url = makeUrl(dir);
    m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_URL, url.toLocal8Bit().toStdString().c_str());
    prepare();
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
	if (NULL != m_pSecCurlHandler)
	{
		curl_easy_cleanup(m_pSecCurlHandler);
		m_pSecCurlHandler = NULL;
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
    // ��֪��ǰ����Ŀ¼
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
    // �ر��ļ�
    if (fileData.stream)
    {
        fclose(fileData.stream);
    }
    return m_iRetCode;
}


int FTP::CFtp::get(const QString &file, QIODevice *dev, TransferType type /*= Binary*/)
{
    // ��֪��ǰ����Ŀ¼
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


double CFtp::getFileSize(const QString &file)
{
	QString filename, url;
	if (file.startsWith("/"))
	{
		filename = file.split("/").last();
		QString dir = file.mid(0, file.lastIndexOf("/"));
		url = makeUrl(dir);
	}
	else
	{
		filename = file;
		url = makeUrl("");
	}
	url = url + filename;
	m_iRetCode = curl_easy_setopt(m_pSecCurlHandler, CURLOPT_URL, url.toLocal8Bit().toStdString().c_str());
	m_iRetCode = curl_easy_setopt(m_pSecCurlHandler, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(m_pSecCurlHandler, CURLOPT_NOBODY, 1);
	curl_easy_setopt(m_pSecCurlHandler, CURLOPT_HEADER, 0);
	curl_easy_setopt(m_pSecCurlHandler, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(m_pSecCurlHandler, CURLOPT_VERBOSE, 1L);
	//prepare();
	struct MemoryData listInfo;
	m_iRetCode = curl_easy_setopt(m_pSecCurlHandler, CURLOPT_WRITEDATA, (void *)&listInfo);
	m_iRetCode = curl_easy_setopt(m_pSecCurlHandler, CURLOPT_WRITEFUNCTION, WriteInMemoryFun);
	m_iRetCode = curl_easy_perform(m_pSecCurlHandler);
	double fileSize = 0.0;
	m_iRetCode = curl_easy_getinfo(m_pSecCurlHandler, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &fileSize);
	return fileSize;
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

	m_iRetCode = curl_easy_setopt(m_pSecCurlHandler, CURLOPT_USERNAME, user.toLocal8Bit().toStdString().c_str());
	m_iRetCode = curl_easy_setopt(m_pSecCurlHandler, CURLOPT_PASSWORD, password.toLocal8Bit().toStdString().c_str());
	

    prepare();
    m_iRetCode = curl_easy_perform(m_pCurlHandler);
    return m_iRetCode;
}

int CFtp::mkdir(const QString &dir)
{
    return 0;
}

int CFtp::put(const QString & localFile, const QString & remoteFile, const QString &suffix /*= QString(".tmp")*/, TransferType type /*= Binary*/)
{
	QString filename, url;
	if (remoteFile.startsWith("/"))
	{
		filename = remoteFile.split("/").last();
		QString dir = remoteFile.mid(0, remoteFile.lastIndexOf("/"));
		url = makeUrl(dir);
	}
	else
	{
		filename = remoteFile;
		url = makeUrl("");
	}

	

	QString cmdTmpName = "RNFR " + filename + suffix;
	QString cmdOrgName = "RNTO " + filename;
	QString strUrl = url + filename + suffix;
	//strUrl += filename + sendsuffix;
	struct curl_slist *headerlist = NULL;
	headerlist = curl_slist_append(headerlist, cmdTmpName.toLocal8Bit().toStdString().c_str());
	headerlist = curl_slist_append(headerlist, cmdOrgName.toLocal8Bit().toStdString().c_str());
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_UPLOAD, 1);
	//////////////////////////////////////////////////////////////////////////
	// �ϵ�����
	std::FILE *pfile = fopen(localFile.toLocal8Bit().toStdString().c_str(), "rb");
	if (pfile == NULL)
	{
		QSLOG_ERROR(QString("open file %1 failure").arg(localFile));
		return -1;
	}
	QSharedPointer<std::FILE> autoclose(pfile, fclose);
	double size = getFileSize(remoteFile);
	fseek(pfile,0 , SEEK_END);
	long fileLen = ftell(pfile);
	if (size != double(-1.0) )
	{	
		if ((long)size != fileLen)
		{
			if (0 != remove(remoteFile))
			{
				QSLOG_ERROR(errorString());
			}
		}
		else
		{
			// �ļ��Ѿ�����
			return m_iRetCode;
		}
	}
	fseek(pfile, 0, SEEK_SET);


	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_APPEND, 0);
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_URL, strUrl.toLocal8Bit().toStdString().c_str());
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_READDATA, pfile);
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_READFUNCTION, ReadFromFile);
	m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_POSTQUOTE, headerlist);
	m_iRetCode = curl_easy_perform(m_pCurlHandler);
	curl_slist_free_all(headerlist);
	return m_iRetCode;
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
    QString filename, url;
    if (file.startsWith("/"))
    {
        filename = file.split("/").last();
        QString dir = file.mid(0, file.lastIndexOf("/"));
        url = makeUrl(dir);
    }
    else
    {
        filename = file;
        url = makeUrl("");
    }
    QString szCmd = QString("DELE %1").arg(filename);
    //m_iRetCode = curl_easy_setopt(m_pCurlHandler, CURLOPT_URL, url);
    struct curl_slist *headerlist = NULL;
    headerlist = curl_slist_append(headerlist, szCmd.toLocal8Bit().toStdString().c_str());
    m_iRetCode = curl_easy_setopt(m_pSecCurlHandler, CURLOPT_POSTQUOTE, headerlist);
	m_iRetCode = curl_easy_setopt(m_pSecCurlHandler, CURLOPT_URL, url.toLocal8Bit().toStdString().c_str());
    prepare();
	m_iRetCode = curl_easy_perform(m_pSecCurlHandler);
    curl_slist_free_all(headerlist);
    return m_iRetCode;
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
        // �Ƚ��뵽root��ʼĿ¼
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
            // ����һ��Ŀ¼
            m_urlBody.pop_back();
            if (m_urlBody.size() < 1)
            {
                qDebug() << "error";
            }
        }
        else
        {
            // appendһ��Ŀ¼
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

        // ���� ʱ�� ��С �ļ���
        if (lstParts.size() == 4)
        {
            //QSLOG_ERROR(QString::fromLocal8Bit("��������ftp����..."));

            process4Ftp(oneInfo, lstParts);
        }
        // -rw-r--r--    1 0        0        61456824 May 10 06:59 Z_NAFP_C_BABJ_2017050912_P_CAMS-GRAPES-ACHN-03.grd
        else if (lstParts.size() == 9)
        {
            // 4 �Ǵ�С�� 5���·ݣ�6�����ڣ�7��ʱ�֣�8���ļ���
            process9Ftp(oneInfo, lstParts);
        }

        if (oneInfo.nType == 1)
        {
            CFileInfo fInfo;
            fInfo.name = oneInfo.strFileName;
            fInfo.size = oneInfo.nFileSize;
            fInfo.path = currentDir + oneInfo.strFileName;
            fInfo.time = QDateTime::fromString(oneInfo.strMdfyTime, "yyyyMMddhhmmss");
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
                // ���������12�㣬����ΪPM
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
        //QSLOG_ERROR(QString::fromLocal8Bit("ftp Ŀǰ��ʱ��������Ŀ¼����."));
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
    // 4 �Ǵ�С�� 5���·ݣ�6�����ڣ�7��ʱ�֣�8���ļ���
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