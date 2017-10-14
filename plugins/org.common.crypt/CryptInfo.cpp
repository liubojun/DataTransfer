#include "CryptInfo.h"
#include "osrng.h"
#include <string>
#include <QByteArray>
#include <QDebug>
#include <QString>
#include <stdlib.h>
#include <io.h>
#include "hex.h"
#include "files.h"

using namespace std;


bool CryptInfo::genarateKeyIv(QByteArray &key, QByteArray &iv)
{
    try
    {
        key.clear();
        iv.clear();
        key.resize(AES::DEFAULT_KEYLENGTH);
        iv.resize(AES::BLOCKSIZE);

        AutoSeededRandomPool rnd;
        rnd.GenerateBlock(reinterpret_cast<byte *>(key.data()), AES::DEFAULT_KEYLENGTH);
        rnd.GenerateBlock(reinterpret_cast<byte *>(iv.data()), AES::BLOCKSIZE);
    }
    catch (std::exception& e)
    {
        qDebug() << "genarateKeyIv error: " << e.what();
        return false;
    }

    return true;
}

bool CryptInfo::encrypt(const QByteArray& raw, QByteArray& key, QByteArray& iv, QByteArray& cipher, E_MODE mode)
{
    QByteArray m_key;
    QByteArray m_iv;

    if (!genarateKeyIv(m_key, m_iv))
    {
        return false;
    }

    string strCipher;
    bool isSucc = false;

    AESEncryption aesEnc(reinterpret_cast<byte *>(m_key.data()), AES::DEFAULT_KEYLENGTH);
    CBC_Mode_ExternalCipher::Encryption *pcbcEnc = NULL;
    ECB_Mode_ExternalCipher::Encryption *pecbEnc = NULL;
    CFB_Mode_ExternalCipher::Encryption *pcfbEnc = NULL;
    OFB_Mode_ExternalCipher::Encryption *pofbEnc = NULL;
    StreamTransformationFilter *pstfEnc = NULL;
    switch (mode)
    {
    case /*E_MODE::*/CBC :
        pcbcEnc = new CBC_Mode_ExternalCipher::Encryption(aesEnc, reinterpret_cast<byte *>(m_iv.data()));
        if (pcbcEnc)
            pstfEnc = new StreamTransformationFilter(*pcbcEnc, new HexEncoder(new StringSink(strCipher)));
        break;

    case /*E_MODE::*/ECB :
        pecbEnc = new ECB_Mode_ExternalCipher::Encryption(aesEnc, reinterpret_cast<byte *>(m_iv.data()));
        if (pecbEnc)
            pstfEnc = new StreamTransformationFilter(*pecbEnc, new HexEncoder(new StringSink(strCipher)));
        break;

    case /*E_MODE::*/CFB :
        pcfbEnc = new CFB_Mode_ExternalCipher::Encryption(aesEnc, reinterpret_cast<byte *>(m_iv.data()));
        if (pcfbEnc)
            pstfEnc = new StreamTransformationFilter(*pcfbEnc, new HexEncoder(new StringSink(strCipher)));
        break;

    case /*E_MODE::*/OFB :
        pofbEnc = new OFB_Mode_ExternalCipher::Encryption(aesEnc, reinterpret_cast<byte *>(m_iv.data()));
        if (pofbEnc)
            pstfEnc = new StreamTransformationFilter(*pofbEnc, new HexEncoder(new StringSink(strCipher)));
        break;

    default:
        break;
    }
    if (pstfEnc)
    {
        pstfEnc->Put(reinterpret_cast<const unsigned char*>(raw.data()), raw.size());
        pstfEnc->MessageEnd();

        cipher.clear();
        cipher.append(strCipher.c_str());

        key = m_key;
        iv = m_iv;

        isSucc = true;
        delete pstfEnc;
        pstfEnc = NULL;
    }
    if (pcbcEnc)
    {
        delete pcbcEnc;
        pcbcEnc = NULL;
    }
    if (pecbEnc)
    {
        delete pecbEnc;
        pecbEnc = NULL;
    }
    if (pcfbEnc)
    {
        delete pcfbEnc;
        pcfbEnc = NULL;
    }
    if (pofbEnc)
    {
        delete pofbEnc;
        pofbEnc = NULL;
    }

    return isSucc;
}

