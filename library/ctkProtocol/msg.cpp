#include "msg.h"
#include <sstream>
#include <fstream>
#include <algorithm>

#include "boost/serialization/serialization.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/map.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include "boost/archive/xml_iarchive.hpp"
#include "boost/serialization/queue.hpp"
#include <boost/serialization/nvp.hpp>

#pragma warning(disable:4101)

msgheader::msgheader()
    : m_queueid(0)
    , m_queuename("")
    , m_msgid(1000)
    , m_msgtime(time(NULL))
    , m_bRemove(true) {}

msgheader::msgheader(unsigned int i_queueid, std::string i_queuename, unsigned int i_msgid, unsigned long i_msgtime)
    : m_queueid(i_queueid)
    , m_queuename(i_queuename)
    , m_msgid(i_msgid)
    , m_msgtime(i_msgtime)
    , m_bRemove(true) {}

msgheader::msgheader(const msgheader& header)
{
    this->m_queueid	  = header.m_queueid;
    this->m_queuename = header.m_queuename;
    this->m_msgid	  = header.m_msgid;
    this->m_msgtime	  = header.m_msgtime;
    this->m_bRemove	  = header.m_bRemove;
}

msgheader& msgheader::operator=(const msgheader& header)
{
    if(this == &header)
        return *this;


    this->m_queueid   = header.m_queueid;
    this->m_queuename = header.m_queuename;
    this->m_msgid	  = header.m_msgid;
    this->m_msgtime   = header.m_msgtime;
    this->m_bRemove	  = header.m_bRemove;

    return *this;
}

msgheader::~msgheader() {}

msgbody::msgbody(void)
    : m_msgdata("") {}

msgbody::msgbody(std::string i_msgdata)
    : m_msgdata(i_msgdata) {}

msgbody::msgbody(const msgbody& body)
    : m_msgdata(body.m_msgdata)
{

}

msgbody::msgbody(const char *i_msgdata) : m_msgdata(i_msgdata)
{

}

msgbody& msgbody::operator=(const msgbody& body)
{
    if( this == &body )
        return *this;

    this->m_msgdata = body.m_msgdata;

    return *this;
}

msgbody::~msgbody() {}

msg::msg(void)
    : head()
    , body()
{

}

msg::msg(unsigned int i_queueid, std::string i_queuename, unsigned int i_msgid, unsigned long i_msgtime, std::string i_msgdata)
    : head(i_queueid, i_queuename, i_msgid, i_msgtime)
    , body(i_msgdata)
{

}

msg::msg(const msg& m)
    : head(m.head)
    , body(m.body)
{

}

msg& msg::operator=(const msg& m)
{
    if( this == &m )
        return *this;

    this->head.operator=(m.head);
    this->body.operator=(m.body);

    return *this;
}

msg::~msg(void)
{
}

msgs::msgs(void)
{
    /*for ( int nIndex = 0 ; nIndex < m_array.size(); ++nIndex )
    {
    	m_array.pop();
    }*/
}

msgs::msgs(const msgs& ms)
{
    int nIndex = 0 ;

    this->m_array.clear();

    nIndex = 0 ;
    for ( ; nIndex < ms.m_array.size(); ++nIndex)
    {
        this->m_array.push_back(ms.m_array[nIndex]);
    }
}

msgs& msgs::operator=(const msgs& ms)
{
    if ( this == &ms )
        return *this;

    int nIndex = 0;
    for (; nIndex < ms.m_array.size(); ++nIndex)
    {
        this->m_array.push_back(ms.m_array[nIndex]);
    }

    return *this;
}

msgs::~msgs()
{

}

msgQuery::msgQuery(void)
    : m_m()
    , m_lPreTime(0)
    , m_lAftTime(0)
    , m_lPreDay(0)
    , m_lAftDay(0)
    , m_sStationNo("")
    , m_iMatchingTime(0)
    , m_sQueueName("")

{

}

msgQuery::msgQuery(msg m, long lPreTime, long lAftTime, unsigned long lPreDay, unsigned long lAftDay, std::string sStationNo, unsigned short iMatchingTime, std::string sQueueName)
    : m_m(m)
    , m_lPreTime(lPreTime)
    , m_lAftTime(lAftTime)
    , m_lPreDay(lPreDay)
    , m_lAftDay(lAftDay)
    , m_sStationNo(sStationNo)
    , m_iMatchingTime(iMatchingTime)
    , m_sQueueName(sQueueName)
{

}

