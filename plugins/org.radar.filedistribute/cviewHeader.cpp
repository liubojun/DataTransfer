#include "cviewHeader.h"

CViewHeader::CViewHeader(int ckbColumnIndex, Qt::Orientation orientation, QWidget *parent/*=0*/)
    : QHeaderView(orientation, parent)
{
    m_isChecked = false;
    m_ckbColNo = ckbColumnIndex;
    m_style = QStyle::State_Off;
}

void CViewHeader::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->restore();
    if (logicalIndex == m_ckbColNo)
    {
        QStyleOptionButton RectOption;
        RectOption.rect = QRect(5, 2, 20, 20);
        if (m_isChecked)
        {
            RectOption.state = QStyle::State_On;
        }
        else
        {
            RectOption.state = QStyle::State_Off;
        }

        this->style()->drawControl(QStyle::CE_CheckBox, &RectOption, painter);
    }
}

void CViewHeader::mousePressEvent(QMouseEvent *e)
{
    if (m_ckbColNo == visualIndexAt(e->pos().x()))
    {
        m_isChecked = !m_isChecked;
        emit clickRow(m_isChecked);
        updateSection(m_ckbColNo);
    }

    QHeaderView::mousePressEvent(e);
}

void CViewHeader::setHeaderBox(bool bState)
{
    if (!bState)
    {
        m_style = QStyle::State_Off;
    }
}

void CViewHeader::setChecked(bool bFlag)
{
    m_isChecked = bFlag;
    updateSection(m_ckbColNo);
}
