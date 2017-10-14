/**
  * @file   TimerCallBackParam.h
  * @brief  定时器超时信号抛出的参数对象.
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

#ifndef TimerCallBackParam_h__
#define TimerCallBackParam_h__

#include <QSharedPointer>
/**
  * @class <TimerCallBackParam>
  * @brief  定时器回调传递参数.
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */ 
class TimerCallBackParam
{
public:
	/**
	 * @brief  默认构造函数
	 */
	TimerCallBackParam(){}


	/**
	 * @brief  析构函数
	 */
	virtual ~TimerCallBackParam(){}

};

typedef QSharedPointer<TimerCallBackParam> TimerParam;
#endif // TimerCallBackParam_h__