msgQuery::msgQuery(const msgQuery& mQ)
{
    this->m_m		= mQ.m_m;
    this->m_lPreTime= mQ.m_lPreTime;
    this->m_lAftTime= mQ.m_lAftTime;
    this->m_lPreDay	= mQ.m_lPreDay;
    this->m_lAftDay	= mQ.m_lAftDay;

    this->m_sStationNo = mQ.m_sStationNo;
    this->m_iMatchingTime = mQ.m_iMatchingTime;
    this->m_sQueueName = mQ.m_sQueueName;
}

msgQuery& msgQuery::operator=(const msgQuery& mQ)
{
    if ( this != &mQ )
    {
        this->m_m		= mQ.m_m;
        this->m_lPreTime= mQ.m_lPreTime;
        this->m_lAftTime= mQ.m_lAftTime;
        this->m_lPreDay	= mQ.m_lPreDay;
        this->m_lAftDay	= mQ.m_lAftDay;

        this->m_sStationNo = mQ.m_sStationNo;
        this->m_iMatchingTime = mQ.m_iMatchingTime;
        this->m_sQueueName = mQ.m_sQueueName;
    }

    return *this;
}

msgQuery::~msgQuery()
{

}

strmsgqueuearray::strmsgqueuearray(void)
    : m_queuename("")
    , m_msgs()
    , m_m() {}

strmsgqueuearray::strmsgqueuearray(const strmsgqueuearray& sQA)
{
    this->m_queuename = sQA.m_queuename;
    this->m_msgs	  = sQA.m_msgs;
    this->m_m		  = sQA.m_m;
}

strmsgqueuearray& strmsgqueuearray::operator=(const strmsgqueuearray& sQA)
{
    if ( this != &sQA )
    {
        this->m_queuename = sQA.m_queuename;
        this->m_msgs.operator=(sQA.m_msgs);
        this->m_m.operator=(sQA.m_m);
    }

    return *this;
}

strmsgqueuearray::~strmsgqueuearray() {}

queueQuery::queueQuery(void)
    : m_queuename("")
    , m_queueId(0) {}

queueQuery::queueQuery(std::string i_queuename, const unsigned int i_queueId)
    : m_queuename(i_queuename)
    , m_queueId(i_queueId)
{}

queueQuery::queueQuery(const queueQuery& qQ)
{
    this->m_queuename = qQ.m_queuename;
    this->m_qIArray	  = qQ.m_qIArray;
    this->m_m		  = qQ.m_m;
    this->m_qIS		  = qQ.m_qIS;
    this->m_queueId	  = qQ.m_queueId;
}

queueQuery& queueQuery::operator=(const queueQuery& qQ)
{
    if ( this != &qQ )
    {
        this->m_queuename = qQ.m_queuename;
        this->m_qIArray	  = qQ.m_qIArray;
        this->m_m		  = qQ.m_m;
        this->m_qIS		  = qQ.m_qIS;
        this->m_queueId	  = qQ.m_queueId;
    }

    return *this;
}

queueQuery::~queueQuery()
{
    this->m_qIArray.clear();
}

ConnectInfo::ConnectInfo()
{
	this->state		= true;
	this->station	= "";
    this->connId	= "";
    this->connTime	= time(NULL);
}

ConnectInfo::~ConnectInfo() {}

ConnectInfo::ConnectInfo(bool bs, std::string s, std::string Id, time_t t_T)
{
	this->state		= bs;
	this->station	= s;
    this->connId	= Id;
    this->connTime	= t_T;
}

ConnectInfo::ConnectInfo(const ConnectInfo& cI)
{
	this->state		= cI.state;
	this->station	= cI.station;
    this->connId	= cI.connId;
    this->connTime	= cI.connTime;
}

ConnectInfo& ConnectInfo::operator=(const ConnectInfo& cI)
{
    if ( this != &cI )
    {
		this->state		= cI.state;
		this->station	= cI.station;
        this->connId	= cI.connId;
        this->connTime	= cI.connTime;		
    }

    return *this;
}

ConnectArray::ConnectArray()
{
    m_cIA.clear();
}

ConnectArray::~ConnectArray()
{

}

ConnectArray::ConnectArray(const ConnectArray& cTA):m_cIA(cTA.m_cIA) {}

