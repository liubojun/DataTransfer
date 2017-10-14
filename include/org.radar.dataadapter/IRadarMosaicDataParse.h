
#ifndef IRadarMosaicDataParse_h__
#define IRadarMosaicDataParse_h__

#include "ILogPrint.h"
#include "PPIStruct.h"
#include <QString>
#include <vector>
#include <string>
using namespace std;

class IRadarMosaicDataParse : public ILogPrint
{
	Q_OBJECT
public:
	IRadarMosaicDataParse(){}
	virtual ~IRadarMosaicDataParse(){}
public:
	//解析标准化后的组网数据
	virtual int readMosaicFile(const QString &filename, MOSAICDATAHEADER &mosaicDataHeader, std::vector<RADARSITEINFO> &pSiteInfo,
		std::vector<unsigned char> &pData, std::vector<short> &pDataStartPos)  = 0;

	//解析地方组网拼图数据
	virtual int parseRegionalMosaicFile(QString &filename) = 0;

	//解析地方组网拼图数据
	virtual int parseRegionalMosaicFile(const QString &filename, MOSAICDATAHEADER &mosaicDataHeader, std::vector<RADARSITEINFO> &pSiteInfo,
		std::vector<unsigned char> &pData, std::vector<short> &pDataStartPos)  = 0;
};
#endif // IRadarMosaicDataParse_h__


