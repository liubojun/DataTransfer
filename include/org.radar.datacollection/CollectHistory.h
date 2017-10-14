/**
  * @file   CollectHistory.h
  * @brief  处理归档数据.
  *
  *  这里是详细说明。
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:06:06 
  * @note   这里是注意事项。
  * @version <版本号>
  */  

#ifndef CollectHistory_H_
#define CollectHistory_H_

#include <string>
using namespace std;
#include <QString>
#include <QVector>
#include <QMap>
#include <QDir>
#include <QThread>
#include "commontypedef.h"
#include "ICtkPluginManager.h"
#include "IDispatchTimer.h"
#include "filenametable.h"
#include "ctkLog.h"
#include "../ctkProtocol/fileMsg.h"
#include "IDataCollection.h"
#include "lateFileTime.h"
#include "ILogPrint.h"
#include "LogInterface.h"
#include "IFileTransfer.h"
#include "transferSet.h"

class IClient;
class IMakePath;

class CollectHistory : public QObject
{
	Q_OBJECT
public:
	CollectHistory(ICtkPluginManager *pManager, QObject *pParent=NULL);
	virtual ~CollectHistory();

	/**
	 * @brief  启动收集
	 * @return int：启动成功返回true,失败返回false
	 */
	 int start();

	/**
	 * @brief  停止收集
	 * @return int：停止成功返回0，失败返回-1
	 */
	int stop();

	void setHistyTime(const QString &strStart, const QString &strEnd)
	{
		m_strStartTime = strStart;
		m_strEndTime = strEnd;
	}

	void setStations(const QList<QString> &strStations)
	{
		m_lstStations = strStations;
	}

	void setType(const int &type)
	{
		m_nType = type;
	}

	void setRootPath(const QString &strPath)
	{
		m_strPath = strPath;
	}

	/**
	 * @brief  设置目标消息队列名
	 * @param  const QString & name：队列名
	 * @return void：描述返回值
	 */
	void setQueName(const QString &name)
	{
		m_strQueName = name;
	}

	/**
	 * @brief  设置文件名匹配规则
	 * @param  const QVector<QString> & rules 匹配的文件名规则 [正则表达式]
	 * @return void：描述返回值
	 */
    void setFileNameRule(const QMap<QString, QVector<QString> > &rules)
	{
		m_TypeRegs = rules;
	}

public slots:
	void getHistoryFiles();

private:
	/**
	 * @brief  连接消息服务器
	 * @return bool：成功返回true
	 */
	bool connectMsgServer();

	/**
	 * @brief  判断是否为历史文件
	 * @param  FileInfo & fileInfo：指定的文件
	 * @return bool：历史文件返回true
	 */
	bool IsHistyFile(const FileInfo &fileInfo);

	void getAllFiles(FileInfoList &fileList, QString strPath);
	void getStationPath(QStringList &strlstPath);
	//void getFilePath(const QString &strRootPath, QStringList &strlstPath);
	void getFilePath(QSet<QString> &setPathes);

	/**
	 * @brief  简述函数功能
	 * @param  QString strName：文件名
	 * @return bool：匹配返回true
	 */
	bool filterFileName(QString strFilePath, FileInfo &fileInfo);

private:
	int m_nType;					///< 0-单站，1-组网，2-高空
	QString m_strPath;				///< 根路径
	QString m_strStartTime;			///< 起始时间
	QString m_strEndTime;			///< 终止时间
	QString m_strQueName;			///< 目标消息队列
	QList<QString> m_lstStations;	///< 组网站号
    QMap<QString, QVector<QString> > m_TypeRegs;	///< 类型和对应的正则表达式们

	QSharedPointer<QThread> m_pThread;
	ICtkPluginManager *m_pManager;
	QSharedPointer<IClient> m_pIClient;

	bool m_bRun;					///< 是否运行

	string m_strCurStation;

	QSharedPointer<IMakePath> m_pMakePath;	///< 路径适配器
};

#endif
