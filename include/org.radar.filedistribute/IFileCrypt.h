#ifndef IFileCrypt_H_
#define IFileCrypt_H_


#include <QtPlugin>
#include <QObject>
#include "IBaseInterface.h"


typedef struct _KeyIv
{
    bool bComp;
    bool bCrypt;
    QByteArray key;
    QByteArray iv;
} KeyIv;

/**
  * @class <IFileCrypt>
  * @brief  加密解密类
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class IFileCrypt : public IBaseInterface
{
    Q_OBJECT
    Q_INTERFACES(IBaseInterface)

public:
    enum E_MODE {CBC = 0, ECB, CFB, OFB};	///< 加密模式

    /**
     * @brief  加密压缩文件流
     * @param  const QByteArray & raw：要加密的原始明文
     * @param  QByteArray & key：[out] 加密使用的密钥
     * @param  QByteArray & iv：[out] 加密使用的iv（初始化加密函数的变量）
     * @param  QByteArray & cipher：[out] 加密后的密文
     * @param  E_MODE mode CBC：AES加密模式
     * @return bool：加密成功返回true，否则返回false
     */
    virtual bool CompCryptFile(const QByteArray& raw, QByteArray& result, KeyIv& keyiv, const bool& bcomp, const bool& bencrypt) = 0;
    /**
     * @brief  解密解压文件流
     * @param  const QByteArray & raw：需要解密压缩的文件流
     * @param  QByteArray & result：解密压缩后的文件流
     * @param  const KeyIv & keyiv：加密密钥信息
     * @param  const bool & bcomp：是否压缩
     * @param  const bool & bencrypt：是否加密
     * @return bool：成功返回true，失败返回false
     */
    virtual bool UCompCryptFile(const QByteArray& raw, QByteArray& result, const KeyIv& keyiv, const bool& bcomp, const bool& bencrypt) = 0;

    /**
     * @brief  解密解压文件
     * @param  const QString & strFileFullPath：文件全路径
     * @param  QByteArray & result：解密解压后的结果
     * @return bool：成功返回true，失败返回false
     */
    virtual bool UCompCryptFile(const QString &strFileFullPath, QByteArray &result) = 0;

    /**
     * @brief  加密压缩落盘文件
     * @param  const QString & srcFilePath：原始路径
     * @param  const QString & dstFilePath：目标路径
     * @param  KeyIv & key
     * @return bool：无
     */
    virtual bool CompCryptFile(const QString &srcFilePath, const QString &dstFilePath, KeyIv &key) = 0;

    /**
     * @brief  解密解压落盘文件
     * @param  const QString & strFileFullPath：原始路径
     * @param  QString & resFullPath：目标路径
     * @return bool：无
     */
    virtual bool UCompCryptFile(const QString &strFileFullPath, QString &resFullPath) = 0;
};

Q_DECLARE_INTERFACE(IFileCrypt, "org.radar.filedistribute.IFileCrypt")

#endif // icrypt_h__
