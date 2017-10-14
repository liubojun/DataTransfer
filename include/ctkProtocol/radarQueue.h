#ifndef radarQueue_H_
#define radarQueue_H_

#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
#include "../macro.h"


class RadarQueue
{
public:
    string grpbName;
    string msgQueue;
    string achvQueue;
    string broadQueue;
};


DLL_EXPORT_DECL bool RadarQueueToXml(const string &strPath, const RadarQueue &lst);

DLL_EXPORT_DECL bool XmlToRadarQueue(const string &strPath, RadarQueue &lst);

#endif
