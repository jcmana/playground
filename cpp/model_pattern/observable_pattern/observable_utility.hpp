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

/// \brief      Waits for modfication to `value` of `observable`.
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
/// \param      callback        Functor with signature `void(const Ta &, const Tb &)`.
template<typename Ta, typename Tb, typename F>
auto join(observable<Ta> & a, observable<Tb> & b, F && callback)
{
    auto observer_a = [&](const Ta & value_a)
    {
        callback({value_a}, {b.get()});
    };

    auto observer_b = [&](const Tb & value_b)
    {
        callback({a.get()}, {value_b});
    };

    auto guard_a = a.observe(std::move(observer_a));
    auto guard_b = b.observe(std::move(observer_b));

    return std::make_tuple(std::move(guard_a), std::move(guard_b));
}