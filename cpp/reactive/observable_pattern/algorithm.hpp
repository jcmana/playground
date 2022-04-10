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
template<typename F, typename ... A>
void await_if(shared_obe<A ...> o, F && predicate)
{
    std::mutex mutex;
    std::condition_variable cv;
    bool awaited = false;

    auto observer = [&predicate, &mutex, &cv, &awaited](const A & ... args)
    {
        if (predicate(args ...))
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
template<typename ... A>
void await_any(const shared_obe<A ...> & o)
{
    auto predicate = [](const A & ... args)
    {
        return true;
    };

    await_if(o, std::move(predicate));
}

/// \brief      Waits for modfication of `shared_obe` to `value`.
template<typename ... A>
void await(const shared_obe<A ...> & o, const A & ... values)
{
    std::tuple awaited_values{values ...};

    auto predicate = [&awaited_values](const A & ... values)
    {
        return std::tuple{values ...} == awaited_values;
    };

    await_if(o, std::move(predicate));
}

/// \brief      Joins `shared_obe`s value as `functor`'s arguments.
template<typename F, typename Ta, typename Tb>
void join(F && functor, shared_obe<Ta> & a, shared_obe<Tb> & b)
{
    auto observer_a = [functor, b](auto ... args) mutable
    {
        std::tuple<Ta> args_a = {args ...};
        std::tuple<Tb> args_b = shared_txn{b};

        std::apply(functor, std::tuple_cat(args_a, args_b));
    };

    auto observer_b = [functor, a](auto ... args) mutable
    {
        std::tuple<Ta> args_a = shared_txn{a};
        std::tuple<Tb> args_b = {args ...};

        std::apply(functor, std::tuple_cat(args_a, args_b));
    };

    a.observe(std::move(observer_a));
    b.observe(std::move(observer_b));
}

template<typename F, typename Ta, typename Tb, typename Tc>
void join(F && functor, shared_obe<Ta> & a, shared_obe<Tb> & b, shared_obe<Tc> & c)
{
    auto observer_a = [functor, b, c](auto ... args) mutable
    {
        std::tuple<Ta> args_a = {args ...};
        std::tuple<Tb> args_b = b;
        std::tuple<Tc> args_c = c;

        std::apply(functor, std::tuple_cat(args_a, args_b, args_c));
    };

    auto observer_b = [functor, a, c](auto ... args) mutable
    {
        std::tuple<Ta> args_a = a;
        std::tuple<Tb> args_b = {args ...};
        std::tuple<Tc> args_c = c;

        std::apply(functor, std::tuple_cat(args_a, args_b, args_c));
    };

    auto observer_c = [functor, a, b](auto ... args) mutable
    {
        std::tuple<Ta> args_a = shared_txn{a};
        std::tuple<Tb> args_b = shared_txn{b};
        std::tuple<Tc> args_c = {args ...};

        std::apply(functor, std::tuple_cat(args_a, args_b, args_c));
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