#include "messageQueue.h"
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
void serialize(Archive& ar, PupMosaicQueue &set, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(set.queueName);
    ar & BOOST_SERIALIZATION_NVP(set.productId);
    ar & BOOST_SERIALIZATION_NVP(set.productEnName);
    ar & BOOST_SERIALIZATION_NVP(set.productZhName);
    ar & BOOST_SERIALIZATION_NVP(set.colorName);
}

template<class Archive>
void serialize(Archive& ar, MsgQueue &set, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(set.groupName);
    //ar & BOOST_SERIALIZATION_NVP(set.mapQueue);
    ar & BOOST_SERIALIZATION_NVP(set.pupQueue);
}

}
}

bool MsgQueueToXml(const string &strPath, const MsgQueue &set)
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

bool XmlToMsgQueue(const string &strPath, MsgQueue &set)
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
