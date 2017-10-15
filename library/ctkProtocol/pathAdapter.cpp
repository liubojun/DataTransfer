#include "pathAdapter.h"
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

#include <iostream>

namespace boost
{
    namespace serialization
    {
        template<class Archive>
        void serialize(Archive& ar, DataPathSetList &set, const unsigned int version)
        {
            //ar & BOOST_SERIALIZATION_NVP(set.strRoot);
            ar & BOOST_SERIALIZATION_NVP(set.type_third);
            ar & BOOST_SERIALIZATION_NVP(set.type_name);
            ar & BOOST_SERIALIZATION_NVP(set.type_root);
        }

        template<class Archive>
        void serialize(Archive& ar, OrgDataBackupPath &set, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(set.strPath);
        }
    }
}

DLL_EXPORT_DECL bool DataPathSetToXml(const string &strPath, const DataPathSetList &setlst)
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

DLL_EXPORT_DECL bool XmlToDataPathSet(const string &strPath, DataPathSetList &setlst)
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

DLL_EXPORT_DECL bool OrgBackupPathToXml(const string &strPath, const OrgDataBackupPath &set)
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

DLL_EXPORT_DECL bool XmlToOrgBackupPath(const string &strPath, OrgDataBackupPath &set)
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
