#pragma once

#include "../../callback_store/atomic_callback_store/atomic_callback_store.hpp"

/// \brief      Common base for event implementation.
template<typename T, template <typename> typename F>
class basic_evt
{
public:
    using value_type = T;

    using store_callback_type = F<const value_type &>;
    using store_type = atomic_callback_store<store_callback_type>;
    using guard_type = atomic_callback_guard<store_callback_type>;

public:
    /// \brief      Default constructor, creates un-observed event.
    basic_evt() :
        m_store()
    {
    }

    /// \brief      Copy constructor, deleted.
    basic_evt(const basic_evt & other) = delete;

    /// \brief      Move contructor.
    basic_evt(basic_evt && other) :
        m_store(std::move(other.m_store))
    {
        other.m_store = {};
    }

    /// \brief      Copy assignment, deleted.
    basic_evt & operator  =(const basic_evt & other) = delete;

    /// \brief      Move assignemnt.
    basic_evt & operator  =(basic_evt && other) noexcept
    {
        auto empty = basic_evt();
        swap(*this, empty);
        swap(*this, other);
        return (*this);
    }

    /// \brief      Subscribes `callback` for notifications.
    auto observe(store_callback_type callback)
    {
        return m_store.subscribe(std::move(callback));
    }

    /// \brief      Invokes each active `callback` with `value` as argument.
    void notify(const T & value) const
    {
        m_store.invoke(value);
    }

    friend void swap(basic_evt & lhs, basic_evt & rhs)
    {
        swap(lhs.m_store, rhs.m_store);
    }

private:
    store_type m_store;
};