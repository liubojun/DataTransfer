#ifndef cviewHeader_H_
#define cviewHeader_H_

#include <QWidget>
#include <QHeaderView>
#include <QPainter>
#include <QMouseEvent>

class CViewHeader : public QHeaderView
{
    Q_OBJECT
public:
    CViewHeader(int ckbColumnIndex, Qt::Orientation orientation, QWidget *parent=0);
    virtual ~CViewHeader() {}
    void setChecked(bool bFlag);
protected:
    virtual void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    virtual void mousePressEvent(QMouseEvent *e);
signals:
    void clickRow(bool);
public slots:
    void setHeaderBox(bool bState);
private:
    bool m_isChecked;
    int m_style;
    int m_ckbColNo;		///< 复选框所在的列数
};

#endif