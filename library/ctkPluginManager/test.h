#ifndef test_h__
#define test_h__

#include <QObject>
#include <QString>

class CTestClass : public QObject
{
	Q_OBJECT
public slots:
	void newfile(const QString &file);

	void filerename(const QString &oldfile, const QString &newfile);

	void fileDeleted(const QString &file);

	void fileModified(const QString &file);
};
#endif // test_h__
