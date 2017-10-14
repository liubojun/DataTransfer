#include "dbSet.h"
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
void serialize(Archive& ar, DbSet &set, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(set.strIP);
    ar & BOOST_SERIALIZATION_NVP(set.nPort);
    ar & BOOST_SERIALIZATION_NVP(set.strUser);
    ar & BOOST_SERIALIZATION_NVP(set.strPwd);
    ar & BOOST_SERIALIZATION_NVP(set.strDb);
}

}
}

bool DbSetToXml(const string &strPath, const DbSet &set)
{
    try
    {
        ofstream ofs(strPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(set);
        ofs.flush();
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }

    return true;
}

bool XmlToDbSet(const string &strPath, DbSet &set)
{
    try
    {
        ifstream ifs(strPath.c_str());
        if (!ifs.good())
        {
            return false;
        }
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(set);
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }

    return true;
}
