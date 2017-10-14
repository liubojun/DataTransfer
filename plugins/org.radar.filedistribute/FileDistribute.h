#ifndef  __FILEDISTRIBUTE_H_20160518_
#define  __FILEDISTRIBUTE_H_20160518_

//#include <QWidget>
#include <QList>
#include <string>
#include <QMap>
#include <QString>
#include "transferSet.h"
#include "IFileDistribute.h"
#include "FileDistributeThread.h"
#include "IFileCrypt.h"
#include "userManageDlg.h"
//#include "transferSetDlg.h"
#include "sendPrdcSetDlg.h"

class FileDistributeThread;

#ifndef WITH_QT5
#define QStringLiteral QString
#endif

class FileDistribute : public IFileDistribute
{
    Q_OBJECT
    Q_INTERFACES(IFileDistribute)
    Q_INTERFACES(IBaseInterface)
public:
    //FileDistribute(QWidget *parent=0);
    FileDistribute(void *parent=0);
    ~FileDistribute();

    /**
    * @brief  创建数据分发管家
    * @return QSharedPointer<FileDistributeThread>：实例指针
    */
    virtual QSharedPointer<IDistributeManager> createDistributeManager();

    //////////////////////////////////////////////////////////////////////////

    FileDistribute(const FileDistribute& rhs);
    FileDistribute& operator=(const FileDistribute& rhs);
public:
    //virtual void SetParent(QWidget *parent,bool sendMsg)

    void SetSendMsg(bool r_bSend);

    /**
     * @brief  添加分发任务（被前台调用）
     * @param  const TransferSet & r_set：一个收集目录对应的设置及新文件列表
     * @return void：无
     */
    //virtual void AddSendTask(const TransferSet & r_set);

    //virtual void AddTransferTask(const TransferSet &tSet);

    /**
    * @brief  停止一个分发任务
    * @param  const string & dirId：收集目录ID
    * @return void：无
    */
    virtual void StopSendTask(const string &dirId);

    //virtual bool HasTask();
    void StopAllThreads();
    //virtual void GetProcessResult(vector<TransFileInfo>& r_result);

    /**
    * @brief  插件中界面初始化方法
    * @param  UIInterface * in_pGuiInterace 主窗体抽象出的接口
    * @return void：无
    */
    virtual void initgui(UIInterface *in_pGuiInterace);

private:
    void getSendAcctInfo(const TransferSet& r_set , vector<SendAcctInfo>& r_vSendAcctInfo);
    void getSendFileList(const TransferSet& r_set , vector<TransFileInfo>& r_vFileList);
    bool fileMatch(const string&r_fileName,const vector<string>&r_vRegExp);

private:
    //QWidget *m_parent;
    void *m_parent;
    volatile bool m_bSendSignalToMainWindow;
    /// 一个收集目录对应一个分发线程
    QMap<string, FileDistributeThread> m_threadList;

//     IFileCrypt *m_pFileCrypt;
//     /**
//      * @brief  获取加密服务的指针
//      * @return void：无
//      */
//     void getFileCrypt();

    UIInterface *m_pGui;							///< gui界面线程对象
    UserManagerDlg *m_pUserManager;
    //TransferSetDlg *m_pTrasferSet;
    SendPrdcSetDlg	*m_pPrdcTrasferSet;
};

#endif