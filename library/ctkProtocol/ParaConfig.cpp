#include "ParaConfig.h"
#include <sstream>
#include <fstream>
#include <algorithm>


#include "boost/serialization/serialization.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include "boost/archive/xml_iarchive.hpp"
#include <boost/serialization/nvp.hpp>
#include "boost/serialization/map.hpp"
#include "boost/serialization/vector.hpp"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CSStation::CSStation()
    : m_sStationNo("")
    , m_sStationName("")
    , m_fLon(0.0f)
    , m_fLat(0.0f)
    , m_elevation(0.0f)
    , m_sRadarType("SA/SB")
    , m_fRange(0.0f)
    , m_sBasePath("") {}

CSStation::CSStation(std::string sStationNo, std::string sStationName, const double fLon,
                     const double fLat, const double elevation, std::string radarType, const double fRange, std::string sBasePath)
{
    m_sStationNo	= sStationNo;
    m_sStationName	= sStationName;
    m_fLon			= fLon;
    m_fLat			= fLat;
    m_elevation		= elevation;
    m_sRadarType	= radarType;
    m_fRange		= fRange;
    m_sBasePath		= sBasePath;
}

CSStation::CSStation(const CSStation& cSS)
{
    m_sStationNo	= cSS.m_sStationNo;
    m_sStationName	= cSS.m_sStationName;
    m_fLon			= cSS.m_fLon;
    m_fLat			= cSS.m_fLat;
    m_elevation		= cSS.m_elevation;
    m_sRadarType	= cSS.m_sRadarType;
    m_fRange		= cSS.m_fRange;
    m_sBasePath		= cSS.m_sBasePath;
}

CSStation& CSStation::operator=(const CSStation& cSS)
{
    if ( this != &cSS)
    {
        m_sStationNo	= cSS.m_sStationNo;
        m_sStationName	= cSS.m_sStationName;
        m_fLon			= cSS.m_fLon;
        m_fLat			= cSS.m_fLat;
        m_elevation		= cSS.m_elevation;
        m_sRadarType	= cSS.m_sRadarType;
        m_fRange		= cSS.m_fRange;
        m_sBasePath		= cSS.m_sBasePath;
    }

    return *this;
}

CSStation::~CSStation() {}

void CSStation::SetParam(std::string sStationNo, std::string sStationName, const double fLon,
                         const double fLat, const double elevation, std::string radarType, const double fRange, std::string sBasePath)
{
    m_sStationNo	= sStationNo;
    m_sStationName	= sStationName;
    m_fLon			= fLon;
    m_fLat			= fLat;
    m_elevation		= elevation;
    m_sRadarType	= radarType;
    m_fRange		= fRange;
    m_sBasePath		= sBasePath;
}

