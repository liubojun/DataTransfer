#include "status.h"
#include <QPainter>

CStatusLabel::CStatusLabel()
{
    m_bIsStart = false;
    m_bLastGreen = false;

    m_oTimer.setInterval(500);
    connect(&m_oTimer, SIGNAL(timeout()), this, SLOT(update()));
}

CStatusLabel::~CStatusLabel()
{

}

void CStatusLabel::paintEvent(QPaintEvent *event)
{
    if (m_bIsStart)
    {
        // 通过标识位m_bLastGreen记录上次的状态，两种状态不断切换
        if (!m_bLastGreen)
        {
            QPainter painter(this);
            painter.setBrush(Qt::green);
            painter.drawEllipse(QPoint(this->geometry().center().x()-10, this->geometry().center().y()-8), 6, 6);
            m_bLastGreen = true;
        }
        else
        {
            m_bLastGreen = false;
        }

    }
    else
    {
        m_bLastGreen = false;
    }

}

void CStatusLabel::start()
{
    m_bIsStart = true;
    m_bLastGreen = false;
    // 启动定时器
    m_oTimer.start();
}

void CStatusLabel::stop()
{
    m_bIsStart = false;
    //停止定时器
    m_oTimer.stop();
}

