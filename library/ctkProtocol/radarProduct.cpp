#include "radarProduct.h"
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
void serialize(Archive &ar, RadarProductSet &set, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(set.nID);
    ar & BOOST_SERIALIZATION_NVP(set.strChName);
    ar & BOOST_SERIALIZATION_NVP(set.strEnName);
    ar & BOOST_SERIALIZATION_NVP(set.bData);
    ar & BOOST_SERIALIZATION_NVP(set.bPicture);
    ar & BOOST_SERIALIZATION_NVP(set.nType);
}

template<class Archive>
void serialize(Archive &ar, RadarProductList &lst, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(lst.lst);
}
}
}

DLL_EXPORT_DECL bool RadarProductToXml(const string &strPath, const RadarProductList &lst)
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

DLL_EXPORT_DECL bool XmlToRadarProduct(const string &strPath, RadarProductList &lst)
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
