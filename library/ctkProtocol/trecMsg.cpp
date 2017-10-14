#include "boost/serialization/serialization.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/map.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include "boost/archive/xml_iarchive.hpp"
#include "boost/serialization/nvp.hpp"
#include "trecMsg.h"

namespace boost
{
namespace serialization
{
template<class Archive>
void serialize(Archive& ar, TrecInfo &fi, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(fi.strType);
    ar & BOOST_SERIALIZATION_NVP(fi.strProduct);
    ar & BOOST_SERIALIZATION_NVP(fi.strStation);
    ar & BOOST_SERIALIZATION_NVP(fi.strFilePath);
    ar & BOOST_SERIALIZATION_NVP(fi.nTrecNum);
    ar & BOOST_SERIALIZATION_NVP(fi.vecTrecTime);
    ar & BOOST_SERIALIZATION_NVP(fi.vecTrecWeight);
}
}
}

bool TrecInfoToString(std::string &strMsg, const TrecInfo &info)
{
    try
    {
        std::ostringstream os;
        boost::archive::xml_oarchive oa(os);	//modify by weikangc 2016-06-02. 序列化成XML可见字符串
        oa << BOOST_SERIALIZATION_NVP(info);		//序列化到一个ostringstream里面
        strMsg = os.str();	//保存序列化后的数据
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }
    return true;
}

bool StringToTrecInfo(const std::string &strMsg, TrecInfo &info)
{
    try
    {
        std::istringstream is(strMsg);
        boost::archive::xml_iarchive ia(is);
        ia >> BOOST_SERIALIZATION_NVP(info);
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }
    return true;
}
