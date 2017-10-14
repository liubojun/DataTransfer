#ifndef param_H_
#define param_H_

#include "transferSet.h"
#include "prdcTransferSet.h"

class TransParam
{
public:
    ~TransParam() {}
    static TransParam *getInstance();

    void init();
    bool loadUserInfo();
    bool saveUserInfo();
    bool loadTranSet();
    bool saveTranSet();
    bool loadPrdcTranSet();
    bool savePrdcTranSet();

    bool getUserInfo(string userId, SendSet &info);

    QMap<QString, QStringList> getAllUserShowInfo();

    QVector<string> getSelectUserID(const QString &taskID);

    //bool

    SendSetList m_userList;
    TransferSetList m_transetList;
    PrdcTransferSetList m_prdcTransetList;

private:
    TransParam();
    static TransParam s_param;

    QString m_userInfoPath;
    QString m_transetPath;
    QString m_prdcTransetPath;
};

#endif
