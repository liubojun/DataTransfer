#include "IXClientCommSet.h"
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
		void serialize(Archive &ar, IXClientCommSet &set, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(set.Server);
			ar & BOOST_SERIALIZATION_NVP(set.SubClientPort);
			ar & BOOST_SERIALIZATION_NVP(set.ReqClientPort);
		}

		template<class Archive>
		void serialize(Archive &ar, FourProductInfo &set, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(set.productname);
			ar & BOOST_SERIALIZATION_NVP(set.productcode);
			ar & BOOST_SERIALIZATION_NVP(set.productheight);
		}
		
		template<class Archive>
		void serialize(Archive &ar, FourProductInfos &set, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(set.m_fPIS);
		}
	}
}

DLL_EXPORT_DECL bool IXClientCommSet2XML(const string &strPath, const IXClientCommSet &lst)
{
    try
    {
        ofstream ofs(strPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(lst);
        ofs.flush();
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }

    return false;
}

DLL_EXPORT_DECL bool XML2IXClientCommSet(const string &strPath, IXClientCommSet &lst)
{
    try
    {
        ifstream ifs(strPath.c_str());
        if (!ifs.good())
        {
            return false;
        }
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(lst);
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }

    return true;
}

DLL_EXPORT_DECL bool FourProductInfos2Xml(const string &strPath, const FourProductInfos &lst)
{
	try
	{
		ofstream ofs(strPath.c_str());
		boost::archive::xml_oarchive xoa(ofs);
		xoa << BOOST_SERIALIZATION_NVP(lst);
		ofs.flush();
	}
	catch (std::exception &e)
	{
		cout << e.what();
		return false;
	}

	return true;
}

DLL_EXPORT_DECL bool Xml2FourProductInfos(const string &strPath, FourProductInfos &lst)
{
	try
	{
		ifstream ifs(strPath.c_str());
		if (!ifs.good())
		{
			return false;
		}
		boost::archive::xml_iarchive xia(ifs);
		xia >> BOOST_SERIALIZATION_NVP(lst);
	}
	catch (std::exception &e)
	{
		cout << e.what();
		return false;
	}

	return true;
}
