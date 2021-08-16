#pragma once

#include <shared_mutex>
#include <functional>
#include <tuple>

#include "../../callback_store/atomic_callback_store/atomic_callback_store.hpp"

/// \brief      Common base for observable pattern implementation.
template<template <typename> typename F, typename ... A>
class basic_observable
{
public:
    using value_type = std::tuple<A ...>;
    using store_callback_type = F<A ...>;
    using store_type = atomic_callback_store<store_callback_type>;
    using guard_type = atomic_callback_guard<store_callback_type>;
    using mutex_type = std::shared_mutex;

public:
    /// \brief      Default contructor.
    basic_observable() noexcept = default;
    basic_observable(const basic_observable & other) noexcept = delete;
    basic_observable(basic_observable && other) noexcept = default;

    /// \brief      Contructor, initializes value with `args`.
    explicit basic_observable(A ... args) :
        m_value(std::forward<A>(args) ...)
    {
    }

    /// \brief      Contructor, initializes value from `tuple`.
    basic_observable(value_type && value) :
        m_value(std::move(value))
    {
    }

    basic_observable & operator =(const basic_observable & other) noexcept = delete;
    basic_observable & operator =(basic_observable && other) noexcept = default;

    basic_observable & operator =(value_type && value)
    {
        m_value = std::move(value);
        return (*this);
    }

    operator std::tuple<A ...>() const
    {
        return m_value;
    }

    template<std::size_t I>
    auto & get() noexcept
    {
        return std::get<I>(m_value);
    }

    template<std::size_t I>
    const auto & get() const noexcept
    {
        return std::get<I>(m_value);
    }

    /// \brief      Subscribes `callback` for modification notifications.
    auto observe(store_callback_type callback) const noexcept
    {
        return m_store.subscribe(std::move(callback));
    }

    /// \brief      Invokes each active callback with current value as argument.
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

    bool try_lock() const
    {
        return m_mutex.try_lock();
    }

    void unlock()
    {
        m_mutex.unlock();
    }

    void lock_shared() const
    {
        m_mutex.lock_shared();
    }

    bool try_lock_shared() const
    {
        return m_mutex.try_lock_shared();
    }

    void unlock_shared()
    {
        m_mutex.unlock_shared();
    }

private:
    mutable store_type m_store;
    mutable mutex_type m_mutex;
    value_type m_value;
};

template <template <typename> typename F, typename ... A> 
struct std::tuple_size<basic_observable<F, A ...>> : std::tuple_size<std::tuple<A ...>>
{
};

template<std::size_t I, template <typename> typename F, typename ... A> 
struct std::tuple_element<I, basic_observable<F, A ...>> : std::tuple_element<I, std::tuple<A ...>>
{
};
