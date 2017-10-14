#ifndef objectfactory_h__
#define objectfactory_h__

#include <QByteArray>
#include <QMetaObject>
#include <QHash>
#include <QSharedPointer>
#include "macro.h"

/**
 * @class <>
 * @brief 反射机制工厂类
 *
 * 这里是类的详细描述
 * @note 这里是注意事项
 */

class DLL_EXPORT_CLASS_DECL ObjectFactory
{
public:

    /**
     * @brief 注册类
     * @return void：无
     */
    template<typename T>
    static void registerClass()
    {
        constructors().insert(T::staticMetaObject.className(),&constructorHelper<T>);
    }

    /**
     * @brief 创建对象
     * @param const QByteArray & className 对象名
     * @param QObject * parent 父类
     * @return QSharedPointer<QObject>：新创建的对象实例
     */
    static QSharedPointer<QObject> createObject(const QByteArray& className,QObject* parent = NULL)
    {
        Constructor constructor = constructors().value(className);
        if (constructor == NULL)
            return QSharedPointer<QObject>();
        return (*constructor)(parent);

    }
private:
    typedef QSharedPointer<QObject> (*Constructor)(QObject* parent);

    template<typename T>
    static QSharedPointer<QObject> constructorHelper(QObject* parent)
    {
        return QSharedPointer<QObject>(new T());
    }

    static QHash<QByteArray,Constructor>& constructors()
    {
        static QHash<QByteArray,Constructor> instance;
        return instance;
    }
};
#endif // objectfactory_h__
