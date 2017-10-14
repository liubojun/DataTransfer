#include "FileCrypt.h"
#include <QFileInfo>
#include <QDataStream>

FileCryptImp::FileCryptImp()
    : m_pIcrypt(NULL)
{
    m_regMatch.setPattern(".[cu][eu]");
}

FileCryptImp::~FileCryptImp()
{

}

bool FileCryptImp::CompCryptFile(const QByteArray& raw, QByteArray& result, KeyIv& keyiv, const bool& bcomp, const bool& bencrypt)
{
    result = raw;

    // 先压缩
    if (bcomp)
    {
        result = qCompress(raw);
    }

    // 再加密
    if (bencrypt)
    {
        getIcrypt();
        if (m_pIcrypt == NULL)
        {
            return false;
        }
        QByteArray baCipher;
        if (m_pIcrypt->encrypt(result, keyiv.key, keyiv.iv, baCipher))
        {
            result = baCipher;
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool FileCryptImp::UCompCryptFile(const QByteArray& raw, QByteArray& result, const KeyIv& keyiv, const bool& bcomp, const bool& bencrypt)
{
    result.clear();

    // 先解密
    if (bencrypt)
    {
        getIcrypt();
        if (m_pIcrypt == NULL)
        {
            return false;
        }

        if (!m_pIcrypt->decrypt(raw, keyiv.key, keyiv.iv, result))
        {
            return false;
        }
    }

    // 再解压
    if (bcomp)
    {
        result = qUncompress(result);
    }

    return true;
}

// bool FileCryptImp::UCompCryptFile(const QString &strFileFullPath, QByteArray &result)
// {
// 	QString strKeyFile = strFileFullPath + ".key";
// 	QFileInfo fileInfo(strFileFullPath);
// 	if (!fileInfo.isFile())
// 	{
// 		return false;
// 	}
// 	QString strFileName = fileInfo.fileName();
//
// // 	QString strLast = strFileName.right(3);
// // 	if (!m_regMatch.exactMatch(strLast))
// // 	{
// // 		return false;
// // 	}
//
// // 	bool bComp = (strLast.mid(1,1) == "u") ? false : true;
// // 	bool bCrypt = (strLast.right(1) == "u") ? false : true;
//
// 	KeyIv keyiv;
// 	//if (bCrypt)
// 	{
// 		// 读取加密文件
// 		strFileName = strFileName.left(strFileName.length() - 3);
// 		//QString strKeyFullPath = fileInfo.path() + "/keys/" +strFileName + ".key";
// 		QString strKeyFullPath = fileInfo.path() +strFileName + ".key";
// 		QFileInfo fileInfo2(strKeyFullPath);
// 		if (!fileInfo2.isFile())
// 		{
// 			return false;
// 		}
// 		QFile fileKey(strKeyFullPath);
// 		if (!fileKey.open(QFile::ReadOnly))
// 		{
// 			return false;
// 		}
// // 		int a = fileKey.size();
// // 		int b = sizeof(KeyIv);
// // 		fileKey.read((char *)&keyiv, a);
// 		QDataStream in(&fileKey);
// 		in >> keyiv.key;
// 		in >> keyiv.iv;
// 		fileKey.close();
// 	}
//
// 	QFile fileData(strFileFullPath);
// 	if (!fileData.open(QFile::ReadOnly))
// 	{
// 		return false;
// 	}
// 	QByteArray baOrg = fileData.readAll();
// 	result.clear();
// 	return UCompCryptFile(baOrg, result, keyiv, bComp, bCrypt);
// }

bool FileCryptImp::UCompCryptFile(const QString &strFileFullPath, QByteArray &result)
{
    QFile fileInfo(strFileFullPath);
    if (!fileInfo.exists())
    {
        return false;
    }

    QString strKeyFullPath = strFileFullPath + ".key";
    if (!fileInfo.exists(strKeyFullPath))
    {
        return false;
    }

    // 读取密钥
    KeyIv keyiv;
    QFile fileKey(strKeyFullPath);
    if (!fileKey.open(QFile::ReadOnly))
    {
        return false;
    }
    QDataStream in(&fileKey);
    in >> keyiv.bComp;
    in >> keyiv.bCrypt;
    in >> keyiv.key;
    in >> keyiv.iv;
    fileKey.close();

    // 读取数据
    //QFile fileData(strFileFullPath);
    //if (!fileData.open(QFile::ReadOnly))
    //{
    //    return false;
    //}
    //QByteArray baOrg = fileData.readAll();
    //fileData.close();

    result.clear();

    //////////////////////////////////////////////////////////////////////////
    // 先解密
    if (keyiv.bCrypt)
    {
        getIcrypt();
        if (m_pIcrypt == NULL)
        {
            return false;
        }

        // QString转char*，一定要分步骤写!!
        QString resFullPath = strFileFullPath + ".jm";
        QByteArray baTmp1 = strFileFullPath.toLocal8Bit();
        QByteArray baTmp2 = resFullPath.toLocal8Bit();
        const char *oldPath = baTmp1.data();
        const char *newPath = baTmp2.data();
        if (!m_pIcrypt->decryptFile(oldPath, keyiv.key, keyiv.iv, newPath))
        {
            return false;
        }

        QFile file(resFullPath);
        if ( !file.open(QFile::ReadOnly) )
        {
            return false;
        }
        result = file.readAll();
        file.close();
    }

    // 再解压
    if (keyiv.bComp)
    {
        result = qUncompress(result);
    }

    return true;
    //////////////////////////////////////////////////////////////////////////

    //return UCompCryptFile(baOrg, result, keyiv, keyiv.bComp, keyiv.bCrypt);
}

bool FileCryptImp::UCompCryptFile(const QString &strFileFullPath, QString &resFullPath)
{
    QFile fileInfo(strFileFullPath);
    if (!fileInfo.exists())
    {
        return false;
    }

    QString strKeyFullPath = strFileFullPath + ".key";
    if (!fileInfo.exists(strKeyFullPath))
    {
        return false;
    }

    // 读取密钥
    KeyIv keyiv;
    QFile fileKey(strKeyFullPath);
    if (!fileKey.open(QFile::ReadOnly))
    {
        return false;
    }
    QDataStream in(&fileKey);
    in >> keyiv.bComp;
    in >> keyiv.bCrypt;
    in >> keyiv.key;
    in >> keyiv.iv;
    fileKey.close();

    // 先解密
    if (keyiv.bCrypt)
    {
        getIcrypt();
        if (m_pIcrypt == NULL)
        {
            return false;
        }

        const char *oldPath = strFileFullPath.toLocal8Bit().data();
        const char *newPath = resFullPath.toLocal8Bit().data();
        if (!m_pIcrypt->decryptFile(oldPath, keyiv.key, keyiv.iv, newPath))
        {
            return false;
        }
    }

    QFile file(resFullPath);
    QByteArray result;
    if ( file.open(QFile::ReadOnly) )
    {
        result = file.readAll();
        file.close();
    }

    // 再解压
    if (keyiv.bComp)
    {
        result = qUncompress(result);
    }

    return true;
}

ICrypt * FileCryptImp::getIcrypt()
{
    if (m_pIcrypt == NULL)
    {
        ICtkPluginManager *pManager = getCtkPluginManager();
        if (pManager != NULL)
        {
            QObject *pObj = pManager->getService("ICrypt");
            if (NULL != pObj)
            {
                m_pIcrypt = qobject_cast<ICrypt *>(pObj);
            }
        }
    }
    return m_pIcrypt;
}
