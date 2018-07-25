#include "sftpCollector.h"
#include "pathbuilder.h"
#include "ctkLog.h"
#include "publicthread.h"
#include "DistributeFile.h"
#include "change_name.h"
#include "curlftp.h"
#include "record.h"
#include "LibCurlSFtp.h"
#include "LibCurlFtp.h"

#include <QElapsedTimer>
#include <QCoreApplication>
#include <QProcess>
#include <QLockFile>
#include <QTime>


SFtpCollector::SFtpCollector(QWaitCondition &in_oCond, QMutex &in_oLocker, int &in_iLogsize)
	: FtpCollector(in_oCond, in_oLocker, in_iLogsize)
{

}

SFtpCollector::~SFtpCollector()
{

}

void SFtpCollector::getNewFiles(const CollectTask &in_oTask)
{
	QSharedPointer<FtpBase> pSFtp = QSharedPointer<FtpBase>(new SFtp());
	FtpCollector::getNewFiles(in_oTask, pSFtp);
}

void SFtpCollector::resetFtpInstance()
{
	m_pFtp = QSharedPointer<FtpBase>(new SFtp());
}



