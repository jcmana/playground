#pragma once

#include "shared_obe.hpp"
#include "basic_txn.hpp"

/// \brief      Shared transaction on `shared_obe`, allows concurrent read.
template<typename T>
class shared_txn : public basic_txn<T>
{
public:
    /*
    using scoped_observable_type = shared_obe<T>;
    using observable_type = typename scoped_observable_type::observable_type;
    using value_type = typename scoped_observable_type::value_type;
    */

    template<typename FT>
    friend class unique_txn;

public:
    shared_txn() :
        m_lock(),
        m_sp()
    {
    }

    shared_txn(const typename basic_txn<T>::scoped_observable_type & observable_ref) :
        m_lock(shared_lock(*observable_ref.m_sp)),
        m_sp(observable_ref.m_sp)
    {
    }

    operator typename basic_txn<T>::value_type() const
    {
        return static_cast<typename basic_txn<T>::value_type>(*m_sp);
    }

    const typename basic_txn<T>::value_type & get() const noexcept
    {
        return m_sp->get();
    }

private:
    mutable shared_lock<typename basic_txn<T>::observable_type> m_lock;
    std::shared_ptr<typename basic_txn<T>::observable_type> m_sp;
};
