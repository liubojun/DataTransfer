#include "listItemWidget.h"
#include <QDateTime>


MyItemWidget::MyItemWidget(const ICONTYPE &icon, const QString &name, QString state)
    : m_nFinish(0)
{
    ui.setupUi(this);
    // 设置无边框
    setWindowFlags(Qt::FramelessWindowHint);

    SetIcon(icon);
    SetName(name);

    //ui.leIcon->show();
    if (state.isEmpty())
    {
        QString strTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        state = QStringLiteral("完成0个，%1/%2 %3:%4 启动").arg(strTime.mid(4,2)).arg(strTime.mid(6,2))
                .arg(strTime.mid(8,2)).arg(strTime.mid(10,2));
    }
    ui.label_2->setText(state);

    // gif动画
    m_pMovie = new QMovie(":/working.gif");
    m_pMovie->setSpeed(50);
    ui.le_gif->setMovie(m_pMovie);
    //m_pStatusWnd = new CStatusLabel();
    //QVBoxLayout *pLayout = new QVBoxLayout();
    //ui.le_gif->setLayout(pLayout);
    //pLayout->addWidget(m_pStatusWnd);
}

MyItemWidget::~MyItemWidget()
{

}

void MyItemWidget::SetState(const QString &state, bool bShow)
{

}

void MyItemWidget::SetHighlight(bool bFlag)
{

}

void MyItemWidget::SetIcon(int colType, int sendType, int enable, int normal)
{

}

void MyItemWidget::SetIcon(const ICONTYPE &icon)
{
    QPixmap pix;
    switch (icon)
    {
    case FILE_FILE:
        pix = QPixmap(QString(":/fileTofile.png"));
        break;
    case FILE_FTP:
        pix = QPixmap(QString(":/fileToftp.png"));
        break;
    case FTP_FILE:
        pix = QPixmap(QString(":/ftpTofile.png"));
        break;
    case FTP_FTP:
        pix = QPixmap(QString(":/ftpToftp.png"));
        break;
    case FILE_FILE_S:
        pix = QPixmap(QString(":/fileTofile2.png"));
        break;
    case FILE_FTP_S:
        pix = QPixmap(QString(":/fileToftp2.png"));
        break;
    case FTP_FILE_S:
        pix = QPixmap(QString(":/ftpTofile2.png"));
        break;
    case FTP_FTP_S:
        pix = QPixmap(QString(":/ftpToftp2.png"));
        break;
    case FILE_FILE_E:
        pix = QPixmap(QString(":/fileTofile3.png"));
        break;
    case FILE_FTP_E:
        pix = QPixmap(QString(":/fileToftp3.png"));
        break;
    case FTP_FILE_E:
        pix = QPixmap(QString(":/ftpTofile3.png"));
        break;
    case FTP_FTP_E:
        pix = QPixmap(QString(":/ftpToftp3.png"));
        break;
    case FILE_FILE_SE:
        pix = QPixmap(QString(":/fileTofile4.png"));
        break;
    case FILE_FTP_SE:
        pix = QPixmap(QString(":/fileToftp4.png"));
        break;
    case FTP_FILE_SE:
        pix = QPixmap(QString(":/ftpTofile4.png"));
        break;
    case FTP_FTP_SE:
        pix = QPixmap(QString(":/ftpToftp4.png"));
        break;
    default:
        break;
    }
    ui.leIcon->setPixmap(pix);

    m_curIcon = icon;
}

void MyItemWidget::SetName(const QString &strName)
{
    ui.label->setText(strName);
}

void MyItemWidget::ShowGif(bool bFlag)
{
    if (bFlag)
    {
        ui.le_gif->show();
        //m_pStatusWnd->start();
        m_pMovie->start();
    }
    else
    {
        ui.le_gif->hide();
        //m_pStatusWnd->stop();
        m_pMovie->stop();
    }
}

void MyItemWidget::SetEnable(bool bFlag)
{
    if (bFlag)
    {
        m_curIcon = (ICONTYPE)(m_curIcon + 4);
    }
    else
    {
        m_curIcon = (ICONTYPE)(m_curIcon - 4);
    }
    SetIcon(m_curIcon);
}

void MyItemWidget::AddSuccess(const QDateTime &dt)
{
    m_nFinish++;

    QString strTime =  dt.toString("yyyyMMddhhmmss");
    QString state = QStringLiteral("完成%5个，最近：%1/%2 %3:%4").arg(strTime.mid(4,2)).arg(strTime.mid(6,2))
                    .arg(strTime.mid(8,2)).arg(strTime.mid(10,2)).arg(m_nFinish);
    ui.label_2->setText(state);
}
