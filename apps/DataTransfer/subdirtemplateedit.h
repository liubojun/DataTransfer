#ifndef subdirtemplateedit_h__
#define subdirtemplateedit_h__

#include <QDialog>
#include "ui_subdirtemplateedit.h"

class CSubDirTemplateUi : public QDialog
{
    Q_OBJECT
public:
    CSubDirTemplateUi(QDialog *parent = NULL);

    virtual ~CSubDirTemplateUi();

protected:
    void initUi();

protected slots:
    void onOk();

private:
    Ui::subdirtemplateedit ui;
};
#endif // subdirtemplateedit_h__
