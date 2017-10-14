#include "pictureSetDlg.h"
#include "pathbuilder.h"
#include "ctkLog.h"
#include "event_macro.h"
#include <QSettings>
#include <QVBoxLayout>
#include <QColorDialog>


PageWidget::PageWidget(QWidget *parent/*=NULL*/) : QWidget(parent)
{
    QLabel *label1 = new QLabel(QStringLiteral("是否启用："));
    m_ckbEnable = new QCheckBox;
    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->addWidget(label1);
    hlayout1->addWidget(m_ckbEnable);
    hlayout1->addStretch();

    QLabel *label2 = new QLabel(QStringLiteral("宽度(像素)："));
    QLabel *label3 = new QLabel(QStringLiteral("高度(像素)："));
    m_leWidth = new QLineEdit;
    m_leHeight = new QLineEdit;
    m_leWidth->setMaximumWidth(70);
    m_leHeight->setMaximumWidth(70);
    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->addWidget(label2);
    hlayout2->addWidget(m_leWidth);
    hlayout2->addStretch();
    hlayout2->addWidget(label3);
    hlayout2->addWidget(m_leHeight);
    hlayout2->addStretch();

    QLabel *label4 = new QLabel(QStringLiteral("是否画距离圈："));
    m_ckbDrawCircle = new QCheckBox;
    QHBoxLayout *hlayout3 = new QHBoxLayout;
    hlayout3->addWidget(label4);
    hlayout3->addWidget(m_ckbDrawCircle);
    hlayout3->addStretch();

    QLabel *label5 = new QLabel(QStringLiteral("是否标注距离："));
    m_ckbDrawDistc = new QCheckBox;
    QHBoxLayout *hlayout4 = new QHBoxLayout;
    hlayout4->addWidget(label5);
    hlayout4->addWidget(m_ckbDrawDistc);
    hlayout4->addStretch();

    QLabel *label6 = new QLabel(QStringLiteral("是否透明处理："));
    m_ckbTransp = new QCheckBox;
    QHBoxLayout *hlayout5 = new QHBoxLayout;
    hlayout5->addWidget(label6);
    hlayout5->addWidget(m_ckbTransp);
    hlayout5->addStretch();

    QLabel *label7 = new QLabel(QStringLiteral("字体大小："));
    m_sbFont = new QSpinBox;
    QHBoxLayout *hlayout6 = new QHBoxLayout;
    hlayout6->addWidget(label7);
    hlayout6->addWidget(m_sbFont);
    hlayout6->addStretch();

    QLabel *label8 = new QLabel(QStringLiteral("背景颜色："));
    QLabel *label9 = new QLabel(QStringLiteral("R："));
    QLabel *label10 = new QLabel(QStringLiteral("G："));
    QLabel *label11 = new QLabel(QStringLiteral("B："));
    label9->setFixedWidth(15);
    label10->setFixedWidth(15);
    label11->setFixedWidth(15);
    m_btnColor = new QPushButton;
    m_sbRed = new QSpinBox;
    m_sbGreen = new QSpinBox;
    m_sbBlue = new QSpinBox;
    m_sbRed->setRange(0, 255);
    m_sbGreen->setRange(0, 255);
    m_sbBlue->setRange(0, 255);
    m_btnColor->setMinimumWidth(60);
    m_btnColor->setMinimumHeight(28);
    QHBoxLayout *hlayout7 = new QHBoxLayout;
    hlayout7->addWidget(label8);
    hlayout7->addWidget(m_btnColor);
    hlayout7->addSpacing(20);
    hlayout7->addWidget(label9);
    hlayout7->addWidget(m_sbRed);
    hlayout7->addSpacing(10);
    hlayout7->addWidget(label10);
    hlayout7->addWidget(m_sbGreen);
    hlayout7->addSpacing(10);
    hlayout7->addWidget(label11);
    hlayout7->addWidget(m_sbBlue);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout1);
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout2);
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout3);
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout4);
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout5);
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout6);
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout7);
    vlayout->addSpacing(20);

    this->setLayout(vlayout);

    connect(m_btnColor, SIGNAL(clicked()), this, SLOT(onBtnColor()));
}

