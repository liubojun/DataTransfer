/**
  * @file   ConnectConfig.h
  * @brief  组网消息总线通信设置序列化与反序列化声明头文件.
  *
  *  这里是详细说明。
  * @author WEIKANGC
  * @par   Copyright (c):
  *          WEIKANGC.
  *         All Rights Reserved
  * @date   2016:05:16
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef _H_H_CONNECTCONFIG_H_H_
#define _H_H_CONNECTCONFIG_H_H_

#include "macro.h"

#include <string>
using namespace std;

#pragma warning(disable:4251)
/**
  * @class <ClientConfig>
  * @brief  消息总线客户端通信连接设置.
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */

class DLL_EXPORT_CLASS_DECL ClientConfig
{
public:
    ClientConfig();
    ClientConfig(int nType, std::string& sIP, unsigned long nPort, const std::string& sProcess, const std::string& sThread);
    ClientConfig(const ClientConfig& cC);
    ClientConfig& operator=(const ClientConfig& cC);
    ~ClientConfig();
public:
    void SetConnParam(int nType, std::string& sIP, unsigned long nPort, const std::string& sProcess, const std::string& sThread);
public:
    int m_nType;			///< 0 TCP 1 IPC 2 INPROC
    std::string m_sIP;		///< 通信IP地址
    unsigned long m_nPort;	///< 通信端口
    std::string m_sProcess;	///< 进程间通信
    std::string m_sThread;	///< 线程间通信
};


/**
 * @brief: 序列化客户端配置到字符串中 ClientConfig -> std::string
 * @param: std::string & sMsg
 * @param: const ClientConfig & m
 * @return: bool
*/
DLL_EXPORT_DECL bool SerializeClient2StdString(std::string& sCC, const ClientConfig& cC);


/**
 * @brief: 反序列化客户端配置到ClientConfig对象 std::string -> ClientConfig
 * @param: const std::string & sCC
 * @param: ClientConfig & cC
 * @return: bool
*/
DLL_EXPORT_DECL bool DeserializeStdString2Client(const std::string& sCC, ClientConfig& cC);


/**
 * @brief: 序列化客户端配置信息至XML文件中
 * @param: std::string & sXml
 * @param: const ClientConfig & cC
 * @return: bool
*/
DLL_EXPORT_DECL bool SerializeClient2XmlFile(const std::string& sXmlPath, const ClientConfig& cC);


/**
 * @brief: 反序列化客户端信息至其对象中
 * @param: const std::string & sXmlPath
 * @param: ClientConfig & cC
 * @return: bool
*/
DLL_EXPORT_DECL bool DeserializeXmlFile2Client(const std::string& sXmlPath,  ClientConfig& cC);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


/**
  * @class <ServerConfig>
  * @brief  消息总线服务端通信设置声明类.
  *
  *  这里是类的详细描述
  * @note    这里是注意事项
  * @see     这里是参考类名
 */
class DLL_EXPORT_CLASS_DECL ServerConfig
{
public:
    ServerConfig();
    ServerConfig(int nType, const std::string& sIP, unsigned long nPort, const std::string& sProcess, const std::string& sThread);
    ServerConfig(const ServerConfig& SC);
    ServerConfig& operator=(const ServerConfig& SC);
    ~ServerConfig();

    void SetConnParam(int nType, const std::string& sIP, unsigned long nPort, const std::string& sProcess, const std::string& sThread);

public:
    int m_nType;			///< 0 TCP 1 IPC 2 INPROC
    std::string m_sIP;		///< 通信IP地址
    unsigned long m_nPort;	///< 通信端口
    std::string m_sProcess;	///< 进程间通信
    std::string m_sThread;	///< 线程间通信
};


/**
 * @brief: 序列化服务端配置到字符串中 ServerConfig -> std::string
 * @param: std::string & sMsg
 * @param: const ServerConfig & m
 * @return: bool
*/
DLL_EXPORT_DECL bool SerializeServer2StdString(std::string& sCC, const ServerConfig& sC);


/**
 * @brief: 反序列化服务端配置到ServerConfig对象 std::string -> ServerConfig
 * @param: const std::string & sCC
 * @param: ServerConfig & cC
 * @return: bool
*/
DLL_EXPORT_DECL bool DeserializeStdString2Server(const std::string& sCC, ServerConfig& sC);


/**
 * @brief: 序列化服务端配置信息至XML文件中
 * @param: std::string & sXml
 * @param: const ServerConfig & cC
 * @return: bool
*/
DLL_EXPORT_DECL bool SerializeServer2XmlFile(std::string& sXmlPath, const ServerConfig& sC);


/**
 * @brief: 反序列化服务端信息至其对象中
 * @param: const std::string & sXmlPath
 * @param: ServerConfig & cC
 * @return: bool
*/
DLL_EXPORT_DECL bool DeserializeXmlFile2Server(const std::string& sXmlPath,  ServerConfig& sC);

#endif



