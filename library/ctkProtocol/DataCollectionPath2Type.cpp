#include "DataCollectionPath2Type.h"
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
#include <sstream>
#include <fstream>

namespace boost
{
    namespace serialization
    {
        template<class Archive>
        void serialize(Archive& ar, CDataCollPath2Types &obj, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(obj.m_data);
        }

        template<class Archive>
        void serialize(Archive& ar, CDataCollPath2Type &obj, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(obj.strName);
            ar & BOOST_SERIALIZATION_NVP(obj.nType);
            ar & BOOST_SERIALIZATION_NVP(obj.strClassName);
            ar & BOOST_SERIALIZATION_NVP(obj.strUrl);
        }
    }
}

bool DataCollPathToXml(const std::string &strPath, const CDataCollPath2Types &info)
{
    try
    {
        std::ofstream ofs(strPath.c_str());
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

bool XmlToDataCollPath(const std::string &strPath, CDataCollPath2Types &info)
{
    try
    {
        std::ifstream ifs(strPath.c_str());
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

