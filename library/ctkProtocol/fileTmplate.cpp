#include "fileTmplate.h"
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
void serialize(Archive& ar, FileTmplate &set, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(set.vecType);
    ar & BOOST_SERIALIZATION_NVP(set.vecReg);
    ar & BOOST_SERIALIZATION_NVP(set.vecIndex);
}

}
}

bool FileTmplateToXml(const string &strPath, const FileTmplate &set)
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

bool XmlToFileTmplate(const string &strPath, FileTmplate &set)
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
