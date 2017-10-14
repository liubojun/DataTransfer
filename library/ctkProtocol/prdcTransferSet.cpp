#include "prdcTransferSet.h"
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
void serialize(Archive& ar, PrdcTransferSet &fi, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(fi.strID);
    ar & BOOST_SERIALIZATION_NVP(fi.bRadarData);
    ar & BOOST_SERIALIZATION_NVP(fi.bRadarPic);
    ar & BOOST_SERIALIZATION_NVP(fi.bMosaicData);
    ar & BOOST_SERIALIZATION_NVP(fi.bMosaicPic);
    ar & BOOST_SERIALIZATION_NVP(fi.bPupData);
    ar & BOOST_SERIALIZATION_NVP(fi.bPupPic);
    ar & BOOST_SERIALIZATION_NVP(fi.bHighData);
    ar & BOOST_SERIALIZATION_NVP(fi.bHighPic);
    ar & BOOST_SERIALIZATION_NVP(fi.bEnable);
    ar & BOOST_SERIALIZATION_NVP(fi.vecRecvID);
}

template<class Archive>
void serialize(Archive& ar, PrdcTransferSetList &fi, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(fi.lst);
}
}
}


bool PrdcTransferSetToXml(const string &strPath, const PrdcTransferSetList &info)
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

bool XmlToPrdcTransferSet(const string &strPath, PrdcTransferSetList &info)
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
