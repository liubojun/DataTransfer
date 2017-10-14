#include "msgqueue.h"

#include <sstream>
#include <fstream>
#include <algorithm>


#include "boost/serialization/serialization.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include "boost/archive/xml_iarchive.hpp"
#include <boost/serialization/nvp.hpp>
#include "boost/serialization/map.hpp"
#include "boost/serialization/vector.hpp"

CMsgQueueSingle::CMsgQueueSingle()
{

}

CMsgQueueSingle::~CMsgQueueSingle()
{

}

CMsgQueue::CMsgQueue()
{

}

CMsgQueue::~CMsgQueue()
{

}


CMsgQueueGroup::CMsgQueueGroup()
{

}

CMsgQueueGroup::~CMsgQueueGroup()
{

}

bool SerializeQueue2Xml(const CMsgQueueGroup& m, const std::string &xmlFile)
{
    bool bFlag = true;

    try
    {
        std::ofstream ofs(xmlFile.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(m);
        ofs.flush();
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));

        bFlag = false;
    }

    return bFlag;
}

bool DeserializeXml2Queue(const std::string &xmlFile, CMsgQueueGroup& m)
{
    bool bFlag = true;

    try
    {
        std::ifstream ifs(xmlFile.c_str());

        if (!ifs.good())
        {
            return false;
        }
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(m);
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));

        bFlag = false;
    }

    return bFlag;
}

namespace boost
{
    namespace serialization
    {
        template<class Archive>
        void serialize(Archive& ar, CMsgQueueSingle &mq, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(mq.m_strQueueName);
            ar & BOOST_SERIALIZATION_NVP(mq.m_bIsRt);
        }

        template<class Archive>
        void serialize(Archive& ar, CMsgQueue &mq, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(mq.m_queue);
        }

        template<class Archive>
        void serialize(Archive& ar, CMsgQueueGroup &mqgroup, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(mqgroup.m_queuedata);
        }
    }
}


