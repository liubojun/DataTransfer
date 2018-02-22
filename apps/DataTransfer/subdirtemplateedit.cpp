#include "subdirtemplateedit.h"
#include "ctkLog.h"

#include <QFile>
#include <QTextStream>

CSubDirTemplateUi::CSubDirTemplateUi(QDialog *parent /*= NULL*/) : QDialog(parent)
{
    ui.setupUi(this);

    initUi();
}

CSubDirTemplateUi::~CSubDirTemplateUi()
{

}

void CSubDirTemplateUi::initUi()
{
    connect(ui.pbt_ok, SIGNAL(clicked()), this, SLOT(onOk()));
    connect(ui.pbt_cancel, SIGNAL(clicked()), this, SLOT(reject()));


    QString rulefile = QCoreApplication::applicationDirPath() + "/config/subdirfilter.rule";

    QFile oFile(rulefile);
    if (!oFile.open(QIODevice::ReadOnly))
    {
        QSLOG_ERROR("open subdirfilter failure.");
        return;
    }

    QTextStream stream(&oFile);
    stream.setCodec("UTF-8");

    ui.textEdit_filecontent->setText(stream.readAll());
}

void CSubDirTemplateUi::onOk()
{
    QString rulefile = QCoreApplication::applicationDirPath() + "/config/subdirfilter.rule";

    QFile oFile(rulefile);
    if (!oFile.open(QIODevice::WriteOnly))
    {
        QSLOG_ERROR("open subdirfilter failure.");
        return;
    }

    QTextStream stream(&oFile);
    stream.setCodec("UTF-8");

    stream << ui.textEdit_filecontent->toPlainText();

    accept();
}
