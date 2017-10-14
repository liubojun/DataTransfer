#include "radarAnimationSet.h"
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
#include  <QDir>
#include <QFileInfo>
#include <QFile>

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		void serialize(Archive &ar, RadarAnimationSet &set, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(set.m_interval);
			ar & BOOST_SERIALIZATION_NVP(set.m_adjust);
			ar & BOOST_SERIALIZATION_NVP(set.m_realInterval);
		}
	}
}

DLL_EXPORT_DECL bool RadarAnimationSetToXml(const string &strPath, const RadarAnimationSet &lst)
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

DLL_EXPORT_DECL bool XmlToRadarAnimationSet(const string &strPath, RadarAnimationSet &lst)
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
