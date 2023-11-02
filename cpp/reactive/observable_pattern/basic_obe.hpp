#pragma once

#include <functional>
#include <tuple>
#include <type_traits>

#include "../../callback_store/atomic_callback_store/atomic_callback_store.hpp"
#include "../../concurrency/concurrency/switch_mutex.h"
#include "../../concurrency/concurrency/switch_mutex_lock.hpp"

#include "basic_obe_storage.hpp"

/// \brief      Common base for observable pattern implementation.
/// \param      T       Stored value type.
/// \param      F       Notification handler type.
/// \param      S       Storage type.
template<typename T, template <typename> typename F, template <typename> typename S = basic_obe_storage>
class basic_obe
{
public:
    using value_type = T;
    using value_storage_type = S<T>;
    using observed_type = typename value_storage_type::observed_type;

    using store_callback_type = F<const observed_type &>;
    using store_type = atomic_callback_store<store_callback_type>;
    using guard_type = atomic_callback_guard<store_callback_type>;

    using mutex_type = switch_mutex;

public:
    /// \brief      Default contructor.
    basic_obe() noexcept :
        m_store(),
        m_value()
    {
    }

    /// \brief      Copy contructor, deleted.
    basic_obe(const basic_obe & other) noexcept = delete;

    /// \brief      Move contructor.
    basic_obe(basic_obe && other) noexcept :
        m_store(std::move(other.m_store)),
        m_value(std::move(other.m_value))
    {
        other.m_store = {};
        other.m_value = {};
    }

    /// \brief      Contructor, initializes with copy of `value`.
    explicit basic_obe(value_type value) noexcept :
        m_value(std::move(value))
    {
    }

    /// \brief      Copy assignment, deleted.
    basic_obe & operator  =(const basic_obe & other) noexcept = delete;

    /// \brief      Move assignment.
    basic_obe & operator  =(basic_obe && other) noexcept
    {
        m_store = std::move(other.m_store);
        m_value = std::move(other.m_value);
        return (*this);
    }

    /// \brief      Assignment, from copy of `value`.
    basic_obe & operator  =(value_type value)
    {
        m_value = std::move(value);
        return (*this);
    }

    /// \brief      Conversion to `value_type`.
    operator observed_type() const
    {
        return m_value.get();
    }

    /// \returns    Reference to the value.
    observed_type & get() noexcept
    {
        return m_value.get();
    }

    /// \returns    Const. reference to the value.
    const observed_type & get() const
    {
        return m_value.get();
    }

    /// \brief      Subscribes `callback` for notifications.
    auto observe(store_callback_type callback) const
    {
        return m_store.subscribe(std::move(callback));
    }

    /// \brief      Subscribes `callback` for notifications.
    template<typename C>
    auto observe(C callback) const
    {
        return m_store.subscribe(static_cast<store_callback_type>(callback));
    }

    /// \brief      Invokes each active `callback` with current value as argument.
    void notify() const
    {
        m_store.invoke(m_value.get());
    }

    friend void swap(basic_obe & lhs, basic_obe & rhs)
    {
        swap(lhs.m_store, rhs.m_store);
        swap(lhs.m_value, rhs.m_value);
    }

public:
    // SwitchMutex implementation:
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

    void lock_unique() const
    {
        m_mutex.lock_unique();
    }

    bool try_lock_unique() const 
    {
        return m_mutex.try_lock_unique();
    }

    void unlock_unique() const
    {
        m_mutex.unlock_unique();
    }

private:
    mutable store_type m_store;
    mutable mutex_type m_mutex;
    value_storage_type m_value;
};


