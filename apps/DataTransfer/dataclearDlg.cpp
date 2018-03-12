#include "dataclearDlg.h"
#include "DataBase.h"
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

DataClearDlg::DataClearDlg(QDialog *parent /*= NULL*/) : QDialog(parent)
{
    ui.setupUi(this);
    InitUI();
}

DataClearDlg::DataClearDlg(const QString &taskname, QDialog *parent /*= NULL*/)
{
    ui.setupUi(this);
    InitUI(taskname);
}

void DataClearDlg::InitUI()
{

    connect(ui.pushButton_open, SIGNAL(pressed()), this, SLOT(openDirOpen()));

    connect(ui.pushButton_confirm, SIGNAL(pressed()), this, SLOT(onApply()));
    connect(ui.pushButton_cancel, SIGNAL(pressed()), this, SLOT(reject()));
    connect(ui.comboBox_unit, SIGNAL(currentIndexChanged(int )), this, SLOT(onComBoxIndexChanged(int)));

    //ui.lineEdit_name->setDisabled(true);
    //ui.lineEdit_dir->setDisabled(true);
    //ui.lineEdit_rule->setDisabled(true);
    //ui.lineEdit_file->setDisabled(true);
    //ui.comboBox_unit->setDisabled(true);
    //ui.comboBox_value->setDisabled(true);

    //QList<ClearTask> tasks;
    //DataBase::getInstance()->queryClearTask(tasks);
    onAdded();

}

void DataClearDlg::InitUI(const QString &taskName)
{
    ClearTask task;
    task.taskName = taskName;
    if (!DataBase::getInstance()->queryClearTask(task))
    {
        return;
    }

    connect(ui.pushButton_open, SIGNAL(pressed()), this, SLOT(openDirOpen()));
    connect(ui.pushButton_confirm, SIGNAL(pressed()), this, SLOT(onApply2()));
    connect(ui.pushButton_cancel, SIGNAL(pressed()), this, SLOT(reject()));
    connect(ui.comboBox_unit, SIGNAL(currentIndexChanged(int )), this, SLOT(onComBoxIndexChanged(int)));

    ui.lineEdit_name->setText(task.taskName);
    ui.lineEdit_rule->setText(task.quartzRule);
    ui.lineEdit_file->setText(task.matchRule);
    ui.lineEdit_dir->setText(task.taskDir);
    ui.comboBox_unit->setCurrentIndex(task.clearUnit);
    onComBoxIndexChanged(task.clearUnit);
    //ui.comboBox_value->addItem(QString::number(task.clearValue));
    ui.comboBox_value->setCurrentText(QString::number(task.clearValue));
    ui.checkBox_active->setChecked(task.activeFlag);

}

void DataClearDlg::openDirOpen()
{
    QString strDir;
    if (!ui.lineEdit_dir->text().isEmpty())
    {
        strDir = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("设置清除目录"), ui.lineEdit_dir->text());
    }
    else
    {
        strDir = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("设置清除目录"));

    }

    if (!strDir.isEmpty())
    {
        ui.lineEdit_dir->setText(strDir);
    }
}

void DataClearDlg::onApply()
{
    // 检查是否所有内容都填充完毕
    if (ui.lineEdit_dir->text().isEmpty() || ui.lineEdit_file->text().isEmpty() || ui.lineEdit_name->text().isEmpty() || ui.lineEdit_rule->text().isEmpty())
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("参数检查"), QString::fromLocal8Bit("参数未正确配置"));
        return;
    }

    // 查表，判断表中是否存在相同的名称或者目录

    // 根据name查表，并将表中的信息返回到界面上
    QList<ClearTask> tasks;
    if (!DataBase::getInstance()->queryClearTask(tasks))
    {
        return;
    }
    foreach (const ClearTask &task, tasks)
    {
        if (task.taskName == ui.lineEdit_name->text() || QFileInfo(task.taskDir).absoluteFilePath() == QFileInfo(ui.lineEdit_dir->text()).absoluteFilePath())
        {
            QMessageBox::warning(this, QString::fromLocal8Bit("参数检查"), QString::fromLocal8Bit("存在相同的任务"));
            return;
        }
    }

    ClearTask task;
    task.taskName = ui.lineEdit_name->text();
    task.taskDir = QFileInfo(ui.lineEdit_dir->text()).absoluteFilePath();
    task.quartzRule = ui.lineEdit_rule->text();
    task.matchRule = ui.lineEdit_file->text();
    task.clearUnit = ui.comboBox_unit->currentIndex();
    task.clearValue = ui.comboBox_value->currentText().toInt();
    task.activeFlag = ui.checkBox_active->isChecked();
    DataBase::getInstance()->insertClearTask(task);

    accept();
    emit newTaskCreated(task);
}