bool SerializeSStation2XmlFile(std::string& sXmlPath, const CSStation& pC)
{
    bool bFlag = true;

    try
    {
        std::ofstream ofs(sXmlPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(pC);
        ofs.flush();
    }
    catch(std::exception& e)
    {
        sXmlPath.clear();
        std::string s(e.what(), strlen(e.what()));

        bFlag = false;
    }

    return bFlag;
}

bool DeserializeXmlFile2SStation(const std::string& sXmlPath, CSStation& pC)
{
    bool bFlag = true;

    try
    {
        std::ifstream ifs(sXmlPath.c_str());
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(pC);
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bFlag = false;
    }

    return bFlag;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


StationConfig::StationConfig()
{
    m_ArrayRadar.clear();
}

StationConfig::StationConfig(const StationConfig& sC)
{
    m_ArrayRadar = sC.m_ArrayRadar;
}

StationConfig& StationConfig::operator=(const StationConfig& sC)
{
    if ( this != &sC )
        m_ArrayRadar = sC.m_ArrayRadar;

    return *this;
}

StationConfig::~StationConfig()
{

}

bool SerializeStationConfig2XmlFile(std::string& sXmlPath, const StationConfig& pC)
{
    bool bFlag = true;

    try
    {
        std::ofstream ofs(sXmlPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(pC);
        ofs.flush();
    }
    catch(std::exception& e)
    {
        sXmlPath.clear();
        std::string s(e.what(), strlen(e.what()));

        bFlag = false;
    }

    return bFlag;
}

bool DeserializeXmlFile2StationConfig(const std::string& sXmlPath, StationConfig& pC)
{
    bool bFlag = true;

    try
    {
        std::ifstream ifs(sXmlPath.c_str());
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(pC);
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bFlag = false;
    }

    return bFlag;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

StationS::StationS()
{
    m_arrayStations.clear();
}

StationS::StationS(const StationS& sC)
{
    m_arrayStations = sC.m_arrayStations;
}

StationS& StationS::operator=(const StationS& sC)
{
    if ( this != &sC )
        m_arrayStations = sC.m_arrayStations;

    return *this;
}

StationS::~StationS() {}

bool SerializeStationS2XmlFile(const std::string& sXmlPath, const StationS& pC)
{
    bool bFlag = true;

    try
    {
        std::ofstream ofs(sXmlPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(pC);
        ofs.flush();
    }
    catch(std::exception& e)
    {
        //sXmlPath.clear();
        std::string s(e.what(), strlen(e.what()));

        bFlag = false;
    }

    return bFlag;
}

bool DeserializeXmlFile2StationS(const std::string& sXmlPath, StationS& pC)
{
    bool bFlag = true;

    try
    {
        std::ifstream ifs(sXmlPath.c_str());
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(pC);
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bFlag = false;
    }

    return bFlag;
}




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ParaConfig::ParaConfig()
    : m_dir("")
    , m_matchTime(10)
    , m_timeGap(20)
    , m_timeDelay(30)
    , m_raial(0.01f)
    , m_zonal(0.01f) {}

ParaConfig::ParaConfig(std::string dir_, const unsigned int matchTime_, const unsigned int timeGap_,
                       const unsigned int timeDelay_, const double raial_, const double zonal_)
{
    m_dir		= dir_;
    m_matchTime = matchTime_;
    m_timeGap	= timeGap_;
    m_timeDelay	= timeDelay_;
    m_raial		= raial_;
    m_zonal		= zonal_;
}

ParaConfig::ParaConfig(const ParaConfig& pC)
{
    m_dir		= pC.m_dir;
    m_matchTime = pC.m_matchTime;
    m_timeGap	= pC.m_timeGap;
    m_timeDelay = pC.m_timeDelay;
    m_raial		= pC.m_raial;
    m_zonal		= pC.m_zonal;
    m_highlayers= pC.m_highlayers;
}

ParaConfig& ParaConfig::operator=(const ParaConfig& pC)
{
    if( this == &pC )
        return *this;

    m_dir		= pC.m_dir;
    m_matchTime = pC.m_matchTime;
    m_timeGap	= pC.m_timeGap;
    m_timeDelay	= pC.m_timeDelay;
    m_raial		= pC.m_raial;
    m_zonal		= pC.m_zonal;
    m_highlayers= pC.m_highlayers;

    return *this;
}

ParaConfig::~ParaConfig() {}

void ParaConfig::SetParam(std::string dir_, const unsigned int matchTime_, const unsigned int timeGap_,
                          const unsigned int timeDelay_, const double raial_, const double zonal_)
{
    m_dir		= dir_;
    m_matchTime = matchTime_;
    m_timeGap	= timeGap_;
    m_timeDelay	= timeDelay_;
    m_raial		= raial_;
    m_zonal		= zonal_;
}

bool SerializePara2XmlFile(std::string& sXmlPath, const ParaConfig& pC)
{
    bool bFlag = true;

    try
    {
        std::ofstream ofs(sXmlPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(pC);
        ofs.flush();
    }
    catch(std::exception& e)
    {
        sXmlPath.clear();
        std::string s(e.what(), strlen(e.what()));

        bFlag = false;
    }

    return bFlag;
}

bool DeserializeXmlFile2Para(const std::string& sXmlPath, ParaConfig& pC)
{
    bool bFlag = true;

    try
    {
        std::ifstream ifs(sXmlPath.c_str());
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(pC);
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bFlag = false;
    }

    return bFlag;
}


namespace boost
{
namespace serialization
{
template<class Archive>
void serialize(Archive& ar, ParaConfig &pC, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(pC.m_dir);
    ar & BOOST_SERIALIZATION_NVP(pC.m_matchTime);
    ar & BOOST_SERIALIZATION_NVP(pC.m_timeGap);
    ar & BOOST_SERIALIZATION_NVP(pC.m_timeDelay);
    ar & BOOST_SERIALIZATION_NVP(pC.m_raial);
    ar & BOOST_SERIALIZATION_NVP(pC.m_zonal);
    ar & BOOST_SERIALIZATION_NVP(pC.m_highlayers);
}

template<class Archive>
void serialize(Archive& ar, CSStation &pC, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(pC.m_sStationNo);
    ar & BOOST_SERIALIZATION_NVP(pC.m_sStationName);
    ar & BOOST_SERIALIZATION_NVP(pC.m_fLon);
    ar & BOOST_SERIALIZATION_NVP(pC.m_fLat);
    ar & BOOST_SERIALIZATION_NVP(pC.m_elevation);
    ar & BOOST_SERIALIZATION_NVP(pC.m_sRadarType);
    ar & BOOST_SERIALIZATION_NVP(pC.m_fRange);
    ar & BOOST_SERIALIZATION_NVP(pC.m_sBasePath);
}

template<class Archive>
void serialize(Archive& ar, StationConfig &pC, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(pC.m_ArrayRadar);
}

template<class Archive>
void serialize(Archive& ar, StationS &pC, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(pC.m_arrayStations);
}
}
}