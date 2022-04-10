#pragma once

#include <mutex>
#include <condition_variable>
#include <tuple>

#include "shared_obe.hpp"
#include "unique_txn.hpp"
#include "shared_txn.hpp"

/// \brief      Waits for modfication of `shared_obe` to meet the `predicate`.
/// \param      o               `shared_obe` to observe.
/// \param      predicate       Functor with signature `bool(const T & ... args)`; should return `true`
///                             if condition is met, `false` otherwise.
template<typename F, typename T>
void await_if(shared_obe<T> o, F && predicate)
{
    std::mutex mutex;
    std::condition_variable cv;
    bool awaited = false;

    auto observer = [&predicate, &mutex, &cv, &awaited](const T & value)
    {
        if (predicate(value))
        {
            // Critical section:
            {
                std::unique_lock lock(mutex);
                awaited = true;
            }

            cv.notify_one();
        }
    };

    o.observe(observer);

    // Critical section:
    {
        std::unique_lock lock(mutex);
        while (awaited == false)
        {
            cv.wait(lock);
        }
    }

    // JMTODO: could await_if return shared_txn to optionally keep the lock?
}

/// \brief      Waits for any modfication of `observable`.
template<typename T>
void await_any(const shared_obe<T> & o)
{
    auto predicate = [](const T & value)
    {
        return true;
    };

    await_if(o, std::move(predicate));
}

/// \brief      Waits for modfication of `shared_obe` to `value`.
template<typename T>
void await(const shared_obe<T> & o, const T & value)
{
    auto predicate = [&value](const T & modified_value)
    {
        return modified_value == value;
    };

    await_if(o, std::move(predicate));
}

/// \brief      Joins `shared_obe`s value as `functor`'s arguments.
template<typename F, typename A, typename B>
void join(F && functor, shared_obe<A> & a, shared_obe<B> & b)
{
    auto observer_a = [functor, b](const A & value_a) mutable
    {
        functor(value_a, shared_txn{b});
    };

    auto observer_b = [functor, a](const B & value_b) mutable
    {
        functor(shared_txn{a}, value_b);
    };

    a.observe(std::move(observer_a));
    b.observe(std::move(observer_b));
}

template<typename F, typename A, typename B, typename C>
void join(F && functor, shared_obe<A> & a, shared_obe<B> & b, shared_obe<C> & c)
{
    auto observer_a = [functor, b, c](const A & value_a) mutable
    {
        functor(value_a, shared_txn{b}, shared_txn{c});
    };

    auto observer_b = [functor, a, c](const B & value_b) mutable
    {
        functor(shared_txn{a}, value_b, shared_txn{c});
    };

    auto observer_c = [functor, a, b](const C & value_c) mutable
    {
        functor(shared_txn{a}, shared_txn{b}, value_c);
    };

    a.observe(std::move(observer_a));
    b.observe(std::move(observer_b));
    c.observe(std::move(observer_c));
}

/// \brief      Joins `shared_obe`s into composite.
/// \warning    Changes made on composite do not propagate into the original `shared_obe`s.
template<typename Ta, typename Tb>
auto join(shared_obe<Ta> & a, shared_obe<Tb> & b)
{
    shared_obe<std::tuple<Ta, Tb>> composite;

    auto observer_a = [composite, b](Ta value_a) mutable
    {
        Tb value_b = shared_txn{b};
        unique_txn{composite} = {std::move(value_a), std::move(value_b)};
    };

    auto observer_b = [composite, a](Tb value_b) mutable
    {
        Ta value_a = shared_txn{a};
        unique_txn{composite} = {std::move(value_a), std::move(value_b)};
    };

    a.observe(std::move(observer_a));
    b.observe(std::move(observer_b));

    return composite;
}