bool CryptInfo::decrypt(const QByteArray& cipher, const QByteArray& key, const QByteArray& iv, QByteArray& recover, E_MODE mode)
{
    string decryptedText;
    bool isSucc = false;

    AES::Decryption aesDec(reinterpret_cast<byte *>(const_cast<char *>(key.data())), AES::DEFAULT_KEYLENGTH);
    CBC_Mode_ExternalCipher::Decryption *pcbcDec = NULL;
    ECB_Mode_ExternalCipher::Decryption *pecbDec = NULL;
    CFB_Mode<AES>::Decryption *pcfbDec = NULL;
    OFB_Mode<AES>::Decryption *pofbDec = NULL;
    HexDecoder *hexDec = NULL;
    switch (mode)
    {
    case /*E_MODE::*/CBC :
        pcbcDec = new CBC_Mode_ExternalCipher::Decryption(aesDec, reinterpret_cast<byte *>(const_cast<char *>(iv.data())));
        if (pcbcDec)
            hexDec = new HexDecoder(new StreamTransformationFilter(*pcbcDec, new StringSink(decryptedText)));
        break;

    case /*E_MODE::*/ECB :
        pecbDec = new ECB_Mode_ExternalCipher::Decryption(aesDec, reinterpret_cast<byte *>(const_cast<char *>(iv.data())));
        if (pecbDec)
            hexDec = new HexDecoder(new StreamTransformationFilter(*pecbDec, new StringSink(decryptedText)));
        break;

    case /*E_MODE::*/CFB :
        pcfbDec = new CFB_Mode<AES>::Decryption(reinterpret_cast<byte *>(const_cast<char *>(key.data())),
                                                AES::DEFAULT_KEYLENGTH, reinterpret_cast<byte *>(const_cast<char *>(iv.data())));
        if (pcfbDec)
            hexDec = new HexDecoder(new StreamTransformationFilter(*pcfbDec, new StringSink(decryptedText)));
        break;

    case /*E_MODE::*/OFB :
        pofbDec = new OFB_Mode<AES>::Decryption(reinterpret_cast<byte *>(const_cast<char *>(key.data())),
                                                AES::DEFAULT_KEYLENGTH, reinterpret_cast<byte *>(const_cast<char *>(iv.data())));
        if (pofbDec)
            hexDec = new HexDecoder(new StreamTransformationFilter(*pofbDec, new StringSink(decryptedText)));
        break;

    default:
        break;
    }
    if (NULL != hexDec)
    {
        try
        {
            hexDec->Put(reinterpret_cast<const unsigned char *>(cipher.data()), cipher.size());
            hexDec->MessageEnd();
        }
        catch (std::exception *e)
        {
            cout << e->what();
        }

        recover.clear();
        recover.append(decryptedText.c_str(), decryptedText.length());

        isSucc = true;
        delete hexDec;
        hexDec = NULL;
    }
    if (pcbcDec)
    {
        delete pcbcDec;
        pcbcDec = NULL;
    }
    if (pecbDec)
    {
        delete pecbDec;
        pecbDec = NULL;
    }
    if (pcfbDec)
    {
        delete pcfbDec;
        pcfbDec = NULL;
    }
    if (pofbDec)
    {
        delete pofbDec;
        pofbDec = NULL;
    }

    return isSucc;
}

