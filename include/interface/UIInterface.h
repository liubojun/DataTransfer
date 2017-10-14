/**
  * @file   UIInterface.h
  * @brief  UI接口.
  *
  *  提供往UI中插入菜单，工具栏等功能接口
  * @author 刘伯俊
  * @par   Copyright (c):
  *          刘伯俊.
  *         All Rights Reserved
  * @date   2016:07:04
  * @note   这里是注意事项。
  * @version <版本号>
  */

#ifndef UIInterface_h__
#define UIInterface_h__

#include <QObject>
#include <QAction>
#include <QWidget>
#include <QMdiSubWindow>
#include <QTreeWidgetItem>

class UIInterface : public QObject
{
    Q_OBJECT

public:
    virtual ~UIInterface() {}

public:

    /**
     * @brief  获取窗体的示例指针
     * @return QWidget *：描述返回值
     */
    virtual QWidget *getWidget() = 0;

    /**
     * @brief  往菜单栏添加菜单项
     * @param  const QString m_idenitty 菜单Action名称
     * @param  QAction * qAction 对应的动作
     * @return int：添加成功，返回0.失败返回-1
     */
    virtual int addAction( const QString m_idenitty, QAction *qAction ) = 0;

    //往归档菜单
    virtual void addActToArchive(QAction *qAction ) = 0;

    //往控制菜单(服务)
    virtual void addActToControl(QAction *qAction, bool bToolBar = false ) = 0;

    //往查看菜单(日志窗口)
    virtual void addActToView(QAction *qAction ) = 0;

    //往通信菜单里添加(通信插件)
    virtual void addToCommunication(QAction *pAction) = 0;

    /**
     * @brief  向多文档窗体中添加子窗体
     * @param  QWidget * pWnd
     * @return void：描述返回值
     */
    virtual void addSubWnd(QWidget *pWnd) = 0;

    /**
     * @brief  移除多文档子窗体
     * @param  const QString & title 窗体标题
     * @return
     */
    virtual void removeSubWnd(const QString &title) = 0;

    virtual QTreeWidgetItem* AppendItem(const QString& sItem) = 0 ;

    virtual void AppendItem(const QString& sItem, QWidget *widget) = 0;

    virtual void AppendItem(QTreeWidgetItem *pParent, const QString& sItem, QWidget *widget) = 0;

    virtual QTreeWidgetItem* InserItem(int index, const QString& sItem) = 0 ;

    virtual void InsertItem(int index, const QString& sItem, QWidget *widget) = 0;

    virtual void InsertItem(QTreeWidgetItem *pParent, int index, const QString& sItem, QWidget *widget) =0;

    virtual void AddMessageGroup(const QString& sGroupName, const QStringList& slabelList) = 0 ;

    virtual void AddMessageGroupEx(const QString& sGroupName, const QStringList& slabelList, const QStringList& sValueList) = 0 ;

    virtual void AddThreadGroup(const QString& sGroupName, const QStringList& slabelList, const QStringList& sValueList) = 0;

    virtual void showSubWnd(const QString &title) = 0 ;

signals:
    void save(const QString&, const QStringList&);
    void saveThread(const QString&, const QStringList&);
};
#endif // UIInterface_h__
