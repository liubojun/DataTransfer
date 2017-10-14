/*************************************************************************
* Copyright(c) 2016,NRIET
* All Rights Reserved.
* @file		:	IDataParse.h
* @brief	:	雷达服务数据解析插件接口方法头文件声明
* @version	:	V1.0
* @author	:	NRIET
* @date		:	2016-03-09
------------------------------------------------------------------------------------------------------
* @modify	:	修改说明
* @brief	:	修改内容
* @author	:	修改者
* @date		:	修改日期
* @version	:	替换版本
**************************************************************************/
#ifndef IDataParse_h__
#define IDataParse_h__

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include "filenametable.h"
#include "struct_vtb.h"
#include "IBaseInterface.h"
#include "../3dpart/Common/PPIStruct.h"

class IRadarBaseDataParse;
class IRadarPubDataParse;
class IRadarMosaicDataParse;

class IDataParse : public IBaseInterface
{
    Q_OBJECT
    Q_INTERFACES(IBaseInterface)
public:
    virtual ~IDataParse() {}

    /**
     * @brief  创建基数据解析器
     * @return QSharedPointer<IRadarBaseDataParse>：基数据解析器实例指针
     */
    virtual QSharedPointer<IRadarBaseDataParse> createBaseDataParser() = 0;

    /**
     * @brief  创建产品数据解析器
     * @return QSharedPointer<IRadarPubDataParse>：产品数据解析器实例指针
     */
    virtual QSharedPointer<IRadarPubDataParse> createPubDataParser() = 0;

	/**
	 * @brief: 雷达组网产品数据解析
	 * @return: QSharedPointer<IRadarMosaicDataParse>: 描述返回值
	*/
	virtual QSharedPointer<IRadarMosaicDataParse> createMosaicDataParser()  = 0;
};

Q_DECLARE_INTERFACE(IDataParse, "org.radar.dataadapter.IDataParse")

#endif // IDataParse_h__
