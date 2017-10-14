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
    QString m_DDrule;		//< 调度规则
    QString m_fullPath;		//< 文件路径
    QString m_style;		//< 删除方式（按时间或者空间剩余规则进行删除）
    QString m_freeSize;			//< 剩余空间（单位：M）
    int m_time;				//< 数据保留时间（单位：天=> 分钟）
    QStringList m_regex;    //< 匹配 （正则表达式）
};
#endif // BaseDatas_h__