ConnectArray& ConnectArray::operator=(const ConnectArray& cTA)
{
    if( this == &cTA)
        return *this;

    this->m_cIA = cTA.m_cIA;

    return *this;
}

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		void serialize(Archive& ar, msgheader &h, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(h.m_queueid);
			ar & BOOST_SERIALIZATION_NVP(h.m_queuename);
			ar & BOOST_SERIALIZATION_NVP(h.m_msgid);
			ar & BOOST_SERIALIZATION_NVP(h.m_msgtime);
			ar & BOOST_SERIALIZATION_NVP(h.m_bRemove);
		}

		template<class Archive>
		void serialize(Archive& ar, msgbody &b, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(b.m_msgdata);
		}

		template<class Archive>
		void serialize(Archive& ar, msg &m, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(m.head);
			ar & BOOST_SERIALIZATION_NVP(m.body);
		}

		template<class Archive>
		void serialize(Archive& ar, msgs &ms, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(ms.m_array);
		}

		template<class Archive>
		void serialize(Archive& ar, msgQuery &msq, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(msq.m_m);
			ar & BOOST_SERIALIZATION_NVP(msq.m_sStationNo);
			ar & BOOST_SERIALIZATION_NVP(msq.m_lAftDay);
			ar & BOOST_SERIALIZATION_NVP(msq.m_lAftTime);
			ar & BOOST_SERIALIZATION_NVP(msq.m_lPreDay);
			ar & BOOST_SERIALIZATION_NVP(msq.m_lPreTime);
			ar & BOOST_SERIALIZATION_NVP(msq.m_iMatchingTime);
			ar & BOOST_SERIALIZATION_NVP(msq.m_sQueueName);
		}

		template<class Archive>
		void serialize(Archive& ar, strmsgqueuearray &sMQA, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(sMQA.m_queuename);
			ar & BOOST_SERIALIZATION_NVP(sMQA.m_msgs);
			ar & BOOST_SERIALIZATION_NVP(sMQA.m_m);
		}

		template<class Archive>
		void serialize(Archive& ar, QUEUEINFOSTRUCT &qIS, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(qIS.queueID);
			ar & BOOST_SERIALIZATION_NVP(qIS.queueName);
			ar & BOOST_SERIALIZATION_NVP(qIS.level);
			ar & BOOST_SERIALIZATION_NVP(qIS.type);
			ar & BOOST_SERIALIZATION_NVP(qIS.curMsgSize);
			ar & BOOST_SERIALIZATION_NVP(qIS.preMsgSize);
			ar & BOOST_SERIALIZATION_NVP(qIS.curCustomers);
			ar & BOOST_SERIALIZATION_NVP(qIS.preCustomers);
			ar & BOOST_SERIALIZATION_NVP(qIS.arrayConnID);
		}

		template<class Archive>
		void serialize(Archive& ar, queueQuery &qQ, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(qQ.m_queuename);
			ar & BOOST_SERIALIZATION_NVP(qQ.m_qIArray);
			ar & BOOST_SERIALIZATION_NVP(qQ.m_m);
			ar & BOOST_SERIALIZATION_NVP(qQ.m_qIS);
			ar & BOOST_SERIALIZATION_NVP(qQ.m_queueId);
		}

		template<class Archive>
		void serialize(Archive& ar, ConnectInfo& cI, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(cI.state);
			ar & BOOST_SERIALIZATION_NVP(cI.station);
			ar & BOOST_SERIALIZATION_NVP(cI.connId);
			ar & BOOST_SERIALIZATION_NVP(cI.connTime);
		}

		template<class Archive>
		void serialize(Archive& ar, ConnectArray& cTA, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(cTA.m_cIA);
		}

		template<class Archive>
		void serialize(Archive& ar, filedata& fd, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(fd.size);
			ar & BOOST_SERIALIZATION_NVP(fd.name);
			ar & BOOST_SERIALIZATION_NVP(fd.path);
			ar & BOOST_SERIALIZATION_NVP(fd.buffer);    
		}

		template<class Archive>
		void serialize(Archive& ar, REQPRODUCTINFO& rPI, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP( rPI.type );
			ar & BOOST_SERIALIZATION_NVP( rPI.area );
			ar & BOOST_SERIALIZATION_NVP( rPI.tm );
			ar & BOOST_SERIALIZATION_NVP( rPI.productid );
			ar & BOOST_SERIALIZATION_NVP( rPI.productname );
			ar & BOOST_SERIALIZATION_NVP( rPI.file );
			ar & BOOST_SERIALIZATION_NVP( rPI.data );
		}

		template<class Archive>
		void serialize(Archive& ar, QUERYHISFILE& qHF, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP( qHF.m_areaCode );
			ar & BOOST_SERIALIZATION_NVP( qHF.m_productID );
			ar & BOOST_SERIALIZATION_NVP( qHF.m_startDateTime );
			ar & BOOST_SERIALIZATION_NVP( qHF.m_endDateTime );
			ar & BOOST_SERIALIZATION_NVP( qHF.arrayFileList );
		}
	}
}


