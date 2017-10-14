#ifndef pupdata_h__
#define pupdata_h__


#include "macro.h"		//导入导出声明
#include <vector>
#include "ProductDef.h"

/**
  * @class <>
  * @brief  雷达pup产品数据存储类.
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL CPupData
{
public:
    /**
     * @brief  构造函数
     * @return ：无
     */
    CPupData()
    {
        memset(&m_header, 0, sizeof(SquareProduct));
    }

    /**
     * @brief  析构函数
     * @return ：无
     */
    virtual ~CPupData() {};

public:
    /// 雷达直角坐标系头
    SquareProduct m_header;

    /// 雷达数据
    std::vector<char> m_oData;
};
#endif // pupdata_h__
