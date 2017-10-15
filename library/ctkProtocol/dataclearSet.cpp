#include "dataclearSet.h"
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

using namespace std;

namespace boost
{
    namespace serialization
    {
        template<class Archive>
        void serialize(Archive& ar, DataClearSet &set, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(set.strTask);
            ar & BOOST_SERIALIZATION_NVP(set.strPath);
            ar & BOOST_SERIALIZATION_NVP(set.strDrule);
            ar & BOOST_SERIALIZATION_NVP(set.nStyle);
            ar & BOOST_SERIALIZATION_NVP(set.reges);
            ar & BOOST_SERIALIZATION_NVP(set.nSize);
            ar & BOOST_SERIALIZATION_NVP(set.nTime);
        }

        template<class Archive>
        void serialize(Archive& ar, DataClearSetList &set, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(set.lst);
        }

// 		template<class Archive>
// 		void serialize(Archive& ar, DataClearSet_N &set, const unsigned int version)
// 		{
// 			ar & BOOST_SERIALIZATION_NVP( set.sClearTask );
// 			ar & BOOST_SERIALIZATION_NVP(set.sDir);
// 			ar & BOOST_SERIALIZATION_NVP(set.sFileRule);
// 			ar & BOOST_SERIALIZATION_NVP(set.bClearType);
// 			ar & BOOST_SERIALIZATION_NVP(set.bClearscheme);
// 			ar & BOOST_SERIALIZATION_NVP(set.nSize);
// 			ar & BOOST_SERIALIZATION_NVP(set.nTime);
// 			ar & BOOST_SERIALIZATION_NVP(set.strDrule);
// 		}
//
// 		template<class Archive>
// 		void serialize(Archive& ar, DataClearSetList_N &set, const unsigned int version)
// 		{
// 			ar & BOOST_SERIALIZATION_NVP(set.arrayDCS);
// 		}
    }
}

bool DataClearSetToXml(const string &strPath, const DataClearSetList &setlst)
{
    if (strPath.empty())
    {
        return false;
    }
    ofstream ofs(strPath.c_str());
    boost::archive::xml_oarchive xoa(ofs);
    xoa << BOOST_SERIALIZATION_NVP(setlst);
    ofs.flush();
    return true;
}

bool XmlToDataClearSet(const string &strPath, DataClearSetList &setlst)
{
    try
    {
        std::ifstream ifs(strPath.c_str());
        if (!ifs.good())
        {
            return false;
        }
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(setlst);
    }
    catch (std::exception &e)
    {
        std::cout<<e.what();
        return false;
    }

    return true;
}

//DLL_EXPORT_DECL bool DataClearSetNToXml(const string &strPath, const DataClearSetList_N &setlst)
//{
//    if (strPath.empty()) return false;
//
//    bool bRet = true;
//    try
//    {
//        ofstream ofs(strPath.c_str());
//        boost::archive::xml_oarchive xoa(ofs);
//        xoa << BOOST_SERIALIZATION_NVP(setlst);
//        ofs.flush();
//    }
//    catch(std::exception& e)
//    {
//        std::cout<<e.what()<<std::endl;
//        bRet = false;
//    }
//
//    return bRet;
//}
//
//DLL_EXPORT_DECL bool XmlToDataClearSetN(const string &strPath, DataClearSetList_N &setlst)
//{
//    bool bRet = true;
//
//    try
//    {
//        ifstream ifs(strPath.c_str());
//        if (!ifs.good()) return false;
//        boost::archive::xml_iarchive xia(ifs);
//        xia >> BOOST_SERIALIZATION_NVP(setlst);
//    }
//    catch(std::exception& e)
//    {
//        std::cout<<e.what()<<std::endl;
//        bRet = false;
//    }
//
//    return bRet;
//}