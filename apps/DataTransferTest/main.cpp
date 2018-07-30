#include "curl/curl.h"
#include <QApplication>

#include <QProcess>
#include <QDebug>

#include "CollectorBase.h"
#include "ftpCollector.h"
#include "sftpCollector.h"
#include "subdirfilter.h"

 
QString getParam(const QString &section, const QString &key)
{
	QStringList parts = section.split("=");
	if (parts.size() != 2 || parts.at(0) != key)
	{
		return QString();
	}
	return parts.at(1).trimmed();
}

//FtpType=1/2,TransferMode=1/0,Ip=192.168.0.10, Port=21,User=test,Password=123456,InitDir =/home/nriet
int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

	if (argc != 2)
	{
		qDebug() << "param num is incorrect!";
		return -1;
	}
	curl_global_init(CURL_GLOBAL_ALL);

	QString strParam(argv[1]);
	QStringList oparams = strParam.split(",");

	int nFtpType = getParam(oparams.at(0), "FtpType").toInt();
	int nTransferMode = getParam(oparams.at(1), "TransferMode").toInt(); 
	QString strIp = getParam(oparams.at(2), "Ip"); 
	int nPort = getParam(oparams.at(3), "Port").toInt(); 
	QString strUser = getParam(oparams.at(4), "User");  
	QString strPassword = getParam(oparams.at(5), "Password");
	QString strInitDir = getParam(oparams.at(6), "InitDir");  

	QSharedPointer<FtpBase> FtpPtr;
	if (nFtpType == 1)  // ftp
	{
		FtpPtr = QSharedPointer<FtpBase>(new CFtp());
	}
	else // sftp
	{
		FtpPtr = QSharedPointer<FtpBase>(new SFtp());
	}

	// 使用主动，默认为被动
	if (1 == nTransferMode)
	{
		FtpPtr->setTransferMode(Active);
	}

	FtpPtr->connectToHost(strIp, nPort, strUser, strPassword);
	

	QStringList allInitDirs = strInitDir.split(";");
	for (int i = 0; i < allInitDirs.size() - 1; ++i)
	{
		QString strDir = allInitDirs.at(i);
		QString url = QString("ftp://%1:%2%3").arg(strIp).arg(nPort).arg(strDir);
		qDebug() << "**************connect to" << i << "dir************";
		if (CURLE_OK != FtpPtr->cd(strDir))
		{
			qDebug()
				<< "**************connect failure************"
				<< "\n"
				<< url
				<< "\n"
				<< "**************connect failure************";
		}
		else
		{
			qDebug()
				<< "**************connect success************"
				<< "\n"
				<< url
				<< "\n"
				<< "**************connect success************";
		}

	}

	qDebug() << "**************all dir test finish************";
    return app.exec();
}