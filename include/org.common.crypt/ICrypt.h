#ifndef icrypt_h__
#define icrypt_h__

#include <QtPlugin>
#include <QObject>
#include "IBaseInterface.h"

/**
  * @class <ICrypt>
  * @brief  加密解密类
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class ICrypt : public IBaseInterface
{
    Q_OBJECT
    Q_INTERFACES(IBaseInterface)

public:
    enum E_MODE {CBC = 0, ECB, CFB, OFB};	///< 加密模式

    /**
     * @brief  加密文件流
     * @param  const QByteArray & raw：要加密的原始明文
     * @param  QByteArray & key：[out] 加密使用的密钥
     * @param  QByteArray & iv：[out] 加密使用的iv（初始化加密函数的变量）
     * @param  QByteArray & cipher：[out] 加密后的密文
     * @param  E_MODE mode CBC：AES加密模式
     * @return bool：加密成功返回true，否则返回false
     */
    virtual bool encrypt(const QByteArray& raw, QByteArray& key, QByteArray& iv, QByteArray& cipher, E_MODE mode = /*E_MODE::*/CBC)=0;

    /**
     * @brief  解密文件流
     * @param  const QByteArray & cipher：待解密的密文
     * @param  const QByteArray & key：密文使用的加密秘钥
     * @param  const QByteArray & iv：密文加密时使用的iv
     * @param  QByteArray & recover：[out] 解密后的明文
     * @param  E_MODE mode CBC：AES加密模式
     * @return bool：解密成功返回true，否则返回false
     */
    virtual bool decrypt(const QByteArray& cipher, const QByteArray& key, const QByteArray& iv, QByteArray& recover, E_MODE mode = /*E_MODE::*/CBC)=0;

    /**
     * @brief  加密文件
     * @param  const char * inFilename：加密前文件全路径
     * @param  QByteArray & key：加密使用的密钥
     * @param  QByteArray & iv：加密使用的iv
     * @param  const char * outFilename：加密后文件全路径
     * @param  E_MODE mode CBC：AES加密模式
     * @return bool：加密成功返回true，否则返回false
     */
    virtual bool encryptFile(const char* inFilename, QByteArray& key, QByteArray& iv, const char *outFilename, E_MODE mode = /*E_MODE::*/CBC)=0;

    /**
     * @brief  解密文件
     * @param  const char * strInFilename：解密前文件全路径
     * @param  const QByteArray & key：使用的加密秘钥
     * @param  const QByteArray & iv：使用的加密iv
     * @param  const char * strOutFilename：解密后的文件全路径
     * @param  E_MODE mode CBC：AES加密模式
     * @return bool：解密成功返回true，否则返回false
     */
    virtual bool decryptFile(const char* strInFilename, const QByteArray& key, const QByteArray& iv, const char* strOutFilename, E_MODE mode = /*E_MODE::*/CBC)=0;

};

//Q_DECLARE_INTERFACE(ICrypt, "org.radar.ICrypt")
Q_DECLARE_INTERFACE(ICrypt, "org.common.crypt.ICrypt")

#endif // icrypt_h__
