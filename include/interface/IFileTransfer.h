#ifndef _H_H_IFILETRANSFER_H_H
#define _H_H_IFILETRANSFER_H_H
#include <QObject>

#include "../ctkProtocol/transferSet.h"

class IFileTransfer : public QObject
{
	Q_OBJECT
public slots:
	virtual void TransFile(const TransferSet &tset) = 0;
};
#endif // _H_H_IFILETRANSFER_H_H
