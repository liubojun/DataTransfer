#include "pluginBaseInfo.h"
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
		void serialize(Archive& ar, pluginInfo &pi, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(pi.bLoad);
			ar & BOOST_SERIALIZATION_NVP(pi.bUse);
			ar & BOOST_SERIALIZATION_NVP(pi.symbolic_name);
			ar & BOOST_SERIALIZATION_NVP(pi.name);
			ar & BOOST_SERIALIZATION_NVP(pi.cname);
			ar & BOOST_SERIALIZATION_NVP(pi.version);
			ar & BOOST_SERIALIZATION_NVP(pi.vendor);
			ar & BOOST_SERIALIZATION_NVP(pi.license);
			ar & BOOST_SERIALIZATION_NVP(pi.copyright);
			ar & BOOST_SERIALIZATION_NVP(pi.activation_policy);
			ar & BOOST_SERIALIZATION_NVP(pi.description);
			ar & BOOST_SERIALIZATION_NVP(pi.category);
			ar & BOOST_SERIALIZATION_NVP(pi.contact_address);
			ar & BOOST_SERIALIZATION_NVP(pi.doc_url);
			ar & BOOST_SERIALIZATION_NVP(pi.icon);
		}
	}
}

DLL_EXPORT_DECL bool PluginInfoToXml(const string &strPath, const pluginInfo &set)
{
	bool bRet = true;

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
		bRet = false;
	}

	return bRet;
}

DLL_EXPORT_DECL bool XmlToPluginInfo(const string &strPath, pluginInfo &set)
{
	bool bRet = true;

	try
	{
		ifstream ifs(strPath.c_str());
		if (!ifs.good())
		{
			bRet = false;
		}
		else
		{
			boost::archive::xml_iarchive xia(ifs);
			xia >> BOOST_SERIALIZATION_NVP(set);
		}
	}
	catch (std::exception &e)
	{
		cout << e.what();
		bRet = false;
	}

	return bRet;
}