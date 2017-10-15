#include "lateFileTime.h"
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

using namespace std;

namespace boost
{
    namespace serialization
    {
        template<class Archive>
        void serialize(Archive& ar, StationTypeTime &fi, const unsigned int version)
        {
            // ar & BOOST_SERIALIZATION_NVP(fi.strStation);
            ar & BOOST_SERIALIZATION_NVP(fi.mapTypeTime);
        }

        template<class Archive>
        void serialize(Archive& ar, StationTypeTimeList &fi, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(fi.lsInfos);
        }
    }
}

DLL_EXPORT_C_DECL bool StationTimeToXml(const std::string &strPath, const StationTypeTimeList &info)
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

DLL_EXPORT_C_DECL bool XmlToStationTime(const std::string &strPath, StationTypeTimeList &info)
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
