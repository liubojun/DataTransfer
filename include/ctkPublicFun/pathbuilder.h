﻿/**
  * @file   pathbuilder.h
  * @brief  xxx.
  *
  *  这里是详细说明。
  * @author nriet
  * @par   Copyright (c):
  *          nriet.
  *         All Rights Reserved
  * @date   2016:04:07
  * @note   这里是注意事项。
  * @version 1.0
  */

#ifndef pathbuilder_h__
#define pathbuilder_h__

#include "../macro.h"
#include "language.h"

#include <QString>
#include <QDate>

/**
  * @class <>
  * @brief  路径构造器.
  *
  *  这里是类的详细描述
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL CPathBuilder
{
public:
    CPathBuilder() {}

    /**
     * @brief  根据语言获取对应的翻译文件全路径
     * @param[in]  language_t lan 语言
     * @return QString：翻译文件的全路径
     */
    QString getTranslatorFilePath(language_t lan);


    /**
     * @brief  获取插件管理器路径
     * @return QString：插件管理器全路径
     */
    QString getPluginManagerPath();

    /**
     * @brief  获取插件路径
     * @return QString：插件所在路径
     */
    QString getPluginsPath();

    /**
     * @brief  获取算法插件所在目录
     * @return QT_NAMESPACE::QString：算法所在目录
     */
    QString getAlgorithmPath();

    /**
     * @brief  获取配置文件路径
     * @return QT_NAMESPACE::QString：配置文件所在路径（不带"/"）
     */
    QString getConfigPath();

    /**
     * @brief: 获取gisPlugins文件夹路径
     * @return: QT_NAMESPACE::QString: 描述返回值
    */
    QString getGisPluginsPath();

    /**
     * @brief: 获取基础地理信息的shape文件的文件夹maps的路径
     * @return: QT_NAMESPACE::QString: 描述返回值
    */
    QString getBaseMapsPath();

    /**
     * @brief: 获取qgis所需的资源文件
     * @return: QT_NAMESPACE::QString: 描述返回值
    */
    QString getResourcesPath();

    /**
     * @brief: 获取图片路径
     * @return QString: 图片资源根路径（不带"/"）
    */
    QString getImagePath();

    /**
     * @brief  获取curl本地缓存路径
     * @return QT_NAMESPACE::QString：无
     */
    QString getCurlFilePath();

    /**
     * @brief  从界面配置的URL中解析出路径[基于系统时间]
     * @return QT_NAMESPACE::QString：返回组装的url
     */
    static QStringList getFinalPathFromUrl(const QString &path);

    /**
    * @brief  从界面配置的URL中解析出路径[基于文件最后修改时间]
    * @return QT_NAMESPACE::QString：返回组装的url
    */
    static QStringList getFinalPathFromUrl(const QString &path, QDateTime in_oDt);

    /**
     * @brief  根据规则从文件名中替换目录变量规则
     * @param[in]  const QString &path 路径（可能包含变量）
     * @param[in]  const QString &filename 文件名
     * @return QT_NAMESPACE::QString：返回组装的url
     */
    static QString getFinalPathFromUrlV2(const QString &path, const QString &filename);

    /**
     * @brief  从URL以及文件目录中获取时间
     * @param[in]  const QString &rule 规则
     * @param[in]  const QString &path 路径
     * @return QT_NAMESPACE::QDateTime：返回获取的时间
     */
    static QDateTime getDateTimeFrom2Urls(const QString &rule, const QString &path);

    /**
     * @brief  从文件名中根据正则表达式获取时间
     * @param[in]  const QString &rule 规则
     * @param[in]  const QString &filename 文件名
     * @return QT_NAMESPACE::QDateTime：返回获取的时间
     */
    static QDateTime getDateTimeFromFileName(const QString &rule, const QString &filename);

    /**
     * @brief  set env for child process
     * @return int：sucess 0, failure -1
     */
    static int setChildProcessEnv();

    /**
     * @brief  组装ftp全路径
     * @param[in]  const QString &ip
     * @param[in]  const QString &port
     * @param[in]  const QString &path
     * @return QString：返回ftp全路径
     */
    static QString getFtpUrl(const QString &ip, int port, const QString &path);

protected:
    bool makeBuilderOrRunning();

    static QStringList parseFromUrl(const QString &url, QDateTime in_oDt);

private:
    // 程序路径
    QString m_strProgramPath;
};

#endif // pathbuilder_h__
