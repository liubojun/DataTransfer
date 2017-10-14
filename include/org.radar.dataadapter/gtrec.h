#include <string>
using namespace std;

#ifndef GTREC_H_
#define GTREC_H_

namespace trec
{
	// Trec.ini配置文件路径
	const string GTREC_INI = "config/Trec.ini";

	// 插值使用周围非0元素个数
	const string GTREC_NONZERO = "NonZeroNum";

	// 集合外推时间槽
	const string GTREC_TIMESLOT = "TrecTimeSlot";

	// 外推DBZ率值
	const string GTREC_DBZFILTER = "DBZFilter";

	// 外推时次
	const string GTREC_FORECASTTIME = "ForecastTime";

	// 外推处理启动线程数目
	const string GTREC_THREADNUM = "threadNum";

	// 集合外推使用的权重
	const string GTREC_WEIGHT = "Weight";

	// ActiveMQ配置文件路径
	const string GTREC_MQ_INI = "Config/activeMQ.ini";

	// 全局组
	const string GTREC_GLOBAL_GROUP = "global";

	// Trec组配置
	const string GTREC_TREC_GROUP = "Trec";

	// 获取ActiveMq.ini配置中的MQ地址
	const string GTREC_MQURL = "ActiveMqUrl";

	// Trec获取消息的队列名
	const string GREC_MQNAME = "SrcMqName";

	// 删除历史文件时，删除历史文件的小时
	const string GTREC_DELHISTIME = "DelHistoryFile";

	const string GTREC_SAVERDFFILE = "SaveRdfFile";

	const string GTREC_ISDRAWMAP = "IsDrawMap";
	const string GTREC_RDFFILEPATH = "RdfFilePath";

	// MaxR文件名中，时间的起始位置
	const int GTREC_TIME_BEGIN = 0;

	// MaxR文件中，时间的长度
	const int GTREC_TIME_LEN = 14;

	// 路径解析相关配置

	enum TRECTYPE
	{
		MAXR = 1
	};
}
#endif /* GTREC_H_ */
