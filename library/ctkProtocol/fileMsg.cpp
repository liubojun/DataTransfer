#include "boost/serialization/serialization.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/map.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include "boost/archive/xml_iarchive.hpp"
#include "boost/serialization/nvp.hpp"

#include "fileMsg.h"

void FileInfoToString(std::string &strMsg, const FileInfo &fileInfo)
{
    std::ostringstream os;
    boost::archive::xml_oarchive oa(os);	//modify by weikangc 2016-06-02. 序列化成XML可见字符串
    oa << BOOST_SERIALIZATION_NVP(fileInfo);		//序列化到一个ostringstream里面
    strMsg = os.str();	//保存序列化后的数据
}

void StringToFileInfo(const std::string &strMsg, FileInfo &fileInfo)
{
    std::istringstream is(strMsg);
    //std::istringstream is(strMsg);
    boost::archive::xml_iarchive ia(is);	//modify by weikangc 2016-06-02. 将XML可见字符串反序列成其对象
    ia >> BOOST_SERIALIZATION_NVP(fileInfo);		//得到原来的对象
}

namespace boost
{
namespace serialization
{
template<class Archive>
void serialize(Archive& ar, FileInfo &fi, const unsigned int version)
{
    ar & BOOST_SERIALIZATION_NVP(fi.strFileName);
    ar & BOOST_SERIALIZATION_NVP(fi.strFilePath);
    ar & BOOST_SERIALIZATION_NVP(fi.strFileTime);
    ar & BOOST_SERIALIZATION_NVP(fi.strFileType);
    ar & BOOST_SERIALIZATION_NVP(fi.strStation);
    ar & BOOST_SERIALIZATION_NVP(fi.strMdyTime);
    ar & BOOST_SERIALIZATION_NVP(fi.nFileType);
}
}
}

// FileInfo& FileInfo::operator=(const FileInfo &fi)
// {
// 	strFileName = fi.strFileName;
// 	strFilePath = fi.strFilePath;
// 	strFileTime = fi.strFileTime;
// 	strFileType = fi.strFileType;
// 	strStation = fi.strStation;
// 	strMdyTime = fi.strMdyTime;
// nFileType = fi.nFileType;
// strOldType = fi.strOldType;
// nFileSize = fi.nFileSize;
//     strDestQueues = fi.strDestQueues;
//     strOrgFilePath = fi.strOrgFilePath;
//     return *this;
// }
//
// FileInfo::FileInfo(const FileInfo &fi)
// {
//     strFileName = fi.strFileName;
//     strFilePath = fi.strFilePath;
//     strFileTime = fi.strFileTime;
//     strFileType = fi.strFileType;
//     strStation = fi.strStation;
//     strMdyTime = fi.strMdyTime;
// nFileType = fi.nFileType;
// strOldType = fi.strOldType;
// nFileSize = fi.nFileSize;
//     strDestQueues = fi.strDestQueues;
//strOrgFilePath = fi.strOrgFilePath;