#include "testserial.h"

#include "boost/serialization/serialization.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/map.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include "boost/archive/xml_iarchive.hpp"
#include <boost/serialization/nvp.hpp>

void TestArchive1()
{
	CMyData d1(2012, "China, good luck");
	d1._arrary.push_back(1);
	d1._arrary.push_back(2);
	d1._arrary.push_back(3);
	d1._map.insert(std::make_pair(1, 2));
	d1._d._text = "1234test";

	std::ostringstream os;
	boost::archive::binary_oarchive oa(os);
	oa << d1;//序列化到一个ostringstream里面

	std::string content = os.str();//content保存了序列化后的数据。

	CMyData d2;
	std::istringstream is(content);
	boost::archive::binary_iarchive ia(is);
	ia >> d2;//从一个保存序列化数据的string里面反序列化，从而得到原来的对象。

	// std::cout << "CMyData tag: " << d2.GetTag() << ", text: " << d2.GetText() << "\n";

	std::ofstream ofs("test.xml");
	boost::archive::xml_oarchive xoa(ofs);
	xoa << BOOST_SERIALIZATION_NVP(d1);
	ofs.flush();
	//xoa << d1;
	//content = os.str();

	//CMyData d3;
	//std::ifstream ifs("test.xml");
	//boost::archive::xml_iarchive xia(ifs);
	//xia >> BOOST_SERIALIZATION_NVP(d3);
}


namespace boost
{
	namespace serialization
	{
		template<class Archive>
		void serialize(Archive& ar, CInterData &d, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(d._tag);
			ar & BOOST_SERIALIZATION_NVP(d._text);
			ar & BOOST_SERIALIZATION_NVP(d._arrary);
			ar & BOOST_SERIALIZATION_NVP(d._list);
			ar & BOOST_SERIALIZATION_NVP(d._map);
		}


		template<class Archive>
		void serialize(Archive& ar, CMyData &d, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(d._tag);
			ar & BOOST_SERIALIZATION_NVP(d._text);
			ar & BOOST_SERIALIZATION_NVP(d._arrary);
			ar & BOOST_SERIALIZATION_NVP(d._list);
			ar & BOOST_SERIALIZATION_NVP(d._map);
			ar & BOOST_SERIALIZATION_NVP(d._d);
		}
	}
}
