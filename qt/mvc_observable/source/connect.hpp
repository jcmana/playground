#pragma once

#include <tuple>
#include <utility>
#include <functional>

#include <QtCore/QObject>

#include "shared_observable.hpp"

template<typename F>
class QObjectScopeGuard :
    public QObject
{
public:
    QObjectScopeGuard(F && functor) :
        m_functor(std::move(functor))
    {
    }

    ~QObjectScopeGuard()
    {
        m_functor();
    }

private:
    F m_functor;
};

template<typename T, typename ... A>
void connect(shared_observable<A ...> & observable, T * object_ptr, void (T:: * slot_ptr)(A ...))
{
    auto observer = [object_ptr, slot_ptr](A ... args)
    {
        auto args_copy = std::tuple{args ...};

        auto functor = [object_ptr, slot_ptr, args_copy]
        {
            auto application = [object_ptr, slot_ptr](auto ... args)
            {
                (*object_ptr.*slot_ptr)(std::forward<A>(args) ...);
            };

            std::apply(application, args_copy);
        };

        auto connected = QMetaObject::invokeMethod(object_ptr, functor);
    };

    auto guard = observable.observe_scoped(observer);

    auto scope_guard_ptr = new QObjectScopeGuard([g = std::move(guard)]{ });
    scope_guard_ptr->setParent(object_ptr);
}

template<typename T, typename ... A>
void connect(T * object_ptr, void (T:: * signal_ptr)(A ...), const shared_observable<A ...> & observable)
{
    auto functor = [observable](A ... args)
    {
        unique_txn{observable} = std::tuple<A ...>{args ...};
    };

    QObject::connect(object_ptr, signal_ptr, functor);

    auto scope_guard_ptr = new QObjectScopeGuard([observable]{ });
    scope_guard_ptr->setParent(object_ptr);
}