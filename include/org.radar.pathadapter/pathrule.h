/**
  * @file   pathrule.h
  * @brief  路径规则定义.
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

#ifndef pathrule_h__
#define pathrule_h__

#include <QString>

namespace pathrule
{
	/// 年
	const QString PR_YEAR = "%yyyy";

	/// 月
	const QString PR_MONTH = "%MM";

	/// 日
	const QString PR_DAY = "%dd";

	/// 时
	const QString PR_HOUR = "%hh";

	/// 分
	const QString PR_MIN = "%mm";

	/// 秒
	const QString PR_SEC = "%ss";

	/// 站号
	const QString PR_STATION_ID = "%stationId";

	/// 站名
	const QString PR_STATION_NE = "%stationNe";

	/// 产品号
	const QString PR_PRODUCT_ID = "%productId";

	/// 产品名
	const QString PR_PRODUCT_NE = "%productNe";

	/// 产品缩写
	const QString PR_PRODUCT_SNE = "%productSNe";

	/// 产品参数
	const QString PR_PRODUCT_PARAM = "%productParam";
}

#endif // pathrule_h__