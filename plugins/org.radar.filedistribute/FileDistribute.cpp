#include "FileDistribute.h"
#include <QRegExp>
#include "ctkLog.h"
#include "FileDistributeThread.h"
#include "param.h"

//FileDistribute::FileDistribute(QWidget *parent)
FileDistribute::FileDistribute(void *parent)
    : m_parent(parent)
    , m_pUserManager(NULL)
    , m_pPrdcTrasferSet(NULL)
{
    m_bSendSignalToMainWindow = false;
    TransParam::getInstance()->init();	// 读取配置文件
}

FileDistribute::~FileDistribute()
{
    StopAllThreads();
}

FileDistribute::FileDistribute(const FileDistribute& rhs)
{
    this->m_parent = rhs.m_parent;
    this->m_threadList = rhs.m_threadList;
    m_bSendSignalToMainWindow = false;
}
FileDistribute& FileDistribute::operator=(const FileDistribute& rhs)
{
    this->m_parent = rhs.m_parent;
    this->m_threadList = rhs.m_threadList;
    m_bSendSignalToMainWindow = false;

    return *this;
}

void FileDistribute::getSendAcctInfo(const TransferSet& r_set , vector<SendAcctInfo>& r_vSendAcctInfo)
{
    for (unsigned int i = 0; i < r_set.vecRcvers.size(); i++)
    {
        SendAcctInfo t_info;
        t_info.m_strSendID = r_set.vecRcvers[i].strUsrname.c_str();
        t_info.m_strSendHost = r_set.vecRcvers[i].strIP.c_str();
        t_info.m_strSendUserName = r_set.vecRcvers[i].strUsr.c_str();
        t_info.m_strSendPasswd = r_set.vecRcvers[i].strPwd.c_str();
        t_info.m_strRltvPath = r_set.vecRcvers[i].strRltvPath.c_str();
        t_info.m_iSendPort = r_set.vecRcvers[i].nPort;
        t_info.m_iSendWay = r_set.vecRcvers[i].nSendWay;
        t_info.m_bEncrypt = r_set.vecRcvers[i].bEncrypt;
        t_info.m_bCompress = r_set.vecRcvers[i].bCompress;
        t_info.m_bKeepDir = r_set.vecRcvers[i].bKeepDir;
        t_info.m_bConSend = r_set.vecRcvers[i].bConSend;
        r_vSendAcctInfo.push_back(t_info);
    }
}

// void FileDistribute::AddTransferTask(const TransferSet &tSet)
// {
//     vector<TransFileInfo> t_vFileList;
//     getSendFileList(tSet, t_vFileList);
//     if (t_vFileList.size() == 0)
//     {
//         return;
//     }
//
//     QThreadPool::globalInstance()->setMaxThreadCount(5);
// }

void FileDistribute::getSendFileList(const TransferSet& r_set , vector<TransFileInfo>& r_vFileList)
{
    int iUserCount = r_set.vecRcvers.size();
    int iFileCount = r_set.fileList.size();
    for (int i = 0; i<iUserCount; i++)
    {
        for (int j = 0 ; j<iFileCount; j++)
        {
            if (fileMatch(r_set.fileList[j].strFileName,r_set.vecRcvers[i].vecRegs))
            {
                TransFileInfo t_info;
                t_info.m_strCollectID = QString::fromLocal8Bit(r_set.strName.c_str());
                t_info.m_iCollectWay = r_set.nCollectWay;
                t_info.m_iSendWay = r_set.vecRcvers[i].nSendWay;

                t_info.m_strFileName =  QString::fromLocal8Bit(r_set.fileList[j].strFileName.c_str());//r_set.fileList[j].strFileName.c_str();
                t_info.m_strCollectFileName = QString::fromLocal8Bit(r_set.fileList[j].strFilePath.c_str());//r_set.fileList[j].strFilePath.c_str();//full path
                //t_info.m_strSendID = QString::fromLocal8Bit(r_set.vecRcvers[i].strUsrname.c_str());
                t_info.m_strSendID = QString::fromStdString(r_set.vecRcvers[i].strUsrname);
                t_info.m_status = UnHandled;
                t_info.m_nSendFlag = r_set.sendFlag;
                r_vFileList.push_back(t_info);
            }

        }
    }
}
// bool FileDistribute::HasTask()
// {
//     QMap<string,FileDistributeThread>::iterator it = m_threadList.begin();
//     for (; it != m_threadList.end(); it++)
//     {
//         if (it->hasTask())
//         {
//             return true;
//         }
//     }
//     return false;
// }
void FileDistribute::SetSendMsg(bool r_bSend)
{
    m_bSendSignalToMainWindow = r_bSend;
    QMap<string,FileDistributeThread>::iterator it = m_threadList.begin();
    for (; it != m_threadList.end(); it++)
    {
        it->setSendMsg(m_bSendSignalToMainWindow);
    }
}
void FileDistribute::StopAllThreads()
{
    QMap<string,FileDistributeThread>::iterator it = m_threadList.begin();
    for (; it != m_threadList.end(); it++)
    {
        it->stop();
        //it->wait();
    }
    m_threadList.clear();
}
bool FileDistribute::fileMatch(const string&r_fileName,const vector<string>&r_vRegExp)
{
    return true;

    //int iCount = r_vRegExp.size();
    //if (0 == iCount)
    //	return true;

    //for (int i = 0 ; i < iCount ; i++)
    //{
    //	string t_regExp = r_vRegExp[i];
    //	QRegExp t_reg(t_regExp.c_str());
    //	t_reg.setPatternSyntax(QRegExp::Wildcard);
    //	if (t_reg.exactMatch(r_fileName.c_str()))
    //		return true;
    //}
    //return false;
}
// void FileDistribute::GetProcessResult(vector<TransFileInfo>& r_result)
// {
//     QMap<string,FileDistributeThread>::iterator it = m_threadList.begin();
//     for (; it != m_threadList.end(); it++)
//     {
//         it.value().getTransFileInfo(r_result);
//     }
// }

void FileDistribute::StopSendTask(const string &dirId)
{
    QMap<string,FileDistributeThread>::iterator it = m_threadList.find(dirId);
    if (it != m_threadList.end())
    {
        it->stop();
        m_threadList.erase(it);
    }
}

void FileDistribute::initgui(UIInterface *in_pGuiInterace)
{
    if (NULL == in_pGuiInterace)
    {
        QSLOG_ERROR("in_pGuiInterace is null.");
        return;
    }
    m_pGui = in_pGuiInterace;
    if (m_pUserManager == NULL)
    {
        m_pUserManager = new UserManagerDlg();
    }
    if (m_pPrdcTrasferSet == NULL)
    {
        m_pPrdcTrasferSet = new SendPrdcSetDlg(getCtkPluginManager());
    }
    QTreeWidgetItem *pParent = m_pGui->AppendItem(QStringLiteral("数据分发"));
    m_pGui->AppendItem(pParent, QStringLiteral("分发用户"), m_pUserManager);
    m_pGui->AppendItem(pParent, QStringLiteral("传输设置"), m_pPrdcTrasferSet);
}

QSharedPointer<IDistributeManager> FileDistribute::createDistributeManager()
{
    QSharedPointer<IDistributeManager> pDistributer = QSharedPointer<FileDistributeThread>(new FileDistributeThread(getCtkPluginManager()));
    return pDistributer;
}
