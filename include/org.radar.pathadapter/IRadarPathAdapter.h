/**
  * @file   IRadarPathAdapter.h
  * @brief  雷达路径适配器接口.
  *
  *  这里是详细说明。
  * @author 刘伯俊
  * @par   Copyright (c):
  *          刘伯俊.
  *         All Rights Reserved
  * @date   2016:04:08
  * @note   这里是注意事项。
  * @version 1.0
  */

#ifndef IRadarPathAdapter_h__
#define IRadarPathAdapter_h__

#include "IBaseInterface.h"
#include "../org.radar.dataadapter/ProductDef.h"
#include "../org.radar.dataadapter/struct_vtb.h"
#include "IMakePath.h"


/**
  * @class <IRadarPathAdapter>
  * @brief  雷达数据路径适配器接口.
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class IRadarPathAdapter : public IBaseInterface
{
    Q_OBJECT
    Q_INTERFACES(IBaseInterface)
public:
    /**
     * @brief  默认析构函数
     */
    virtual ~IRadarPathAdapter() {}

    /**
     * @brief  创建路径适配器
     * @return QSharedPointer<IMakePath>：描述返回值
     */
    virtual QSharedPointer<IMakePath> createRadarProductPath() = 0;

};

Q_DECLARE_INTERFACE(IRadarPathAdapter, "org.radar.pathadapter.IRadarPathAdapter")
#endif // IRadarPathAdapter_h__