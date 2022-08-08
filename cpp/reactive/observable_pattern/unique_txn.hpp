#pragma once

#include "shared_obe.hpp"

template<typename T>
class unique_txn;

/// \brief      Unique transaction on `shared_obe` allows unique read and write.
template<typename T>
class unique_txn
{
public:
    using scoped_observable_type = shared_obe<T>;
    using observable_type = typename scoped_observable_type::observable_type;
    using value_type = typename scoped_observable_type::value_type;
    using observed_type = typename scoped_observable_type::observed_type;

public:
    unique_txn() :
        m_lock(),
        m_sp()
    {
    }

    /// \brief      Contructor, uniquelly locks the observable.
    explicit unique_txn(scoped_observable_type & observable_ref) :
        m_lock(unique_lock(*observable_ref.m_sp)),
        m_sp(observable_ref.m_sp)
    {
    }

    /// \brief      Contructor, attempts to uniquelly lock the observable.
    explicit unique_txn(scoped_observable_type & observable_ref, std::try_to_lock_t) :
        m_lock(unique_lock(*observable_ref.m_sp, std::try_to_lock)),
        m_sp(observable_ref.m_sp)
    {
    }

    /// \brief      Destructor, notifies observers under `shared_lock` and unlocks after.
    ~unique_txn()
    {
        if (m_sp)
        {
            // Downgrade to shared lock to allow read access in callbacks. Using
            // unique lock leads to deadlock in subsequent callbacks when reading
            // this observable value is required.
            m_lock.unlock_unique();

            // Notify callbacks with current value
            m_sp->notify();
        }
    }

    unique_txn & operator  =(observed_type value) noexcept
    {
        m_sp->get() = std::move(value);
        return (*this);
    }

    operator observed_type() const
    {
        return static_cast<observed_type>(*m_sp);
    }

    observed_type & get() noexcept
    {
        return m_sp->get();
    }

    const observed_type & get() const noexcept
    {
        return m_sp->get();
    }

    void assign(observed_type value)
    {
        m_sp->get() = std::move(value);
    }

    /// \returns    `true` if uniquelly locked, `false` otherwise.
    bool owns_lock() const
    {
        return m_lock.owns_lock();
    }

private:
    mutable switch_lock<observable_type> m_lock;
    std::shared_ptr<observable_type> m_sp;
};

/// \brief      Unique transaction for `const` observables is disabled.
template<typename T>
class unique_txn<const T>
{
public:
    explicit unique_txn(shared_obe<const T> so)
    {
        static_assert(false, "unique_txn is illegal for shared_obe with const value");
    }
};