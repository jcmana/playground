#pragma once

#include <shared_mutex>
#include <functional>
#include <tuple>

#include "../../callback_store/atomic_callback_store/atomic_callback_store.hpp"

/// <summary> Common base for observable pattern implementation.</summary>
template<template <typename> typename F, typename ... A>
class basic_observable
{
public:
    using value_type = std::tuple<A ...>;
    using store_callback_type = F<A ...>;
    using store_type = atomic_callback_store<store_callback_type>;
    using guard_type = atomic_callback_guard<store_callback_type>;

public:
    /// \brief      Default contructor.
    basic_observable() :
        m_value()
    {
    }

    basic_observable(A ... args) :
        m_value(std::forward<A>(args) ...)
    {
    }

    template<std::size_t I>
    typename std::tuple_element<I, value_type>::type & get() noexcept
    {
        return std::get<I>(m_value);
    }

    template<std::size_t I>
    const typename std::tuple_element<I, value_type>::type & cget() noexcept
    {
        return m_value;
    }

    /// \brief      Subscribes `callback` for modification notifications.
    auto observe(store_callback_type callback) const
    {
        return m_store.subscribe(std::move(callback));
    }

    /// \brief      Invokes each subscribed callback.
    void notify() const
    {
        auto application = [this](auto ... args)
        {
            m_store.invoke(args ...);
        };

        std::apply(std::move(application), m_value);
    }

public:
    // SharedMutex implementation:
    void lock() const
    {
        m_mutex.lock();
    }

    auto try_lock() const
    {
        return m_mutex.try_lock();
    }

    void lock_shared() const
    {
        m_mutex.lock_shared();
    }

    auto try_lock_shared() const
    {
        return m_mutex.try_lock_shared();
    }

    void unlock() const
    {
        m_mutex.unlock();
    }

    void unlock_shared() const
    {
        m_mutex.unlock_shared();
    }

private:
    mutable std::shared_mutex m_mutex;
    mutable store_type m_store;

    value_type m_value;
};
