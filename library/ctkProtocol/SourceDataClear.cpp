#include "SourceDataClear.h"

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
        void serialize(Archive& ar, CSourceDataClearSet &obj, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(obj.m_data);
        }

        template<class Archive>
        void serialize(Archive& ar, CSourceDataClear &obj, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(obj.strGuid);
            ar & BOOST_SERIALIZATION_NVP(obj.strClearDir);
            ar & BOOST_SERIALIZATION_NVP(obj.bUsed);
            ar & BOOST_SERIALIZATION_NVP(obj.nClearDay);

        }
    }
}

bool CSourceDataClearSet2Xml(const std::string &path, const CSourceDataClearSet &in_obj)
{
    try
    {
        std::ofstream ofs(path.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(in_obj);
        ofs.flush();
    }
    catch (std::exception &e)
    {
        std::cout << e.what();
        return false;
    }
    return true;
}

bool Xml2CSourceDataClearSet(const std::string &path, CSourceDataClearSet &in_obj)
{
    try
    {
        std::ifstream ifs(path.c_str());
        if (!ifs.good())
        {
            return false;
        }
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(in_obj);
    }
    catch (std::exception &e)
    {
        std::cout << e.what();
        return false;
    }
    return true;
}


