/**
  * @file   ILogPrint.h
  * @brief  日志重定向输出.
  *
  *  这里是详细说明。
  * @author nriet
  * @par   Copyright (c):
  *          nriet.
  *         All Rights Reserved
  * @date   2016:04:11 
  * @note   这里是注意事项。
  * @version 1.0
  */  

#ifndef ILogPrint_h__
#define ILogPrint_h__

#include <QObject>

class ILogPrint : public QObject
{
	Q_OBJECT
public:
	virtual ~ILogPrint(){}

signals:
	void print(const QString &logInfo);
};

#define BIND_LOG(src, dst, s) QObject::connect(src, SIGNAL(print(const QString &)), dst, SLOT(s));

#endif // ILogPrint_h__