#ifndef _H_TRANSFILEINFO_H_
#define _H_TRANSFILEINFO_H_

#include <QString>
#include <QBuffer>
#include <QFile>
#include <string>
#include <QSharedPointer>
#include <string>
using namespace std;

enum TaskState
{
    UnHandled = 0,		///< 未处理
    BeHandled,			///< 正在处理
    Failed,				///< 处理失败
    Finished			///< 处理成功
};

/**
  * @class <TransFileInfo>
  * @brief  分发任务类
  *
  *  一个收集任务有多少个分发用户，就有多少个TransFileInfo
  * @note	这里是注意事项
  * @see 	这里是参考类名
 */
class TransFileInfo
{
public:
    TransFileInfo():m_getDone(0),m_putDone(0),m_totalSize(0)
    {
        m_status = UnHandled;
    }

    QString m_strCollectID;			///< 收集唯一用户标识
    int     m_iCollectWay;			///< 收集方式（0-目录，1-FTP）
    int     m_iSendWay;				///< 发布方式（0-目录，1-FTP）
    QString m_strFileName;			///< 无路径的文件名
    QString m_strCollectFileName;	///< 完整路径的文件名
    QString m_strSendID;			///< 发送唯一用户标识
    //----进度条相关----
    TaskState m_status;				///< 0-未处理，1-正在处理，2-处理失败，3-处理成功
    QString m_strStartDateTime;		///<开始处理时间
    QString m_strEndDateTime;		///<结束处理时间
    QString m_errorInfo;			///<错误信息
    volatile long    m_getDone;		///< 已经get的bytes数
    volatile long    m_putDone;		///< 已经put的bytes数
    volatile long    m_totalSize;	///< 总大小（字节）
    int	m_nSpeed;					///< 下载速度（字节/秒）
    int		m_nSendFlag;			///< 传输标识

    //////////////////////////////////////////////////////////////////////////
    string	m_strCollectIP;
    int		m_iCollectPort;
    string	m_strCollectUser;
    string	m_strCollectPwd;
    QList<QString> m_strUsers;		///< 分发用户ID集合
    QString m_strCollectPath;		///< 收集根路径

    //重载==
    bool operator==(const TransFileInfo& info)
    {
        if ( this->m_strSendID == info.m_strSendID &&
                this->m_strCollectFileName == info.m_strCollectFileName )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};


#endif // _H_TRANSFILEINFO_H_
