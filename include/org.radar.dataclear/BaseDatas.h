/**
  * @file   BaseDatas.h
  * @brief  数据清除结构.
  *
  *  这里是详细说明。
  * @author nriet
  * @par   Copyright (c):
  *          nriet.
  *         All Rights Reserved
  * @date   2016:04:08
  * @note   这里是注意事项。
  * @version 1.0
  */

#ifndef BaseDatas_h__
#define BaseDatas_h__

#include "../org.common.quartz/TimerCallBackParam.h"
#include <QString>
#include <QStringList>

/**
  * @class <BaseDatas>
  * @brief  清理文件需要的相关参数
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	TimerCallBackParam
 */
class BaseDatas : public TimerCallBackParam
{
public:
	QString m_taskName;		// 任务名
    QString m_DDrule;		//< 调度规则
    QString m_fullPath;		//< 文件路径
    QString m_style;		//< 删除方式（按时间或者空间剩余规则进行删除）
    QString m_freeSize;		//< 剩余空间（单位：M）
    int m_time;				//< 数据保留时间（单位：天=> 分钟）
    QStringList m_regex;    //< 匹配 （正则表达式）

	/////////////支撑ftp远程清理////////////////////////
	int taskType;		// 任务类型 0：目录清理，1: ftp清理
	QString ip;			// ftp ip
	quint16 port;		// ftp port
	QString user;		// ftp user
	QString password;	// ftp password
	int transfermode;	// ftp transfermode 0：主动， 1： 被动
};
#endif // BaseDatas_h__
