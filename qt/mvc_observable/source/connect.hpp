#pragma once

#include <tuple>
#include <utility>

#include <QtCore/QObject>

#include "shared_observable.hpp"

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

    observable.observe(observer);
}

template<typename T, typename ... A>
void connect(T * object_ptr, void (T:: * signal_ptr)(A ...), const shared_observable<A ...> & observable)
{
    auto functor = [observable](A ... args)
    {
        unique_txn{observable} = std::tuple<A ...>{args ...};
    };

    QObject::connect(object_ptr, signal_ptr, functor);
}