void PageWidget::refreshDlg(const Picture &picinfo)
{
    m_ckbEnable->setChecked(picinfo.enable);
    m_ckbDrawCircle->setChecked((bool)picinfo.drawCircle);
    m_ckbDrawDistc->setChecked((bool)picinfo.drawDistance);
    m_ckbTransp->setChecked((bool)picinfo.transparent);
    m_leWidth->setText(QString::number(picinfo.width));
    m_leHeight->setText(QString::number(picinfo.height));
    m_sbFont->setValue(picinfo.fontSize);
    m_sbRed->setValue(picinfo.backColor[0]);
    m_sbGreen->setValue(picinfo.backColor[0]);
    m_sbBlue->setValue(picinfo.backColor[0]);
    QString strColorStyle = QString("rgb(%1,%2,%3)").arg(picinfo.backColor[0]).arg(picinfo.backColor[1]).arg(picinfo.backColor[2]);
    m_btnColor->setStyleSheet("background-color:"+strColorStyle);
}

void PageWidget::saveSet(Picture &picinfo)
{
    picinfo.enable = m_ckbEnable->isChecked();
    picinfo.drawCircle = m_ckbDrawCircle->isChecked();
    picinfo.drawDistance = m_ckbDrawDistc->isChecked();
    picinfo.transparent = m_ckbTransp->isChecked();
    picinfo.width = m_leWidth->text().toInt();
    picinfo.height = m_leHeight->text().toInt();
    picinfo.fontSize = m_sbFont->value();
    picinfo.backColor[0] = m_sbRed->value();
    picinfo.backColor[1] = m_sbGreen->value();
    picinfo.backColor[2] = m_sbBlue->value();
}

void PageWidget::onBtnColor()
{
    QColorDialog *pColorDlg = new QColorDialog(Qt::white, this);
    if (pColorDlg->exec() != QDialog::Accepted)
    {
        return;
    }
    QColor color = pColorDlg->selectedColor();
    int r = color.red();
    int g = color.green();
    int b = color.blue();
    QString strStyle = QString("background-color:rgb(%1,%2,%3)").arg(r).arg(g).arg(b);
    m_btnColor->setStyleSheet(strStyle);
    m_sbRed->setValue(r);
    m_sbGreen->setValue(g);
    m_sbBlue->setValue(b);
}

//////////////////////////////////////////////////////////////////////////

PictureSetDlg::PictureSetDlg(QWidget *parent/*=NULL*/)
    : m_qFont("楷体_GB2312", 14)
{
    ui.setupUi(this);
    loadPicInfo();
//     connect(ui.btn_Apply, SIGNAL(clicked()), this, SLOT(saveSets()));
//     connect(ui.btn_Ok, SIGNAL(clicked()), this, SLOT(onBtnOk()));
//     connect(ui.btn_Cancel, SIGNAL(clicked()), this, SLOT(onBtnCancel()));
//     ui.btn_Apply->setVisible(false);
//     ui.btn_Ok->setVisible(false);
//     ui.btn_Cancel->setVisible(false);
}

PictureSetDlg::~PictureSetDlg()
{

}

bool PictureSetDlg::event(QEvent *e)
{
    if (e->type() == ApplyEvent)
    {
        saveSets();
        return true;
    }
    else if (e->type() == OkEvent)
    {
        onBtnOk();
        return true;
    }
    else if (e->type() == CancelEvent)
    {
        onBtnCancel();
        return true;
    }

    return QObject::event(e);
}

