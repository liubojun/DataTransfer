#include "mosaicArea.h"
#include "boost/serialization/serialization.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/map.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include "boost/archive/xml_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/nvp.hpp"

namespace boost
{
namespace serialization
{
template<class Archive>
void serialize(Archive& ar, CSStation &pC, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(pC.m_sStationNo);
    ar & BOOST_SERIALIZATION_NVP(pC.m_sStationName);
    ar & BOOST_SERIALIZATION_NVP(pC.m_fLon);
    ar & BOOST_SERIALIZATION_NVP(pC.m_fLat);
    ar & BOOST_SERIALIZATION_NVP(pC.m_elevation);
    //ar & BOOST_SERIALIZATION_NVP(pC.m_sRadarType);
    ar & BOOST_SERIALIZATION_NVP(pC.m_fRange);
    ar & BOOST_SERIALIZATION_NVP(pC.m_sBasePath);
}

template<class Archive>
void serialize(Archive &ar, RadarMosaicArea &set, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(set.sID);
    ar & BOOST_SERIALIZATION_NVP(set.sAreaName);
    ar & BOOST_SERIALIZATION_NVP(set.bUse);
    ar & BOOST_SERIALIZATION_NVP(set.stationArray);
}

template<class Archive>
void serialize(Archive &ar, RadarMosaicAreaList &lst, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(lst.lst);
}

template<class Archive>
void serialize(Archive &ar, monitor_ &m_, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(m_.radarnum);
    ar & BOOST_SERIALIZATION_NVP(m_.radarname);
    ar & BOOST_SERIALIZATION_NVP(m_.radarip);
}

template<class Archive>
void serialize(Archive &ar, monitor_s &m_s, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(m_s.ArrayMonitors);
}

template<class Archive>
void serialize(Archive &ar, welkin &m_, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(m_.m_bType);
    ar & BOOST_SERIALIZATION_NVP(m_.m_picHeight);
    ar & BOOST_SERIALIZATION_NVP(m_.m_picWidth);
}

template<class Archive>
void serialize(Archive &ar, welkin_s &m_s, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(m_s.m_tlogP);
    ar & BOOST_SERIALIZATION_NVP(m_s.m_windrosepic);
    ar & BOOST_SERIALIZATION_NVP(m_s.m_bShowPhy);
}

template<class Archive>
void serialize(Archive &ar, mosaicQueue &m_q, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(m_q.m_groupBoxName);
    ar & BOOST_SERIALIZATION_NVP(m_q.m_messageQueue);
    ar & BOOST_SERIALIZATION_NVP(m_q.m_archiveQueue);
    ar & BOOST_SERIALIZATION_NVP(m_q.m_broadcastQueue);
}
}
}

DLL_EXPORT_DECL bool RadarMosaicAreaToXml(const string &strPath, const RadarMosaicAreaList &lst)
{
    try
    {
        ofstream ofs(strPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(lst);
        ofs.flush();
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }

    return true;
}

DLL_EXPORT_DECL bool XmlToRadarMosaicArea(const string &strPath, RadarMosaicAreaList &lst)
{
    try
    {
        ifstream ifs(strPath.c_str());
        if (!ifs.good())
        {
            return false;
        }
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(lst);
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }

    return true;
}

DLL_EXPORT_DECL bool RadarMonitorToXml(const string &strPath, const monitor_s &lst)
{
    try
    {
        ofstream ofs(strPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(lst);
        ofs.flush();
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }

    return true;
}

DLL_EXPORT_DECL bool XmlToRadarMonitor(const string &strPath, monitor_s &lst)
{
    try
    {
        ifstream ifs(strPath.c_str());
        if (!ifs.good())
        {
            return false;
        }
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(lst);
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }

    return true;
}

DLL_EXPORT_DECL bool WelkinToXml(const string &strPath, const welkin_s &lst)
{
    try
    {
        ofstream ofs(strPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(lst);
        ofs.flush();
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }

    return true;
}

DLL_EXPORT_DECL bool XmlToWelkin(const string &strPath, welkin_s &lst)
{
    try
    {
        ifstream ifs(strPath.c_str());
        if (!ifs.good())
        {
            return false;
        }
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(lst);
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }

    return true;
}


DLL_EXPORT_DECL bool MosaicQueueToXml(const string &strPath, const mosaicQueue &lst)
{
    try
    {
        ofstream ofs(strPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(lst);
        ofs.flush();
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }

    return true;
}

DLL_EXPORT_DECL bool XmlToMosaicQueue(const string &strPath, mosaicQueue &lst)
{
    try
    {
        ifstream ifs(strPath.c_str());
        if (!ifs.good())
        {
            return false;
        }
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(lst);
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }

    return true;
}