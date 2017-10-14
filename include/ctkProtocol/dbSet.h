#ifndef dbSet_H_
#define dbSet_H_


#include <QString>
#include <QMap>
#include <QVector>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
#include "../macro.h"


class DbSet
{
public:
    string	strIP;
    int		nPort;
    string	strUser;
    string	strPwd;
    string	strDb;

    // жиди==
    bool operator==(const DbSet& set)
    {
        return (this->strIP == set.strIP) && (this->nPort == set.nPort) &&
               (this->strUser == set.strUser) && (this->strPwd == set.strPwd)
               && (this->strDb == set.strDb);
    }
};

DLL_EXPORT_DECL bool DbSetToXml(const string &strPath, const DbSet &set);
DLL_EXPORT_DECL bool XmlToDbSet(const string &strPath, DbSet &set);


#endif
