#include "systemset.h"
#include "baseset.h"

#include <QHBoxLayout>
CSystemSetDlg::CSystemSetDlg(QDialog *parent /*= NULL*/)
{
    ui.setupUi(this);
    QHBoxLayout *layout = new QHBoxLayout();
    CBaseSetDlg *othreadDlg = new CBaseSetDlg();
    layout->addWidget(othreadDlg);
    ui.tab->setLayout(layout);

    connect(othreadDlg, SIGNAL(wndquit()), this, SLOT(accept()));
    connect(othreadDlg, SIGNAL(wndcancel()), this, SLOT(reject()));
}

CSystemSetDlg::~CSystemSetDlg()
{

}

