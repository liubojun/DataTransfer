#include "collectSetDlg.h"
#include "pathbuilder.h"
#include "ctkLog.h"
#include <QFileDialog>

CollectSetDlg::CollectSetDlg(QWidget *parent/*=NULL*/)
{
    ui.setupUi(this);

    connect(ui.btn_sel, SIGNAL(clicked()), this, SLOT(onBtnSelectPath()));
}

CollectSetDlg::~CollectSetDlg()
{

}

void CollectSetDlg::onBtnSelectPath()
{
    // 打开当前所在的文件夹
    QString strPath = QFileDialog::getExistingDirectory(this, "选择文件夹", ui.lineEdit->text(), QFileDialog::ShowDirsOnly);
    if (!strPath.isEmpty())
    {
        ui.lineEdit->setText(strPath);
        ui.lineEdit->setCursorPosition(-1);
    }
}

void CollectSetDlg::loadCollects()
{
    CPathBuilder pd;
    m_strConPath = pd.getConfigPath() + "/CollectSet.xml";
    if (!XmlToCollectSet(m_strConPath.toLocal8Bit().toStdString(), m_lstCollects))
    {
        QSLOG_ERROR("Fail to loadCollects.");
        return;
    }
}