bool CryptInfo::encryptFile( const char* inFile, QByteArray& key, QByteArray& iv, const char* outFile, E_MODE mode /*= E_MODE::CBC*/ )
{
    QByteArray m_key, m_iv;

    bool isSucc = false;

    if (-1 == _access(inFile, 0))
    {
        qDebug() << "The file: " << inFile << "is not exist!";
        return false;
    }

    if (!genarateKeyIv(m_key, m_iv))
    {
        return false;
    }

    AESEncryption aesEnc(reinterpret_cast<byte *>(m_key.data()), AES::DEFAULT_KEYLENGTH);
    CBC_Mode<AES>::Encryption *pcbcEnc = NULL;
    CFB_Mode<AES>::Encryption *pcfbEnc = NULL;
    //ECB_Mode<AES>::Encryption *pecbEnc = NULL;
    ECB_Mode_ExternalCipher::Encryption *pecbEnc = NULL;
    OFB_Mode<AES>::Encryption *pofbEnc = NULL;
    FileSource *pFs = NULL;
    //StreamTransformationFilter *pstfEnc = NULL;
    switch (mode)
    {
    case CBC:
        pcbcEnc = new CBC_Mode<AES>::Encryption(reinterpret_cast<byte *>(m_key.data()),
                                                AES::DEFAULT_KEYLENGTH, reinterpret_cast<byte *>(m_iv.data()));

        if (NULL != pcbcEnc)
        {
            pFs = new FileSource(inFile, true, new StreamTransformationFilter(*pcbcEnc, new FileSink(outFile)));
            if (pFs)
            {
                key = m_key;
                iv = m_iv;
                isSucc = true;
            }
        }
        /*if (pcbcEnc)
        	pstfEnc = new StreamTransformationFilter(*pcbcEnc, new FileSink(outFile.c_str().c_str()));*/
        break;

    case /*E_MODE::*/CFB:
        pcfbEnc = new CFB_Mode<AES>::Encryption(reinterpret_cast<byte *>(m_key.data()),
                                                AES::DEFAULT_KEYLENGTH, reinterpret_cast<byte *>(m_iv.data()));
        if (NULL != pcfbEnc)
        {
            pFs = new FileSource(inFile, true,
                                 new StreamTransformationFilter(*pcfbEnc, new FileSink(outFile)));
            if (pFs)
            {
                key = m_key;
                iv = m_iv;
                isSucc = true;
            }
        }
        break;

    case /*E_MODE::*/ECB:
        /*pecbEnc = new ECB_Mode<AES>::Encryption(reinterpret_cast<byte *>(m_key.data()),
        			AES::DEFAULT_KEYLENGTH, reinterpret_cast<byte *>(m_iv.data()));*/
        pecbEnc = new ECB_Mode_ExternalCipher::Encryption(aesEnc, reinterpret_cast<byte *>(m_iv.data()));
        if (NULL != pecbEnc)
        {
            pFs = new FileSource(inFile, true,
                                 new StreamTransformationFilter(*pecbEnc, new FileSink(outFile)));
            if (pFs)
            {
                key = m_key;
                iv = m_iv;
                isSucc = true;
            }
        }
        break;

    case /*E_MODE::*/OFB:
        pofbEnc = new OFB_Mode<AES>::Encryption(reinterpret_cast<byte *>(m_key.data()),
                                                AES::DEFAULT_KEYLENGTH, reinterpret_cast<byte *>(m_iv.data()));
        if (NULL != pofbEnc)
        {
            pFs = new FileSource(inFile, true,
                                 new StreamTransformationFilter(*pofbEnc, new FileSink(outFile)));
            if (pFs)
            {
                key = m_key;
                iv = m_iv;
                isSucc = true;
            }
        }
        break;

    default:
        isSucc = false;
        break;
    }
    /*if (pstfEnc) {
    	pstfEnc->Put(reinterpret_cast<const unsigned char*>(raw.data()), raw.size());
    	pstfEnc->MessageEnd();
    	isSucc = true;
    }*/
    /*if (outFile) {
    	free(outFile);
    	outFile = NULL;
    }
    if (inFile) {
    	free(inFile);
    	inFile = NULL;
    }*/
    if (pFs)
    {
        delete pFs;
        pFs = NULL;
    }
    if (pcbcEnc)
    {
        delete pcbcEnc;
        pcbcEnc = NULL;
    }
    if (pcfbEnc)
    {
        delete pcfbEnc;
        pcfbEnc = NULL;
    }
    if (pecbEnc)
    {
        delete pecbEnc;
        pecbEnc = NULL;
    }
    if (pofbEnc)
    {
        delete pofbEnc;
        pofbEnc = NULL;
    }

    return isSucc;
}

