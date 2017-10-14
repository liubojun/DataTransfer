#ifndef CryptInfo_h__
#define CryptInfo_h__

#include "ICrypt.h"
#include "aes.h"
//#include "filters.h"
#include "modes.h"
#include "cryptlib.h"

// #pragma comment(lib, "cryptlib.lib")

USING_NAMESPACE(CryptoPP)

// Implement ICrypt
class CryptInfo : public ICrypt
{
    Q_OBJECT
    Q_INTERFACES(ICrypt)
public:
    CryptInfo() {};
    //CryptInfo(const QByteArray& key, const QByteArray& iv);
    ~CryptInfo() {};

    bool encrypt(const QByteArray& raw, QByteArray& key, QByteArray& iv, QByteArray& cipher, E_MODE mode);
    bool decrypt(const QByteArray& cipher, const QByteArray& key, const QByteArray& iv, QByteArray& recover, E_MODE mode);

    /**
     * @brief  加密文件
     * @param  const char * inFilename：源文件全路径
     * @param  QByteArray & key：
     * @param  QByteArray & iv：
     * @param  const char * outFilename：加密后文件路径
     * @param  E_MODE mode CBC
     * @return bool：无
     */
    bool encryptFile(const char* inFilename, QByteArray& key, QByteArray& iv, const char* outFilename, E_MODE mode = /*E_MODE::*/CBC);
    /**
     * @brief  解密文件
     * @param  const char * inFile：源文件全路径
     * @param  const QByteArray & key
     * @param  const QByteArray & iv
     * @param  const char * outFile：加密后文件路径
     * @param  E_MODE mode CBC
     * @return bool：无
     */
    bool decryptFile( const char* inFile, const QByteArray& key, const QByteArray& iv, const char* outFile, E_MODE mode = /*E_MODE::*/CBC );

    /**
     * @brief  产生密钥
     * @param  QByteArray & key
     * @param  QByteArray & iv
     * @return bool：无
     */
    bool genarateKeyIv(QByteArray &key, QByteArray &iv);

// private:
//     QByteArray m_key;
//     QByteArray m_iv;
};
#endif // CryptInfo_h__
