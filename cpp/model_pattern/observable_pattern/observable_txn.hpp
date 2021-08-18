#pragma once

#include <mutex>

#include "shared_observable.hpp"

/*
template<typename ... A>
class shared_txn
{
public:
    using scoped_observable_type = shared_observable<A ...>;
    using value_type = typename scoped_observable_type::value_type;

public:
    shared_txn() :
        m_lock(),
        m_observable_ptr(nullptr)
    {
    }

    shared_txn(scoped_observable_type & observable_ref) :
        m_lock(observable_ref),
        m_observable_ptr(&observable_ref)
    {
    }

    operator std::tuple<A ...>() const
    {
        return (*m_observable_ptr);
    }

    template<std::size_t I>
    const auto & get() const noexcept
    {
        return m_observable_ptr->get<I>();
    }

private:
    mutable std::shared_lock<scoped_observable_type> m_lock;
    scoped_observable_type * m_observable_ptr;
};
*/