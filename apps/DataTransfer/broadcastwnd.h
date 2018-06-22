#ifndef broadcastwnd_h__
#define broadcastwnd_h__

#include <QDialog>
#include <QUdpSocket>

#include "ui_broadcastwnd.h"

class CBroadCastWnd : public QDialog
{
    Q_OBJECT
public:
    CBroadCastWnd(QDialog *parent = NULL);

    virtual ~CBroadCastWnd();

protected slots:
    void rcvMsg();

private:
    Ui::BroadCastWnd ui;

    QUdpSocket m_oSocket;

    int m_oBindPort;
};
#endif // broadcastwnd_h__