bool CryptInfo::decryptFile( const char* inFile, const QByteArray& key, const QByteArray& iv, const char* outFile, E_MODE mode /*= E_MODE::CBC*/ )
{
    bool isSucc = false;

    //if (-1 == access(inFile, 0))
    if (-1 == _access(inFile, 0))
    {
        //qDebug() << "The file: " << strInFilename << "is not exist!";
        return false;
    }

    CBC_Mode<AES>::Decryption *pcbcDec = NULL;
    CFB_Mode<AES>::Decryption *pcfbDec = NULL;
    AESDecryption aesDec(reinterpret_cast<byte *>(const_cast<char *>(key.data())), AES::DEFAULT_KEYLENGTH);
    ECB_Mode_ExternalCipher::Decryption *pecbDec = NULL;
    OFB_Mode<AES>::Decryption *pofbDec = NULL;
    FileSource *pFs = NULL;
    switch (mode)
    {
    case /*E_MODE::*/CBC:
        pcbcDec = new CBC_Mode<AES>::Decryption(reinterpret_cast<byte *>(const_cast<char *>(key.data())),
                                                AES::DEFAULT_KEYLENGTH, reinterpret_cast<byte *>(const_cast<char *>(iv.data())));

        if (NULL != pcbcDec)
        {
            pFs = new FileSource(inFile, true, new StreamTransformationFilter(*pcbcDec, new FileSink(outFile)));
            if (pFs)
                isSucc = true;
        }
        break;

    case /*E_MODE::*/CFB:
        pcfbDec = new CFB_Mode<AES>::Decryption(reinterpret_cast<byte *>(const_cast<char *>(key.data())),
                                                AES::DEFAULT_KEYLENGTH, reinterpret_cast<byte *>(const_cast<char *>(iv.data())));
        if (NULL != pcfbDec)
        {
            pFs = new FileSource(inFile, true,
                                 new StreamTransformationFilter(*pcfbDec, new FileSink(outFile)));
            if (pFs)
                isSucc = true;
        }
        break;

    case /*E_MODE::*/ECB:
        pecbDec = new ECB_Mode_ExternalCipher::Decryption(aesDec, reinterpret_cast<byte *>(const_cast<char *>(key.data())));
        if (NULL != pecbDec)
        {
            pFs = new FileSource(inFile, true,
                                 new StreamTransformationFilter(*pecbDec, new FileSink(outFile)));
            if (pFs)
                isSucc = true;
        }
        break;

    case /*E_MODE::*/OFB:
        pofbDec = new OFB_Mode<AES>::Decryption(reinterpret_cast<byte *>(const_cast<char *>(key.data())),
                                                AES::DEFAULT_KEYLENGTH, reinterpret_cast<byte *>( const_cast<char *>(iv.data()) ) );
        if (NULL != pofbDec)
        {
            pFs = new FileSource(inFile, true,
                                 new StreamTransformationFilter(*pofbDec, new FileSink(outFile)));
            if (pFs)
                isSucc = true;
        }
        break;

    default:
        isSucc = false;
        break;
    }
    if (pFs)
    {
        delete pFs;
        pFs = NULL;
    }
    if (pcbcDec)
    {
        delete pcbcDec;
        pcbcDec = NULL;
    }
    if (pcfbDec)
    {
        delete pcfbDec;
        pcfbDec = NULL;
    }
    if (pecbDec)
    {
        delete pecbDec;
        pecbDec = NULL;
    }
    if (pofbDec)
    {
        delete pofbDec;
        pofbDec = NULL;
    }

    return isSucc;
}