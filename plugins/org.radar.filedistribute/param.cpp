#include "param.h"
#include "pathbuilder.h"
#include "ctkLog.h"


TransParam TransParam::s_param;
TransParam * TransParam::getInstance()
{
    return &s_param;
}

TransParam::TransParam()
{
    CPathBuilder pb;
    QString strConPath = pb.getConfigPath();
    //m_userInfoPath = strConPath + "/DistributeUserInfos.xml";
    m_userInfoPath = strConPath + "/prdctSendUserInfo.xml";
    m_transetPath = strConPath + "/CollectionDirInfos.xml";
    m_prdcTransetPath = strConPath + "/productSend.xml";
}

void TransParam::init()
{
    loadUserInfo();
    loadTranSet();
    loadPrdcTranSet();

    QList<string> allUserID;
    list<SendSet>::iterator it = m_userList.lsts.begin();
    while (it != m_userList.lsts.end())
    {
        allUserID.append(it->strUsrname);
        it++;
    }

//     bool bFlag = false;
//     list<TransferSet>::iterator jt = m_transetList.lsSets.begin();
//     while (jt != m_transetList.lsSets.end())
//     {
//         vector<string>::iterator kt = jt->vecRecvID.begin();
//         while (kt != jt->vecRecvID.end())
//         {
//             if (!allUserID.contains(*kt))
//             {
//                 kt = jt->vecRecvID.erase(kt);
//                 bFlag = true;
//             }
//             else
//             {
//                 kt++;
//             }
//         }
//         jt++;
//     }


    // 临时屏蔽，有问题
//     if (bFlag)
//     {
//         saveTranSet();
//     }
}

bool TransParam::loadUserInfo()
{
    m_userList.lsts.clear();
    if (!XmlToSendSetList(m_userInfoPath.toLocal8Bit().data(), m_userList) || m_userList.lsts.empty())
    {
        QSLOG_ERROR("Fail to loadUserInfo.");
        return false;
    }
    return true;
}

bool TransParam::saveUserInfo()
{
    if (!SendSetListToXml(m_userInfoPath.toLocal8Bit().data(), m_userList))
    {
        QSLOG_ERROR("Fail to saveUserInfo.");
        return false;
    }
    return true;
}

bool TransParam::loadTranSet()
{
    m_transetList.lsSets.clear();
    if (!XmlToTransferSet(m_transetPath.toLocal8Bit().data(), m_transetList))
    {
        QSLOG_ERROR("Fail to loadTranSet.");
        return false;
    }
    return true;
}

bool TransParam::saveTranSet()
{
    if (!TransferSetToXml(m_transetPath.toLocal8Bit().data(), m_transetList))
    {
        QSLOG_ERROR("Fail to saveTranSet.");
        return false;
    }
    return true;
}

bool TransParam::getUserInfo(string userId, SendSet &info)
{
    list<SendSet>::const_iterator it = m_userList.lsts.begin();
    for (; it != m_userList.lsts.end(); ++it)
    {
        if (it->strUsrname == userId)
        {
            info = *it;
            break;
        }
    }
    if (it == m_userList.lsts.end())
    {
        return false;
    }
    return true;
}

QMap<QString, QStringList> TransParam::getAllUserShowInfo()
{
    QMap<QString, QStringList> res;

    list<SendSet>::const_iterator it = m_userList.lsts.begin();
    while (it != m_userList.lsts.end())
    {
        QString strKey, strTmp;
        QStringList lstValue;
        // 用户id
        strKey = QString::fromStdString(it->strUsrname);
        // URL
        if (it->nSendWay == 0)
        {
            strTmp = QString::fromStdString(it->strRltvPath);
        }
        else
        {
            strTmp = QString("ftp://%1:%2%3").arg(QString::fromStdString(it->strIP)).arg(it->nPort).arg(QString::fromStdString(it->strRltvPath));
        }
        lstValue.append(strTmp);
        // 原目录结构
        if (it->bKeepDir)
        {
            strTmp = QStringLiteral("是");
        }
        else
        {
            strTmp = QStringLiteral("否");
        }
        lstValue.append(strTmp);
        // 断点续传
        if (it->bConSend)
        {
            strTmp = QStringLiteral("是");
        }
        else
        {
            strTmp = QStringLiteral("否");
        }
        lstValue.append(strTmp);
        // 加密
        if (it->bEncrypt)
        {
            strTmp = QStringLiteral("是");
        }
        else
        {
            strTmp = QStringLiteral("否");
        }
        lstValue.append(strTmp);
        // 压缩
        if (it->bCompress)
        {
            strTmp = QStringLiteral("是");
        }
        else
        {
            strTmp = QStringLiteral("否");
        }
        lstValue.append(strTmp);
        res.insert(strKey, lstValue);
        it++;
    }

    return res;
}

QVector<string> TransParam::getSelectUserID(const QString &taskID)
{
    QVector<string> res;
    list<TransferSet>::const_iterator it = m_transetList.lsSets.begin();
    while (it != m_transetList.lsSets.end())
    {
        if (it->strName == taskID.toLocal8Bit().data())
        {
            res = QVector<string>::fromStdVector(it->vecRecvID);
            break;
        }
        it++;
    }
    return res;
}

bool TransParam::loadPrdcTranSet()
{
//     m_prdcTransetList.lst.clear();
//     if (!XmlToPrdcTransferSet(m_prdcTransetPath.toLocal8Bit().data(), m_prdcTransetList))
//     {
//         QSLOG_ERROR("Fail to loadPrdcTranSet.");
//         return false;
//     }
    return true;
}

bool TransParam::savePrdcTranSet()
{
    if (!PrdcTransferSetToXml(m_prdcTransetPath.toLocal8Bit().data(), m_prdcTransetList))
    {
        QSLOG_ERROR("Fail to savePrdcTranSet.");
        return false;
    }
    return true;
}
