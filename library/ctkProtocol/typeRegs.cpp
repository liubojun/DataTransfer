#include "typeRegs.h"
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


DLL_EXPORT_C_DECL void TypeRegsToXml(const std::string &strPath, const TypeRegsList &infos)
{
	std::ofstream ofs(strPath.c_str());
	boost::archive::xml_oarchive xoa(ofs);
	// boost::archive::text_oarchive xoa(ofs);
	xoa << BOOST_SERIALIZATION_NVP(infos);
	ofs.flush();
}

DLL_EXPORT_C_DECL void XmlToTypeRegs(const std::string &strPath, TypeRegsList &infos)
{
	std::ifstream ifs(strPath.c_str());
	if (!ifs.good())
	{
		return;
	}
	boost::archive::xml_iarchive xia(ifs);
	// boost::archive::text_iarchive xia(ifs);
	xia >> BOOST_SERIALIZATION_NVP(infos);
}

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		void serialize(Archive& ar, TypeRegs &fi, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(fi.strType);
			ar & BOOST_SERIALIZATION_NVP(fi.vecRegs);
		}
	}
}

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		void serialize(Archive& ar, TypeRegsList &fi, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(fi.lsInfos);
		}
	}
}
