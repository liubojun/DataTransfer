#ifndef pupmosaicProduct_h__
#define pupmosaicProduct_h__

#include "../macro.h"
#include <vector>
#include <string>

/**
  * @class <>
  * @brief  pup产品数据组网拼图产品设置类.
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class CPupMosaicProductSetting
{
public:
    // 产品ID
    int nProductId;

    // 产品简写
    std::string strProductShortName;

    // 产品名
    std::string strProductName;

    // 是否生成产品数据
    bool bActiveData;

    // 是否生成产品图片
    bool bActiveImage;
};

/**
  * @class <>
  * @brief  pup产品数据组网拼图产品设置类..
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class CPupMosaicProductSettings
{
public:
    std::vector<CPupMosaicProductSetting> m_data;
};

/**
 * @brief  CPupMosaicProductSettings序列化到xml配置文件中
 * @param  const std::string & strPath xml配置文件全路径
 * @param  const CPupMosaicProductSettings & set 待序列化对象
 * @return DLL_EXPORT_C_DECL bool：序列化成功返回true,失败返回false
 */
DLL_EXPORT_C_DECL bool PupMosaicProductSetToXml(const std::string &strPath, const CPupMosaicProductSettings &set);

/**
 * @brief  从xml配置文件中反序列化出对象
 * @param  const std::string & strPath xml配置文件全路径
 * @param  CPupMosaicProductSettings & set 反序列化的对象
 * @return DLL_EXPORT_C_DECL bool：反序列化成功返回true,失败返回false
 */
DLL_EXPORT_C_DECL bool XmlToPupMosaicProductSet(const std::string &strPath, CPupMosaicProductSettings &set);

#endif // pupmosaicProduct_h__
