#include "pupmosaicProduct.h"
#include <fstream>
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

bool PupMosaicProductSetToXml(const std::string &strPath, const CPupMosaicProductSettings &set)
{
    try
    {
        std::ofstream ofs(strPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(set);
        ofs.flush();
    }
    catch (std::exception &e)
    {
        std::cout << e.what();
        return false;
    }
    return true;
}

bool XmlToPupMosaicProductSet(const std::string &strPath, CPupMosaicProductSettings &set)
{
    try
    {
        std::ifstream ifs(strPath.c_str());
        if (!ifs.good())
        {
            return false;
        }
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(set);
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
        void serialize(Archive& ar, CPupMosaicProductSettings &fi, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(fi.m_data);
        }

        template<class Archive>
        void serialize(Archive& ar, CPupMosaicProductSetting &fi, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(fi.nProductId);
            ar & BOOST_SERIALIZATION_NVP(fi.strProductShortName);
            ar & BOOST_SERIALIZATION_NVP(fi.strProductName);
            ar & BOOST_SERIALIZATION_NVP(fi.bActiveData);
            ar & BOOST_SERIALIZATION_NVP(fi.bActiveImage);
        }
    }
}

