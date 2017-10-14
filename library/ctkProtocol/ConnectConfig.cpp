#include "ConnectConfig.h"
#include <sstream>
#include <fstream>
#include <algorithm>


#include "boost/serialization/serialization.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include "boost/archive/xml_iarchive.hpp"
#include <boost/serialization/nvp.hpp>


ClientConfig::ClientConfig()
    : m_nType( 0 )
    , m_sIP("localhost")
    , m_nPort( 9528 )
    , m_sProcess("IPCH")
    , m_sThread("INPROCH")
{

}

ClientConfig::ClientConfig(int nType, std::string& sIP, unsigned long nPort, const std::string& sProcess, const std::string& sThread)
    : m_nType( nType )
    , m_sIP( sIP )
    , m_nPort( nPort )
    , m_sProcess( sProcess )
    , m_sThread( sThread )
{

}

ClientConfig::ClientConfig(const ClientConfig& cC)
{
    m_nType = cC.m_nType;
    m_sIP   = cC.m_sIP;
    m_nPort = cC.m_nPort;

    m_sProcess = cC.m_sProcess;
    m_sThread  = cC.m_sThread;
}

ClientConfig& ClientConfig::operator=(const ClientConfig& cC)
{
    if( this == &cC )
        return *this;

    m_nType = cC.m_nType;
    m_sIP	= cC.m_sIP;
    m_nPort	= cC.m_nPort;

    m_sProcess = cC.m_sProcess;
    m_sThread  = cC.m_sThread;

    return *this;
}

ClientConfig::~ClientConfig()
{

}

void ClientConfig::SetConnParam(int nType, std::string& sIP, unsigned long nPort, const std::string& sProcess, const std::string& sThread)
{
    m_nType = nType;
    m_sIP	= sIP;
    m_nPort = nPort;

    m_sProcess = sProcess;
    m_sThread  = sThread;
}

bool SerializeClient2StdString(std::string& sCC, const ClientConfig& cC)
{
    bool bRet = true;

    try
    {
        std::ostringstream os;
        boost::archive::binary_oarchive oa(os);
        oa << cC;
        sCC = os.str();
    }
    catch(std::exception& e)
    {
        sCC.clear();
        std::string s(e.what(), strlen(e.what()));

        bRet = false;
    }

    return bRet;
}

bool DeserializeStdString2Client(const std::string& sCC, ClientConfig& cC)
{
    bool bRet = true;

    try
    {
        std::istringstream is(sCC);
        boost::archive::binary_iarchive ia(is);
        ia >> cC;
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool SerializeClient2XmlFile(const std::string& sXmlPath, const ClientConfig& cC)
{
    bool bRet = true;

    try
    {
        std::ofstream ofs(sXmlPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(cC);
        ofs.flush();
    }
    catch(std::exception& e)
    {
        //sXmlPath.clear();
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool DeserializeXmlFile2Client(const std::string& sXmlPath, ClientConfig& cC)
{
    bool bRet = true;

    try
    {
        std::ifstream ifs(sXmlPath.c_str());
        if (!ifs.good())
        {
            return false;
        }
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(cC);
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


ServerConfig::ServerConfig()
    : m_nType(0)
    , m_sIP("*")
    , m_nPort(9528)
    , m_sProcess("IPCH")
    , m_sThread("INPROCH")
{

}

ServerConfig::ServerConfig(int nType, const std::string& sIP, unsigned long nPort, const std::string& sProcess, const std::string& sThread)
    : m_nType(nType)
    , m_sIP(sIP)
    , m_nPort(nPort)
    , m_sProcess(sProcess)
    , m_sThread(sThread)
{

}

ServerConfig::ServerConfig(const ServerConfig& SC)
{
    m_nType = SC.m_nType;
    m_sIP	= SC.m_sIP;
    m_nPort	= SC.m_nPort;

    m_sProcess = SC.m_sProcess;
    m_sThread  = SC.m_sThread;
}

ServerConfig& ServerConfig::operator=(const ServerConfig& SC)
{
    if( this == &SC )
        return *this;

    m_nType = SC.m_nType;
    m_sIP	= SC.m_sIP;
    m_nPort	= SC.m_nPort;

    m_sProcess = SC.m_sProcess;
    m_sThread  = SC.m_sThread;

    return *this;
}

ServerConfig::~ServerConfig()
{

}

void ServerConfig::SetConnParam(int nType, const std::string& sIP, unsigned long nPort, const std::string& sProcess, const std::string& sThread)
{
    m_nType = nType;
    m_sIP	= sIP;
    m_nPort	= nPort;

    m_sProcess = sProcess;
    m_sThread  = sThread;
}

bool SerializeServer2StdString(std::string& sCC, const ServerConfig& sC)
{
    bool bRet = true;

    try
    {
        std::ostringstream os;
        boost::archive::binary_oarchive oa(os);
        oa << sC;
        sCC = os.str();
    }
    catch(std::exception& e)
    {
        sCC.clear();
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool DeserializeStdString2Server(const std::string& sCC, ServerConfig& sC)
{
    bool bRet = true;

    try
    {
        std::istringstream is(sCC);
        boost::archive::binary_iarchive ia(is);
        ia >> sC;
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool SerializeServer2XmlFile(std::string& sXmlPath, const ServerConfig& sC)
{
    bool bRet = true;

    try
    {
        std::ofstream ofs(sXmlPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(sC);
        ofs.flush();
    }
    catch(std::exception& e)
    {
        sXmlPath.clear();
        std::string s(e.what(), strlen(e.what()));

        bRet = false;
    }

    return bRet;
}

bool DeserializeXmlFile2Server(const std::string& sXmlPath,  ServerConfig& sC)
{
    bool bRet = true;

    try
    {
        std::ifstream ifs(sXmlPath.c_str());
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(sC);
    }
    catch(std::exception& e)
    {
        bRet = false;
        std::string s(e.what(), strlen(e.what()));
    }

    return bRet;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

namespace boost
{
namespace serialization
{
template<class Archive>
void serialize(Archive& ar, ClientConfig &cC, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(cC.m_nType);
    ar & BOOST_SERIALIZATION_NVP(cC.m_sIP);
    ar & BOOST_SERIALIZATION_NVP(cC.m_nPort);
    ar & BOOST_SERIALIZATION_NVP(cC.m_sProcess);
    ar & BOOST_SERIALIZATION_NVP(cC.m_sThread);
}

template<class Archive>
void serialize(Archive& ar, ServerConfig &sC, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(sC.m_nType);
    ar & BOOST_SERIALIZATION_NVP(sC.m_sIP);
    ar & BOOST_SERIALIZATION_NVP(sC.m_nPort);
    ar & BOOST_SERIALIZATION_NVP(sC.m_sProcess);
    ar & BOOST_SERIALIZATION_NVP(sC.m_sThread);
}
}
}