#include "datacollectionImp.h"
#include "ftpCollector.h"
#include "shareddirCollector.h"
#include "streamCollector.h"
#include "pathbuilder.h"
#include "IMakePath.h"
#include "IRadarPathAdapter.h"
#include "CollectManager.h"


DataCollectionImp::DataCollectionImp()
{
}

DataCollectionImp::~DataCollectionImp()
{
    stopComCollection();
}

QSharedPointer<ICollectManager> DataCollectionImp::createCollector(QWaitCondition &in_oCondition, QMutex &in_oLocker, int &logsize)
{
    return QSharedPointer<ICollectManager>(new CollectManager(getCtkPluginManager(), in_oCondition, in_oLocker, logsize));
}


QMap<QString, QString> DataCollectionImp::GetLastVtbPath(const QStringList &stations, const QString &strStart, const QString &strEnd)
{
    ICtkPluginManager *pManager = getCtkPluginManager();
    QObject *obj = pManager->getService("IRadarPathAdapter");
    QSharedPointer<IMakePath> pMakePath = (qobject_cast<IRadarPathAdapter *>(obj))->createRadarProductPath();

    QMap<QString, QString> result;
    for (int i=0; i<stations.size(); ++i)
    {
        QString strPath = pMakePath->getLastVtbPath(stations.at(i), strStart, strEnd);
        if (!strPath.isEmpty())
        {
            result[stations.at(i)] = strPath;
        }
    }

    return result;
}

void DataCollectionImp::stopComCollection()
{
    if (!m_pComCollector.isNull())
    {
        m_pComCollector->stopAllCollection();
    }
}

