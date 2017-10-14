#include <QDebug>
#include <QDateTime>

#include "test.h"

void CTestClass::newfile( const QString &file )
{
	qDebug() << QDateTime::currentDateTime() << "file was created "<< file;
}

void CTestClass::filerename( const QString &oldfile, const QString &newfile )
{
	qDebug() <<  QDateTime::currentDateTime() << oldfile << ">>>>>>>>>" << newfile;
}

void CTestClass::fileDeleted( const QString &file )
{
	qDebug() << QDateTime::currentDateTime() << "file was deleted "<< file;
}

void CTestClass::fileModified( const QString &file )
{
	qDebug() << QDateTime::currentDateTime() << "file was modified "<< file;
}

