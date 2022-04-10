#pragma once

#include "shared_obe.hpp"

/// \brief      Unique transaction on `shared_obe` allow read and write.
template<typename T>
class unique_txn
{
public:
    using scoped_observable_type = shared_obe<T>;
    using observable_type = typename scoped_observable_type::observable_type;
    using value_type = typename scoped_observable_type::value_type;

public:
    unique_txn() = default;

    explicit unique_txn(const scoped_observable_type & observable_ref) :
        m_lock(unique_lock(*observable_ref.m_sp)),
        m_sp(observable_ref.m_sp)
    {
    }

    ~unique_txn()
    {
        if (m_sp)
        {
            // Downgrade to shared lock to avoid read access in callbacks
            m_lock.unlock_unique();

            // Notify callbacks with current value
            m_sp->notify();
        }
    }

    unique_txn & operator  =(value_type value) noexcept
    {
        (*m_sp) = std::move(value);
        return (*this);
    }

    operator value_type() const
    {
        return (*m_sp);
    }

    value_type & get() noexcept
    {
        return (*m_sp);
    }

    const value_type & get() const noexcept
    {
        return (*m_sp);
    }

private:
    mutable switch_lock<observable_type> m_lock;
    std::shared_ptr<observable_type> m_sp;
};