void PictureSetDlg::loadPicInfo()
{
    // 初始化界面
    //QTabWidget *tabWidget = new QTabWidget(this);
    QTabWidget *tabWidget = ui.tabWidget;
    tabWidget->clear();
    tabWidget->setFont(m_qFont);
    tabWidget->setTabPosition(QTabWidget::North);
    tabWidget->setTabShape(QTabWidget::Triangular);

    m_vecPicture.clear();
    CPathBuilder pb;
    m_strConPath = pb.getConfigPath() + "/PictureInfo.ini";
    QString strTmp;
    // QSettings::IniFormat表明m_strConPath为配置文件路径
    QSettings picIni(m_strConPath, QSettings::IniFormat);
    strTmp = picIni.value("PICTURE/Resolution").toString();
    QStringList lstSize = strTmp.split("|");
    strTmp = picIni.value("PICTURE/FLAG").toString();
    QStringList lstFlag = strTmp.split("|");
    for (int i=0; i<lstSize.size() && i<lstFlag.size(); ++i)
    {
        Picture picInfo;
        picInfo.enable = false;
        strcpy(picInfo.type, lstSize.at(i).toStdString().c_str());
        if (lstFlag.at(i) == "1")
        {
            QSLOG_INFO(lstSize.at(i) + " is true.");
            picInfo.enable = true;
        }
        // 读取具体设置
        picInfo.width = picIni.value(lstSize.at(i)+"/Width").toInt();
        picInfo.height = picIni.value(lstSize.at(i)+"/Height").toInt();
        picInfo.fontSize = picIni.value(lstSize.at(i)+"/FontSize").toInt();
        picInfo.drawCircle = picIni.value(lstSize.at(i)+"/DrawCircle").toInt();
        picInfo.drawDistance = picIni.value(lstSize.at(i)+"/DrawDistance").toInt();
        picInfo.drawMap = picIni.value(lstSize.at(i)+"/DrawMap").toInt();
        picInfo.drawInfo = picIni.value(lstSize.at(i)+"/DrawInfo").toInt();
        picInfo.drawColor = picIni.value(lstSize.at(i)+"/DrawColor").toInt();
        picInfo.transparent = picIni.value(lstSize.at(i)+"/Transparent").toInt();
        QStringList lstColor = picIni.value(lstSize.at(i)+"/BackColor").toString().split("|");
        if (lstColor.size() == 3)
        {
            picInfo.backColor[0] = lstColor[0].toInt();
            picInfo.backColor[1] = lstColor[0].toInt();
            picInfo.backColor[2] = lstColor[0].toInt();
        }
        m_vecPicture.push_back(picInfo);
    }

    for (int i=0; i<m_vecPicture.size(); ++i)
    {
        QString strSize(m_vecPicture[i].type);
        PageWidget *widGet = new PageWidget(this);
        widGet->refreshDlg(m_vecPicture.at(i));
        tabWidget->addTab(widGet, strSize);
        m_vecPageSet.push_back(widGet);
    }
}

void PictureSetDlg::refreshDlg(int nIndex)
{
//     if (nIndex < 0 || nIndex >= m_vecPicture.size())
//     {
//         return;
//     }
//     const Picture &pic = m_vecPicture.at(nIndex);
//     ui.checkBox_enable->setChecked(pic.enable);
//     ui.lineEdit_width->setText(QString::number(pic.width));
//     ui.lineEdit_height->setText(QString::number(pic.height));
//     ui.checkBox_dscircle->setChecked((bool)pic.drawCircle);
//     ui.checkBox_distance->setChecked((bool)pic.drawDistance);
//     ui.checkBox_transp->setChecked((bool)pic.transparent);
//     ui.spinBox_font->setValue(pic.fontSize);
//     ui.spinBox_red->setValue(pic.backColor[0]);
//     ui.spinBox_green->setValue(pic.backColor[1]);
//     ui.spinBox_blue->setValue(pic.backColor[2]);
//     QString strColorStyle = QString("rgb(%1,%2,%3)").arg(pic.backColor[0]).arg(pic.backColor[1]).arg(pic.backColor[2]);
//     ui.btn_color->setStyleSheet("background-color:"+strColorStyle);
}