void DataClearDlg::onItemChanged(const QString &item)
{
    QString name = item;
    ClearTask task;
    task.taskName = item;
    if (!DataBase::getInstance()->queryClearTask(task))
    {
        return;
    }

    ui.lineEdit_name->setText(task.taskName);
    ui.lineEdit_rule->setText(task.quartzRule);
    ui.lineEdit_file->setText(task.matchRule);
    ui.lineEdit_dir->setText(task.taskDir);
    ui.comboBox_unit->setCurrentIndex(task.clearUnit);
    ui.comboBox_value->setCurrentIndex(task.clearValue);

    if (item.isEmpty())
    {
        ui.lineEdit_name->setDisabled(true);
        ui.lineEdit_dir->setDisabled(true);
        ui.lineEdit_rule->setDisabled(true);
        ui.lineEdit_file->setDisabled(true);
        ui.comboBox_unit->setDisabled(true);
        ui.comboBox_value->setDisabled(true);

        ui.lineEdit_name->setText("");
        ui.lineEdit_rule->setText("");
        ui.lineEdit_file->setText("");
        ui.lineEdit_dir->setText("");
        ui.comboBox_unit->setCurrentIndex(2);
    }

}

void DataClearDlg::onComBoxIndexChanged(int index)
{
    // 0:分钟， 1:小时, 2：天 3: 月
    int step = 1;
    QStringList values;

    switch (index)
    {
    case 0:
        step = 60;
        values.append("10");
        values.append("20");
        values.append("30");
        values.append("40");
        values.append("50");
        values.append("60");
        ui.comboBox_value->clear();
        ui.comboBox_value->addItems(values);
        break;
    case 1:
        values.append("1");
        values.append("2");
        values.append("3");
        values.append("6");
        values.append("12");
        values.append("24");
        step = 60*60;
        ui.comboBox_value->clear();
        ui.comboBox_value->addItems(values);
        break;
    case 2:
        values.append("1");
        values.append("2");
        values.append("3");
        values.append("4");
        values.append("5");
        values.append("6");
        values.append("7");
        values.append("10");
        values.append("20");
        values.append("30");
        step = 24*60*60;
        ui.comboBox_value->clear();
        ui.comboBox_value->addItems(values);
        break;
    case 3:
        values.append("1");
        values.append("2");
        values.append("3");
        values.append("4");
        values.append("5");
        values.append("6");
        values.append("7");
        values.append("8");
        values.append("9");
        values.append("10");
        values.append("11");
        values.append("12");
        step = 30*24*60*60;
        ui.comboBox_value->clear();
        ui.comboBox_value->addItems(values);
        break;
    default:
        step = 1;
        break;
    }

}

void DataClearDlg::onAdded()
{
    ui.lineEdit_name->setDisabled(false);
    ui.lineEdit_dir->setDisabled(false);
    ui.lineEdit_rule->setDisabled(false);
    ui.lineEdit_file->setDisabled(false);
    ui.comboBox_unit->setDisabled(false);
    ui.comboBox_value->setDisabled(false);
    ui.lineEdit_name->setText(QString::fromLocal8Bit("目录清理-") + QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    ui.lineEdit_rule->setText("0 0 * * * *");
    ui.lineEdit_file->setText(".*");
    ui.lineEdit_dir->setText("");
    ui.comboBox_unit->setCurrentIndex(2);
    ui.checkBox_active->setChecked(true);
}

int DataClearDlg::computeSeconds()
{
    int index = ui.comboBox_unit->currentIndex();
    QString vaule = ui.comboBox_value->currentText();

    // 0:分钟， 1:小时, 2：天 3: 月
    int step = 1;

    switch (index)
    {
    case 0:
        step = 60;
        break;
    case 1:
        step = 60*60;
        break;
    case 2:
        step = 24*60*60;
        break;
    case 3:
        step = 30*24*60*60;
        break;
    default:
        step = 1;
        break;
    }

    return vaule.toInt() * step;
}

void DataClearDlg::onDelete()
{
    // 数据库删除
    //DataBase::getInstance()->deleteClearTask(ui.listWidget->currentItem()->text());

}

void DataClearDlg::onApply2()
{
    // 检查是否所有内容都填充完毕
    if (ui.lineEdit_dir->text().isEmpty() || ui.lineEdit_file->text().isEmpty() || ui.lineEdit_name->text().isEmpty() || ui.lineEdit_rule->text().isEmpty())
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("参数检查"), QString::fromLocal8Bit("参数未正确配置"));
        return;
    }

    //// 查表，判断表中是否存在相同的名称或者目录

    //// 根据name查表，并将表中的信息返回到界面上
    //QList<ClearTask> tasks;
    //if (!DataBase::getInstance()->queryClearTask(tasks))
    //{
    //    return;
    //}
    //foreach (const ClearTask &task, tasks)
    //{
    //    if (task.taskName == ui.lineEdit_name->text() || QFileInfo(task.taskDir).absoluteFilePath() == QFileInfo(ui.lineEdit_dir->text()).absoluteFilePath())
    //    {
    //        return;
    //    }
    //}

    //ClearTask task;
    m_task.taskName = ui.lineEdit_name->text();
    m_task.taskDir = QFileInfo(ui.lineEdit_dir->text()).absoluteFilePath();
    m_task.quartzRule = ui.lineEdit_rule->text();
    m_task.matchRule = ui.lineEdit_file->text();
    m_task.clearUnit = ui.comboBox_unit->currentIndex();
    m_task.clearValue = ui.comboBox_value->currentText().toInt();
    m_task.activeFlag = ui.checkBox_active->isChecked();
    //DataBase::getInstance()->insertClearTask(task);

    accept();
    //emit newTaskCreated(task);
}
