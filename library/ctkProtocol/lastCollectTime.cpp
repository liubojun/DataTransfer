#include "lastCollectTime.h"
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
#include <iostream>
namespace boost
{
    namespace serialization
    {

        template<class Archive>
        void serialize(Archive &ar, CollectTime &set, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(set.dir_time);
        }

        template<class Archive>
        void serialize(Archive &ar, CollectTimeList &set, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(set.id_last);
        }

        template<class Archive>
        void serialize(Archive &ar, LatestFileList &set, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(set.m_files);
        }

    }
}

bool CollectTimeToXml(const std::string &strPath, const CollectTimeList &lst)
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

bool XmlToCollectTime(const std::string &strPath, CollectTimeList &lst)
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

bool LatestFileListToXml(const std::string &strPath, const LatestFileList &lst)
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

bool XmlToLatestFileList(const std::string &strPath, LatestFileList &lst)
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
