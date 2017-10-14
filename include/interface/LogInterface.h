#ifndef LogInterface_h__
#define LogInterface_h__
#include <QObject>

class LogInterface : public QObject
{
	Q_OBJECT
public slots:
	virtual void append(const QString &value) = 0;
};
#endif // LogInterface_h__
