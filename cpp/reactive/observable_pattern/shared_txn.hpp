#pragma once

#include "shared_obe.hpp"

/// \brief      Shared transaction on `shared_obe` allows concurrent read.
template<typename T>
class shared_txn
{
public:
    using scoped_observable_type = shared_obe<T>;
    using observable_type = typename scoped_observable_type::observable_type;
    using value_type = typename scoped_observable_type::value_type;

    template<typename FT>
    friend class unique_txn;

public:
    shared_txn() :
        m_lock(),
        m_sp()
    {
    }

    shared_txn(const scoped_observable_type & observable_ref) :
        m_lock(shared_lock(*observable_ref.m_sp)),
        m_sp(observable_ref.m_sp)
    {
    }

    operator value_type() const
    {
        return static_cast<value_type>(*m_sp);
    }

    const value_type & get() const noexcept
    {
        return m_sp->get();
    }

private:
    mutable shared_lock<observable_type> m_lock;
    std::shared_ptr<const observable_type> m_sp;
};
