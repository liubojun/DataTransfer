#include "regexpression.h"
#include <QRegExp>
#include <QPalette>

CRegExpDlg::CRegExpDlg(QDialog *parent /*= 0*/)
{
    ui.setupUi(this);

    connect(ui.textEdit_input, SIGNAL(textChanged()), this, SLOT(valueChanged()));
    connect(ui.lineEdit_regexp, SIGNAL(textChanged(const QString &)), this, SLOT(valueChanged()));

}

void CRegExpDlg::valueChanged()
{
    QString orginalText = ui.textEdit_input->toPlainText();
    QString matchRegExp = ui.lineEdit_regexp->text();

    bool retFlag = false;
    if (matchRegExp.isEmpty())
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText, Qt::black);
        ui.label_valid->setPalette(pa);
        retFlag = true;
    }

    if (orginalText.isEmpty())
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText, Qt::black);
        ui.label_match->setPalette(pa);
        retFlag = true;
    }

    if (retFlag)
    {
        return;
    }

    QRegExp oReg(matchRegExp);

    if (oReg.isValid())
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText, Qt::green);
        ui.label_valid->setPalette(pa);
    }
    else
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText, Qt::red);
        ui.label_valid->setPalette(pa);
    }



    if (oReg.exactMatch(orginalText))
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText, Qt::green);
        ui.label_match->setPalette(pa);
    }
    else
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText, Qt::red);
        ui.label_match->setPalette(pa);
    }
}
