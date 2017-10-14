#include "toVtbThread.h"
#include "IRadarBaseDataParse.h"
#include "ctkPublicFun.h"
#include "CollectorBase.h"

ToVtbTask::ToVtbTask(CollectorBase *pParent, IRadarBaseDataParse *pPaser, FileInfo fileInfo)
{
    m_pParent = pParent;
    m_fileInfo = fileInfo;
    m_pBaseParser = pPaser;

    setAutoDelete(true);
}

ToVtbTask::~ToVtbTask()
{
    QSLOG_DEBUG("ToVtbTask Deconstruct");
}

void ToVtbTask::run()
{
    bool bFlag = true;
    QString strVtbPath, strFileTime, strStation;
    strStation = QString::fromStdString(m_fileInfo.strStation);
    //QString strFilePath = QString::fromStdString(m_fileInfo.strFilePath);
    QString strFilePath = QString::fromLocal8Bit(m_fileInfo.strFilePath.c_str());	//解决string转QString中文乱码
    QString strType = QString::fromStdString(m_fileInfo.strFileType);
    m_fileInfo.strOldType = m_fileInfo.strFileType;
    m_pParent->showMsg(QStringLiteral("[%1] 开始vtb标准化...").arg(strFilePath));
    if (m_pBaseParser->ChangeToVtb(strFilePath, strType, strVtbPath, strFileTime, strStation))
    {
        m_fileInfo.strOrgFilePath = m_fileInfo.strFilePath;
        m_fileInfo.strFilePath = strVtbPath.toStdString();
        m_fileInfo.strFileName = strVtbPath.right(strVtbPath.length() - strVtbPath.lastIndexOf("/") - 1).toStdString();
        m_fileInfo.strFileTime = strFileTime.toStdString();
        m_fileInfo.strFileType = "VTB_NEW";
        if (m_fileInfo.strStation.empty())
        {
            m_fileInfo.strStation = strStation.toStdString();
        }
        getRadarFileType(m_fileInfo.strFileType, m_fileInfo.nFileType);

        m_pParent->showMsg(QStringLiteral("[%1] 完成vtb标准化。").arg(strFilePath));
    }
    else
    {
        bFlag = false;
        m_pParent->showMsg(QStringLiteral("[%1] 标准化失败！").arg(strFilePath));
    }

    m_pParent->taskDone(bFlag, m_fileInfo);
}
