#ifndef sendPrdcSetDlg_H_
#define sendPrdcSetDlg_H_


#include <QWidget>
#include <QButtonGroup>
#include "ui_sendProductSet.h"
#include "param.h"
#include "IMakePath.h"

class ICtkPluginManager;
class IFileDistribute;
class IDistributeManager;
class ICollectManager;
class IDataCollection;

class SendPrdcSetDlg : public QWidget
{
    Q_OBJECT
public:
    explicit SendPrdcSetDlg(ICtkPluginManager *pManager, QWidget *parent=0);
    ~SendPrdcSetDlg() {}

    void initDlg();
    void showInfo(const PrdcTransferSet &set);
    void getUserID();
    void clearAllInfo();

    void startTask(const PrdcTransferSet &set);
    void stopTask(const QString &strID);

    QVector<TransferSet> changeToTransferSet(const PrdcTransferSet &set);

protected:
    /**
     * @brief  显示事件
     * @param  QShowEvent * e
     * @return void：无
     */
    virtual void showEvent(QShowEvent *e);

    virtual bool event(QEvent *e);

signals:
    void ok();
    void cancel();

private slots:
    void onChangeTask(int rowNo);
    void onApplyTask();
    void onSelUser();
    void onBtnAddTask();
    void onBtnDelTask();
    void TransFile(const TransferSet &tset);

private:
    Ui::sendProductSet ui;
    QVector<QCheckBox *> m_vecCheckBox;
    TransParam *m_pParam;
    QStringList m_lstUserID;
    int m_nTaskFlag;			// 0-初始值，1-新增，2-修改
    PrdcTransferSet m_tranSet;
    ICtkPluginManager *m_pManager;

    QSharedPointer<QThread> m_pThread;

    QSharedPointer<IMakePath> m_pMakePath;	///< 路径适配器

    QSharedPointer<IDistributeManager> m_pDistributer;

    //QMap<QString, QVector<QSharedPointer<CollectorBase>> > m_mapCollects;

    QMap<QString, QVector<string> > m_mapCollectID;

    IDataCollection *m_pDataCollectionSev;
    QSharedPointer<ICollectManager> m_pCollector;

    IFileDistribute *m_pFileTransfer;
};


#endif