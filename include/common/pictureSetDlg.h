#ifndef pictureSetDlg_H_
#define pictureSetDlg_H_

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include "ui_radarpicture.h"
#include "commontypedef.h"

class PageWidget : public QWidget
{
    Q_OBJECT
public:
    PageWidget(QWidget *parent=NULL);
    ~PageWidget() {}
    void refreshDlg(const Picture &picinfo);
    void saveSet(Picture &picinfo);
private slots:
    void onBtnColor();
public:
    QCheckBox	*m_ckbEnable;
    QCheckBox	*m_ckbDrawCircle;
    QCheckBox	*m_ckbDrawDistc;
    QCheckBox	*m_ckbTransp;
    QLineEdit	*m_leWidth;
    QLineEdit	*m_leHeight;
    QSpinBox	*m_sbFont;
    QSpinBox	*m_sbRed;
    QSpinBox	*m_sbGreen;
    QSpinBox	*m_sbBlue;
    QPushButton	*m_btnColor;
};

class PictureSetDlg : public QWidget
{
    Q_OBJECT
public:
    PictureSetDlg(QWidget *parent=NULL);
    ~PictureSetDlg();
signals:
    void close();
    void ok();
    void cancel();
private slots:
    void saveSets();
    void onBtnOk();
    void onBtnCancel();
protected:
    virtual bool event(QEvent *e);
private:
    void loadPicInfo();
    void refreshDlg(int nIndex);
    QWidget *createTabPage(int nIndex);
private:
    Ui::radarPicture ui;
    QFont	m_qFont;

    QVector<Picture> m_vecPicture;		///< 图片规格信息（定义为全局?）
    QString m_strConPath;
    QVector<PageWidget*> m_vecPageSet;
};

#endif
