#include "collectSet.h"
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


bool CollectSetToXml(const string &strPath, const CollectSetList &setlst)
{
    try
    {
        ofstream ofs(strPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(setlst);
        ofs.flush();
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }

    return true;
}

bool XmlToCollectSet(const string &strPath, CollectSetList &setlst)
{
    try
    {
        ifstream ifs(strPath.c_str());
        if (!ifs.good())
        {
            return false;
        }
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(setlst);
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
void serialize(Archive& ar, CollectSet &set, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(set.strId);
    ar & BOOST_SERIALIZATION_NVP(set.strPath);
    ar & BOOST_SERIALIZATION_NVP(set.nWtype);
    ar & BOOST_SERIALIZATION_NVP(set.strDispatch);
    ar & BOOST_SERIALIZATION_NVP(set.mapTypeRegs);
    ar & BOOST_SERIALIZATION_NVP(set.vecQueNames);
    ar & BOOST_SERIALIZATION_NVP(set.strStation);
    ar & BOOST_SERIALIZATION_NVP(set.bEnable);
    //ar & BOOST_SERIALIZATION_NVP(set.strCapRegular);
    //ar & BOOST_SERIALIZATION_NVP(set.nStationCapIndex);
    //ar & BOOST_SERIALIZATION_NVP(set.nTimeCapIndex);
}

template<class Archive>
void serialize(Archive& ar, CollectSetList &set, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(set.lsSets);
}

template<class Archive>
void serialize(Archive& ar, CDataInfo &di, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(di.typeReg);
    ar & BOOST_SERIALIZATION_NVP(di.queNames);
}

template<class Archive>
void serialize(Archive& ar, NameMatchRule &di, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(di.strNameRegExp);
    ar & BOOST_SERIALIZATION_NVP(di.nTimeIndex);
    ar & BOOST_SERIALIZATION_NVP(di.nStationIndex);
}

}
}