QWidget * PictureSetDlg::createTabPage(int nIndex)
{
    if (nIndex < 0 || nIndex >= m_vecPicture.size())
    {
        return NULL;
    }
    const Picture &pic = m_vecPicture.at(nIndex);

    QLabel *label1 = new QLabel(QStringLiteral("是否启用："));
    QCheckBox *ckbEnable = new QCheckBox;
    ckbEnable->setChecked(pic.enable);
    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->addWidget(label1);
    hlayout1->addWidget(ckbEnable);
    hlayout1->addStretch();

    QLabel *label2 = new QLabel(QStringLiteral("宽度(像素)："));
    QLabel *label3 = new QLabel(QStringLiteral("高度(像素)："));
    QLineEdit *leWidth = new QLineEdit;
    leWidth->setText(QString::number(pic.width));
    leWidth->setMaximumWidth(70);
    QLineEdit *leHeight = new QLineEdit;
    leHeight->setText(QString::number(pic.height));
    leHeight->setMaximumWidth(70);
    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->addWidget(label2);
    hlayout2->addWidget(leWidth);
    hlayout2->addStretch();
    hlayout2->addWidget(label3);
    hlayout2->addWidget(leHeight);
    hlayout2->addStretch();

    QLabel *label4 = new QLabel(QStringLiteral("是否画距离圈："));
    QCheckBox *ckbDrawCircle = new QCheckBox;
    ckbDrawCircle->setChecked((bool)pic.drawCircle);
    QHBoxLayout *hlayout3 = new QHBoxLayout;
    hlayout3->addWidget(label4);
    hlayout3->addWidget(ckbDrawCircle);
    hlayout3->addStretch();

    QLabel *label5 = new QLabel(QStringLiteral("是否标注距离："));
    QCheckBox *ckbDrawDist = new QCheckBox;
    ckbDrawDist->setChecked((bool)pic.drawDistance);
    QHBoxLayout *hlayout4 = new QHBoxLayout;
    hlayout4->addWidget(label5);
    hlayout4->addWidget(ckbDrawDist);
    hlayout4->addStretch();

    QLabel *label6 = new QLabel(QStringLiteral("是否透明处理："));
    QCheckBox *ckbTransp = new QCheckBox;
    ckbTransp->setChecked((bool)pic.transparent);
    QHBoxLayout *hlayout5 = new QHBoxLayout;
    hlayout5->addWidget(label6);
    hlayout5->addWidget(ckbTransp);
    hlayout5->addStretch();

    QLabel *label7 = new QLabel(QStringLiteral("字体大小："));
    QSpinBox *cmbFont = new QSpinBox;
    cmbFont->setValue(pic.fontSize);
    QHBoxLayout *hlayout6 = new QHBoxLayout;
    hlayout6->addWidget(label7);
    hlayout6->addWidget(cmbFont);
    hlayout6->addStretch();

    QLabel *label8 = new QLabel(QStringLiteral("背景颜色："));
    QLabel *label9 = new QLabel(QStringLiteral("R："));
    QLabel *label10 = new QLabel(QStringLiteral("G："));
    QLabel *label11 = new QLabel(QStringLiteral("B："));
    label9->setFixedWidth(15);
    label10->setFixedWidth(15);
    label11->setFixedWidth(15);
    QPushButton *btnColor = new QPushButton;
    QString strColorStyle = QString("rgb(%1,%2,%3)").arg(pic.backColor[0]).arg(pic.backColor[1]).arg(pic.backColor[2]);
    btnColor->setStyleSheet("background-color:"+strColorStyle);
    btnColor->setMinimumWidth(60);
    btnColor->setMinimumHeight(28);
    QSpinBox *sbR = new QSpinBox;
    QSpinBox *sbG = new QSpinBox;
    QSpinBox *sbB = new QSpinBox;
    sbR->setValue(pic.backColor[0]);
    sbG->setValue(pic.backColor[1]);
    sbB->setValue(pic.backColor[2]);
    QHBoxLayout *hlayout7 = new QHBoxLayout;
    hlayout7->addWidget(label8);
    hlayout7->addWidget(btnColor);
    hlayout7->addSpacing(20);
    hlayout7->addWidget(label9);
    hlayout7->addWidget(sbR);
    hlayout7->addSpacing(10);
    hlayout7->addWidget(label10);
    hlayout7->addWidget(sbG);
    hlayout7->addSpacing(10);
    hlayout7->addWidget(label11);
    hlayout7->addWidget(sbB);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout1);
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout2);
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout3);
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout4);
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout5);
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout6);
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout7);
    vlayout->addSpacing(20);
    QWidget *widget = new QWidget;
    widget->setLayout(vlayout);

    return widget;
}

void PictureSetDlg::saveSets()
{
    QStringList lstFlag;
    for (int i=0; i<m_vecPageSet.size() && i<m_vecPicture.size(); ++i)
    {
        m_vecPageSet[i]->saveSet(m_vecPicture[i]);
        lstFlag.append(m_vecPicture[i].enable ? "1" : "0");
    }

    // 保存到配置文件
    QString strTmp;
    QSettings picIni(m_strConPath, QSettings::IniFormat);
    strTmp = lstFlag.join("|");
    picIni.setValue("PICTURE/FLAG", strTmp);
    for (int i=0; i<m_vecPicture.size(); ++i)
    {
        const Picture &pic = m_vecPicture.at(i);
        QString strSize(pic.type);
        picIni.beginGroup(strSize);
        picIni.setValue("Width", pic.width);
        picIni.setValue("Height", pic.height);
        picIni.setValue("FontSize", pic.fontSize);
        picIni.setValue("DrawCircle", pic.drawCircle);
        picIni.setValue("DrawDistance", pic.drawDistance);
        picIni.setValue("Transparent", pic.transparent);
        strTmp = QString("%1|%2|%3").arg(pic.backColor[0]).arg(pic.backColor[1]).arg(pic.backColor[2]);
        picIni.setValue("BackColor", strTmp);
        picIni.endGroup();
    }
}

void PictureSetDlg::onBtnCancel()
{
    emit close();
    emit cancel();
}

void PictureSetDlg::onBtnOk()
{
    saveSets();
    emit close();
    emit ok();
}