bool SerializeMsg2StdString(std::string& sMsg, const msg& m)
{
    bool bRet = true;

    try
    {
        std::ostringstream os;
        boost::archive::xml_oarchive oa(os);
        oa << BOOST_SERIALIZATION_NVP(m);
        sMsg = os.str();
    }
    catch(std::exception& e)
    {
        sMsg.clear();
        std::string s(e.what(), strlen(e.what()));

        bRet = false;
    }

    return bRet;
}

bool DeserializeStdString2Msg(const std::string& sMsg, msg& m)
{
    bool bRet = true;

    try
    {
        std::istringstream is(sMsg);
        boost::archive::xml_iarchive ia(is);
        ia >> BOOST_SERIALIZATION_NVP(m);
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool SerializeMsg2XmlFile(std::string& sXmPath, const msg& m)
{
    bool bRet = true;

    try
    {
        std::ofstream ofs(sXmPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(m);
        ofs.flush();
    }
    catch(std::exception& e)
    {
        sXmPath.clear();
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool DeserializeXmlFile2Msg(const std::string& sXmlPath, msg& m)
{
    bool bRet = true;

    try
    {
        std::ifstream ifs(sXmlPath.c_str());
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(m);
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool SerializeMsg2File(std::string& sFilePath, const msg& m)
{
    bool bRet = true;

    try
    {
        std::ofstream ofs(sFilePath.c_str());
        boost::archive::binary_oarchive oa(ofs);
        oa << BOOST_SERIALIZATION_NVP(m);
        ofs.flush();
    }
    catch(std::exception& e)
    {
        sFilePath.clear();
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool DeserializeFile2Msg(const std::string& sFilePath, msg& m)
{
    bool bRet = true;

    try
    {
        std::ifstream ifs(sFilePath.c_str());
        boost::archive::binary_iarchive ia(ifs);
        ia >> BOOST_SERIALIZATION_NVP(m);
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool SerializeArrayMsg2XmlFile(std::string& sXmlPath, const msgs& QMS)
{

    bool bRet = true;

    try
    {
        std::ofstream ofs(sXmlPath.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(QMS.m_array);
        ofs.flush();
    }
    catch(std::exception& e)
    {
        sXmlPath.clear();
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool DeserializeXmlFile2ArrayMsg(const std::string& sXmlPath, msgs& QMS)
{
    bool bRet = true;

    try
    {
        std::ifstream ifs(sXmlPath.c_str());
        boost::archive::xml_iarchive xia(ifs);
        xia >> BOOST_SERIALIZATION_NVP(QMS.m_array);
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool SerializeMsgQuery2StdString(std::string& sMsg, const msgQuery& m)
{
    bool bRet = true;

    try
    {
        std::ostringstream os;
        boost::archive::xml_oarchive oa(os);
        oa << BOOST_SERIALIZATION_NVP(m);
        sMsg = os.str();
    }
    catch(std::exception& e)
    {
        sMsg.clear();
        std::string s(e.what(), strlen(e.what()));

        bRet = false;
    }

#if 0
    try
    {
        std::ostringstream os;
        boost::archive::binary_oarchive oa(os);
        oa << m;
        sMsg = os.str();
    }
    catch(std::exception& e)
    {
        sMsg.clear();
        std::string s(e.what(), strlen(e.what()));

        bRet = false;
    }
#endif

    return bRet;
}

bool DeserializeStdString2MsgQuery(const std::string& sMsg, msgQuery& m)
{
    bool bRet = true;

    try
    {
        std::istringstream is(sMsg);
        boost::archive::xml_iarchive ia(is);
        ia >> BOOST_SERIALIZATION_NVP(m);;
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

#if 0
    try
    {
        std::istringstream is(sMsg);
        boost::archive::binary_iarchive ia(is);
        string str = is.str();
        ia >> m;
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }
#endif

    return bRet;
}

bool Serializestrmsgqueuearray2StdString(std::string& sMsg, const strmsgqueuearray& m)
{
    bool bRet = true;

    try
    {
        std::ostringstream os;
        boost::archive::xml_oarchive oa(os);
        oa << BOOST_SERIALIZATION_NVP(m);
        sMsg = os.str();
    }
    catch(std::exception& e)
    {
        sMsg.clear();
        std::string s(e.what(), strlen(e.what()));

        bRet = false;
    }

    return bRet;
}

bool DeserializeStdString2strmsgqueuearray(const std::string& sMsg, strmsgqueuearray& m)
{
    bool bRet = true;

    try
    {
        std::istringstream is(sMsg);
        boost::archive::xml_iarchive ia(is);
        ia >> BOOST_SERIALIZATION_NVP(m);
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool SerializequeueQuery2StdString(std::string& sMsg, const queueQuery& m)
{
    bool bRet = true;

    try
    {
        std::ostringstream os;
        boost::archive::xml_oarchive oa(os);
        oa << BOOST_SERIALIZATION_NVP(m);
        sMsg = os.str();
    }
    catch(std::exception& e)
    {
        sMsg.clear();
        std::string s(e.what(), strlen(e.what()));

        bRet = false;
    }

    return bRet;
}

bool DeserializeStdString2queueQuery(const std::string& sMsg, queueQuery& m)
{
    bool bRet = true;

    try
    {
        std::istringstream is(sMsg);
        boost::archive::xml_iarchive ia(is);
        ia >> BOOST_SERIALIZATION_NVP(m);
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool SerializeConnectionInfo2String(std::string& sMsg, const ConnectInfo& cI)
{
    bool bRet = true;

    try
    {
        std::ostringstream os;
        boost::archive::xml_oarchive oa(os);
        oa << BOOST_SERIALIZATION_NVP(cI);
        sMsg = os.str();
    }
    catch(std::exception& e)
    {
        bRet = false;
    }

    return bRet;
}

bool DeserializeStdString2ConnectionInfo(const std::string& sMsg, ConnectInfo& cI)
{
    bool bRet = true;

    try
    {
        std::istringstream is(sMsg);
        boost::archive::xml_iarchive ia(is);
        ia >> BOOST_SERIALIZATION_NVP(cI);
    }
    catch(std::exception& e)
    {
        bRet = false;
    }

    return bRet;
}

bool SerializeConnectArray2String(std::string& sMsg, const ConnectArray& cI)
{
    bool bRet = true;

    try
    {
        std::ostringstream os;
        boost::archive::xml_oarchive oa(os);
        oa << BOOST_SERIALIZATION_NVP(cI);
        sMsg = os.str();
    }
    catch(std::exception& e)
    {
        bRet = false;
    }

    return bRet;
}

bool DeserializeStdString2ConnectArray(const std::string& sMsg, ConnectArray& cI)
{
    bool bRet = true;

    try
    {
        std::istringstream is(sMsg);
        boost::archive::xml_iarchive ia(is);
        ia >> BOOST_SERIALIZATION_NVP(cI);
    }
    catch(std::exception& e)
    {
        bRet = false;
    }

    return bRet;
}

bool SerializeProductInfo2String(std::string& sMsg, const REQPRODUCTINFO& reqPI)
{
    bool bRet = true;

    try
    {
        std::ostringstream os;
        boost::archive::xml_oarchive oa(os);
        oa << BOOST_SERIALIZATION_NVP(reqPI);
        sMsg = os.str();
    }
    catch(std::exception& e)
    {
        bRet = false;
    }

    return bRet;
}

bool DeserialzeString2ProductInfo(const std::string& sMsg, REQPRODUCTINFO& reqPI)
{
    bool bRet = true;

    try
    {
        std::istringstream is(sMsg);
        boost::archive::xml_iarchive ia(is);
        ia >> BOOST_SERIALIZATION_NVP(reqPI);
    }
    catch(std::exception& e)
    {
		std::string s(e.what());		
        bRet = false;
    }


    return bRet;
}

bool SerializeQueryHisFile2String(std::string& sMsg, const QUERYHISFILE& qHF)
{
	bool bRet = true;

	try
	{
		std::ostringstream os;
		boost::archive::xml_oarchive oa(os);
		oa << BOOST_SERIALIZATION_NVP(qHF);
		sMsg = os.str();
	}
	catch(std::exception& e)
	{
		bRet = false;
	}

	return bRet;
}

bool DeserializeString2QueryHisFile(const std::string& sMsg, QUERYHISFILE& qHF)
{
	bool bRet = true;

	try
	{
		std::istringstream is(sMsg);
		boost::archive::xml_iarchive ia(is);
		ia >> BOOST_SERIALIZATION_NVP(qHF);
	}
	catch(std::exception& e)
	{
		std::string s(e.what());		
		bRet = false;
	}


	return bRet;
}
