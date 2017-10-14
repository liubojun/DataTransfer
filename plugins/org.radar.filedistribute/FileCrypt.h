#ifndef FileCrypt_H_
#define FileCrypt_H_

#include "IFileCrypt.h"
#include "ICrypt.h"
#include <QRegExp>

class FileCryptImp : public IFileCrypt
{
    Q_OBJECT
    Q_INTERFACES(IFileCrypt)
    Q_INTERFACES(IBaseInterface)
public:
    FileCryptImp();
    ~FileCryptImp();

    virtual bool CompCryptFile(const QByteArray& raw, QByteArray& result, KeyIv& keyiv, const bool& bcomp, const bool& bencrypt);
    virtual bool UCompCryptFile(const QByteArray& raw, QByteArray& result, const KeyIv& keyiv, const bool& bcomp, const bool& bencrypt);
    virtual bool UCompCryptFile(const QString &strFileFullPath, QByteArray &result);

    virtual bool CompCryptFile(const QString &srcFilePath, const QString &dstFilePath, KeyIv &key);
    virtual bool UCompCryptFile(const QString &strFileFullPath, QString &resFullPath);


private:
    ICrypt *getIcrypt();
    ICrypt *m_pIcrypt;
    QRegExp m_regMatch;

    int Compress(const char *srcName, const char *dstName);

    int UnCompress(const char *srcName, const char *dstName);
};

#endif