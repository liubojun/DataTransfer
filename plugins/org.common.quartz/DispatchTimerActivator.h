#ifndef DispatchTimerActivator_h__
#define DispatchTimerActivator_h__

#include <ctkPluginActivator.h>

// 数据服务插件激活类,通过该类向上下文中注册各种服务

class IDispatchTimer;

class DispatchTimerActivator : public QObject, public ctkPluginActivator
{
	Q_OBJECT
	Q_INTERFACES(ctkPluginActivator)
#ifdef WITH_QT5
	Q_PLUGIN_METADATA(IID "org_common_Quartz")
#endif
public:

	//************************************
	// Method:    DispatchTimerActivator
	// FullName:  DispatchTimerActivator::DispatchTimerActivator
	// Access:    public 
	// Returns:   
	// Qualifier: 默认构造函数
	//************************************
	DispatchTimerActivator();

	//************************************
	// Method:    ~DispatchTimerActivator
	// FullName:  DispatchTimerActivator::~DispatchTimerActivator
	// Access:    virtual public 
	// Returns:   
	// Qualifier: 默认析构函数
	//************************************
	virtual ~DispatchTimerActivator();
	
	//************************************
	// Method:    start
	// FullName:  DispatchTimerActivator::start
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: 启动数据服务，将当前插件需要向外部暴露的服务注册到ctk插件上下文中
	// Parameter: ctkPluginContext * context ctk上下文
	//************************************
	virtual void start(ctkPluginContext *context);

	//************************************
	// Method:    stop
	// FullName:  DispatchTimerActivator::stop
	// Access:    virtual public 
	// Returns:   void
	// Qualifier: 停止数据服务，将当前插件需要向外部暴露的服务从上下文中移除
	// Parameter: ctkPluginContext * context
	//************************************
	virtual void stop(ctkPluginContext *context);

private:
	///////////////////// 对外暴露服务,由服务中提供对外的方法////////////////////////
	IDispatchTimer *m_pDispatchTimer;
	//////////////////////////////////////////////////////////
};

#endif // DispatchTimerActivator_h__