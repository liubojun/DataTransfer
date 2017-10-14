/**
  * @file   ITLogPDataParse.h
  * @brief  高空TLogP数据解析接口.
  *
  *  这里是详细说明。
  * @author 刘伯俊
  * @par   Copyright (c):
  *          刘伯俊.
  *         All Rights Reserved
  * @date   2016:04:07 
  * @note   这里是注意事项。
  * @version 1.0
  */  
#ifndef ITLogPDataParse_h__
#define ITLogPDataParse_h__

#include "IBaseInterface.h"

class ITLopDataParse : public IBaseInterface
{
	Q_OBJECT
	Q_INTERFACES(IBaseInterface)
public:

};

Q_DECLARE_INTERFACE(ITLopDataParse, "org.radar.dataadapter.ITLopDataParse")

#endif // ITLogPDataParse_h__


