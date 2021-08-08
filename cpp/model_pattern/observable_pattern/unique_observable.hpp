#pragma once

#include <mutex>
#include <memory>
#include <vector>
#include <functional>

#include "basic_observable.hpp"

template<typename ... A>
class unique_observable
{
public:
    template<typename ... FA>
    using functor_type = std::function<void(FA ...)>;
    using observable_type = basic_observable<functor_type, A ...>;
    using value_type = typename observable_type::value_type;

public:
    unique_observable() noexcept :
        m_up(new observable_type),
        m_observers()
    {
    }

    unique_observable(const unique_observable & other) = delete;

    unique_observable(unique_observable && other) noexcept :
        unique_observable()
    {
        swap(*this, other);
    }

    explicit unique_observable(A ... args) noexcept :
        m_up(new observable_type(std::forward<A>(args) ...)),
        m_observers()
    {
    }

    unique_observable & operator  =(const unique_observable & other) = delete;

    unique_observable & operator  =(unique_observable && other) noexcept
    {
        auto empty = unique_observable();
        swap(*this, empty);
        swap(*this, other);
        return (*this);
    }

    unique_observable & operator  =(value_type && value)
    {
        (*m_up) = std::move(value);
        return (*this);
    }

    template<std::size_t I>
    auto & get() noexcept
    {
        return m_up->get<I>();
    }

    template<std::size_t I>
    const auto & get() const noexcept
    {
        return m_up->get<I>();
    }

    void observe(functor_type<A ...> callback) const noexcept
    {
        m_observers.push_back(m_up->observe(std::move(callback)));
    }

    void notify() const
    {
        m_up->notify();
    }

    template<typename ... FA>
    friend void swap(unique_observable & lhs, unique_observable & rhs)
    {    
        using std::swap;
        swap(lhs.m_up, rhs.m_up);

        lhs.m_observers.clear();
        rhs.m_observers.clear();
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
    mutable std::shared_mutex m_mutex;
    std::unique_ptr<observable_type> m_up;
    mutable std::vector<typename observable_type::guard_type> m_observers;
};
