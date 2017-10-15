#include "transferSet.h"
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/map.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include "boost/archive/xml_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/nvp.hpp"

#include <iostream>
using namespace std;

bool TransferSetToXml(const string &strPath, const TransferSetList &info)
{
    try
    {
        ofstream ofs(strPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(info);
        ofs.flush();
    }
    catch (std::exception &e)
    {
        std::cout << e.what();
        return false;
    }

    return true;
}

bool XmlToTransferSet(const string &strPath, TransferSetList &info)
{
    try
    {
        ifstream ifs(strPath.c_str());
        if (!ifs.good())
        {
            return false;
        }
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(info);
    }
    catch (std::exception &e)
    {
        std::cout << e.what();
        return false;
    }

    return true;
}

namespace boost
{
    namespace serialization
    {
        template<class Archive>
        void serialize(Archive& ar, SendSet &fi, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(fi.strUsrname);
            ar & BOOST_SERIALIZATION_NVP(fi.strPath);
            ar & BOOST_SERIALIZATION_NVP(fi.vecRegs);
            ar & BOOST_SERIALIZATION_NVP(fi.strRltvPath);
            ar & BOOST_SERIALIZATION_NVP(fi.bEncrypt);
            ar & BOOST_SERIALIZATION_NVP(fi.bCompress);
            ar & BOOST_SERIALIZATION_NVP(fi.bKeepDir);
            ar & BOOST_SERIALIZATION_NVP(fi.nSendWay);
            ar & BOOST_SERIALIZATION_NVP(fi.bConSend);
            ar & BOOST_SERIALIZATION_NVP(fi.strUsr);
            ar & BOOST_SERIALIZATION_NVP(fi.strPwd);
            ar & BOOST_SERIALIZATION_NVP(fi.nPort);
            ar & BOOST_SERIALIZATION_NVP(fi.strIP);
        }

        template<class Archive>
        void serialize(Archive& ar, SendSetList &fi, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(fi.lsts);
        }

        template<class Archive>
        void serialize(Archive& ar, TransferSet &fi, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(fi.strName);
            ar & BOOST_SERIALIZATION_NVP(fi.strPath);
            ar & BOOST_SERIALIZATION_NVP(fi.nCollectWay);
            ar & BOOST_SERIALIZATION_NVP(fi.vecRegs);
            ar & BOOST_SERIALIZATION_NVP(fi.nWatchWay);
            ar & BOOST_SERIALIZATION_NVP(fi.strDispatch);
            ar & BOOST_SERIALIZATION_NVP(fi.strUsr);
            ar & BOOST_SERIALIZATION_NVP(fi.strPwd);
            ar & BOOST_SERIALIZATION_NVP(fi.nPort);
            ar & BOOST_SERIALIZATION_NVP(fi.strIP);
            //ar & BOOST_SERIALIZATION_NVP(fi.vecRcvers);	//≤ª–Ú¡–ªØ
            ar & BOOST_SERIALIZATION_NVP(fi.vecRecvID);
            ar & BOOST_SERIALIZATION_NVP(fi.bEnable);
        }

        template<class Archive>
        void serialize(Archive& ar, TransferSetList &fi, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(fi.lsSets);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
namespace boost
{
    namespace serialization
    {
        template<class Archive>
        void serialize(Archive& ar, TransCollectTime &fi, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(fi.strDirID);
            ar & BOOST_SERIALIZATION_NVP(fi.mapDirTime);
        }

        template<class Archive>
        void serialize(Archive& ar, TransCollectTimeList &fi, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(fi.lsts);
        }
    }
}

DLL_EXPORT_C_DECL bool TransCollectTimeToXml(const std::string &strPath, const TransCollectTimeList &lst)
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
        std::cout << e.what();
        return false;
    }
    return true;
}

DLL_EXPORT_C_DECL bool XmlToTransCollectTime(const std::string &strPath, TransCollectTimeList &lst)
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
        std::cout << e.what();
        return false;
    }
    return true;
}

DLL_EXPORT_C_DECL bool SendSetListToXml(const string &strPath, const SendSetList &lst)
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
        std::cout << e.what();
        return false;
    }
    return true;
}

DLL_EXPORT_C_DECL bool XmlToSendSetList(const string &strPath, SendSetList &lst)
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
        std::cout << e.what();
        return false;
    }
    return true;
}
