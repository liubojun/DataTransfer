/**
  * @file   collectSet.h
  * @brief  数据收集设置序列化.
  *
  *  这里是详细说明。
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:05:16
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef collectSet_H_
#define collectSet_H_

#include <QString>
#include <QMap>
#include <QVector>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
#include <QUuid>
using namespace std;
#include "../macro.h"

/**
  * @class <>
  * @brief  正则表达式与队列对应关系
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class CDataInfo
{
public:
    /**
     * @brief  默认构造函数
     * @return ：无
     */
    CDataInfo()
    {
    }

    /**
     * @brief  带参数构造函数
     * @param  const std::string & reg 正则表达式
     * @param  const std::vector<string> & queues 需要发送的队列
     * @return ：无
     */
    CDataInfo(const std::string &reg, const std::vector<string> &queues) : typeReg(reg), queNames(queues)
    {

    }
public:
    ///< 类型正则表达式
    string typeReg;

    ///< 对应的队列
    std::vector<string> queNames;
};

class NameMatchRule
{
public:
    /// 无效值
    const static int INVALID = -1;

    NameMatchRule()
    {
        nTimeIndex = INVALID;
        nStationIndex = INVALID;
    }

public:
    std::string strNameRegExp;	// 文件名正则表达式 (Z_RADR_I_Z)([0-9]{4})_([0-9]{14})(_O_DOR_SC_CAP.bin)
    int nTimeIndex;				// 从文件名中提取时间的序号，无效设置为-1
    int nStationIndex;			// 从文件名中提取站号的序号，无效设置为-1
};

class CollectSet
{
public:
    /// 无效值
    const static int INVALID = -1;

public:
    /// 收集目录唯一ID（要求唯一，在构造函数自动设置）
    string strId;

    /// 监控目录或URL
    string strPath;

    /// 目录监控方式（0-定时，1-实时）
    int nWtype;

    /// 调度规则
    string strDispatch;

    /// 数据类型和正则表达式们
    //map<string, vector<CDataInfo> > mapTypeRegs;

    std::map<std::string, NameMatchRule> mapTypeRegs;

    vector<string> vecQueNames;					//目标队列名

    /// 站名（用户输入）
    string strStation;

    bool	bEnable;			///< 是否启用


    ///// 扩充字段
    ///// 各项信息提取规则(正则表达式), 如文件名Z_RADR_I_Z9250_20160531003931_P_DOR_SA_R_10_230_5.250.bin
    ///// 使用(.*)(Z[0-9]{4})(_)([0-9]{14})(.*)(SA_R_10_230_5)(.*), 站号的提取规则为cap(2),时间的提取规则为cap(4)
    //string strCapRegular;

    ///// 站点提取规则索引下标，下标的索引从1开始
    //int nStationCapIndex;

    /////时间提取规则索引下标，下标的索引从1开始
    //int nTimeCapIndex;

    /////类型提取规则索引下标，下标的索引从1开始
    //int nTypeCapIndex;

    //////////////////////////////////////////////////////////////////////////
    //归档用
    //string strStartTime;						//起始时间
    //string strEndTime;						//终止时间

    CollectSet()
    {
        nWtype = INVALID;
        //nStationCapIndex = INVALID;
        //nTimeCapIndex = INVALID;
        //nTypeCapIndex = INVALID;
        strId = QUuid::createUuid().toString().toStdString();
    }

    //重载==，便于容器直接remove
    bool operator==(const CollectSet& CS)
    {
        if ( this->strId == CS.strId )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};


class CollectSetList
{
public:
    list<CollectSet> lsSets;
};

DLL_EXPORT_DECL bool CollectSetToXml(const string &strPath, const CollectSetList &setlst);
DLL_EXPORT_DECL bool XmlToCollectSet(const string &strPath, CollectSetList &setlst);


class HistoryCollect
{
public:
    QString strID;					///< 唯一标识
    //QString strPath;				///< 归档根路径
    QString strStartTime;			///< 起始时间
    QString strEndTime;				///< 终止时间
    int		nType;					///< 0-单站，1-组网，2-高空
    QString strQueName;				///< 目标消息队列名
    QList<QString> lstStations;		///< 站名（单站1个，组网多个）
    //QMap<QString, QVector<QString>> mapTypeRegs;	///< 类型和对应的正则表达式们
};

#endif