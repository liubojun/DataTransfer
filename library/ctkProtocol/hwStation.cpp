#include "hwStation.h"
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


bool HwStationToXml(const std::string &strPath, const HWstationMap &info)
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
        cout << e.what();
        return false;
    }

    return true;
}

bool XmlToHwStation(const std::string &strPath, HWstationMap &info)
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
        cout << e.what();
        return false;
    }

    return true;
}

namespace boost
{
namespace serialization
{
template<class Archive>
void serialize(Archive& ar, HWstation &set, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(set.strId);
    ar & BOOST_SERIALIZATION_NVP(set.strName);
    ar & BOOST_SERIALIZATION_NVP(set.strLat);
    ar & BOOST_SERIALIZATION_NVP(set.strLon);
    ar & BOOST_SERIALIZATION_NVP(set.strVgate);
    ar & BOOST_SERIALIZATION_NVP(set.strAreaGate);
    ar & BOOST_SERIALIZATION_NVP(set.strRange);
}

template<class Archive>
void serialize(Archive& ar, HWstationMap &set, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(set.mapIdSet);
}

}
}