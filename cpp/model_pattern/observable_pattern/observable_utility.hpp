#pragma once

#include <mutex>
#include <condition_variable>
#include <initializer_list>
#include <tuple>

#include "observable.hpp"

/// \brief      Waits for modfication of `observable` to meet the `predicate`.
/// \param      predicate       Functor with signature `bool(const T & value)`; should return `true`
///                             if condition is met, `false` otherwise.
template<typename T, typename F>
void await_if(const observable<T> & o, F && predicate)
{
    std::condition_variable cv;

    auto wait = [&](const T & value)
    {
        if (predicate(value))
        {
            cv.notify_one();
        }
    };

    auto guard = o.observe(wait);

    // Critical section:
    {
        std::mutex mutex;
        std::unique_lock lock(mutex);
        cv.wait(lock);
    }
}

/// \brief      Waits for any modfication of `observable`.
template<typename T>
void await_any(const observable<T> & o)
{
    auto predicate = [](const T &)
    {
        return true;
    };

    await_if(o, std::move(predicate));
}

/// \brief      Waits for modfication of `observable` to `value`.
template<typename T>
void await(const observable<T> & o, const T & awaited_value)
{
    auto predicate = [&](const T & value)
    {
        return value == awaited_value;
    };

    await_if(o, std::move(predicate));
}

/// \brief      Waits for modfication to one of `awaited_list` values of `observable`.
template<typename T>
void await(observable<T> & o, std::initializer_list<T> awaited_list)
{
    auto predicate = [&](const T & value)
    {
        for (const auto & awaited_value : awaited_list)
        {
            if (value == awaited_value)
            {
                return true;
            }
        }

        return false;
    };

    await_if(o, std::move(predicate));
}

/// \brief      Joins `observable` values `a` and `b` into single `callback`.
/// \param      a               First observable.
/// \param      b               Second observable.
/// \param      callback        Functor with signature `void(const Ta &, const Tb &)`.
/// \returns    `tuple` of subscription guards.
///
/// Invokes `callback` upon modification of either `observable` `a` or `b`. One value is
/// propagated through the notification, the other is read under lock from the other
/// `observable`, as if `observable::get()` was called.
template<typename F, typename Ta, typename Tb>
auto join(F && callback, const observable<Ta> & a, const observable<Tb> & b)
{
    auto observer_a = [&](const Ta & value_a)
    {
        callback({value_a}, {b.get()});
    };

    auto observer_b = [&](const Tb & value_b)
    {
        callback({a.get()}, {value_b});
    };

    // JMTODO: a.get(), b.get() is not good, the reference might be dangling
    // and we are accessing deleted observable
    //  - maybe somehow limit this by cb guard active() state? 
    //  - what is the expected semantics of composite observable outliving 
    //    the underlaying ones?

    auto guard_a = a.observe(std::move(observer_a));
    auto guard_b = b.observe(std::move(observer_b));

    return std::make_tuple(std::move(guard_a), std::move(guard_b));
}

/*
template<typename Ta, typename Tb>
auto join(shared_observable_v2<Ta> & a, shared_observable_v2<Tb> & b)
{
    shared_observable_v2<Ta, Tb> composite;

    auto observer_a = [composite, b](const Ta & value)
    {
        composite.m_sp->m_store.invoke(value, b.m_sp->get());
        // JMTODO: composite.set(value, b.get());
    };

    auto observer_b = [composite, a](const Tb & value)
    {
        composite.m_sp->m_store.invoke(a.m_sp->get(), value);
    };

    a.m_guard = a.m_sp->observe(std::move(observer_a));
    b.m_guard = b.m_sp->observe(std::move(observer_b));

    // JMTODO: this wastes the shared_observable built-in guard for the joining and therefore
    // makes it not usable for custom observation; also it is most likely not possible to use
    // single shared observable in multiple joins

    return composite;
}
*/