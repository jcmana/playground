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
    /// \brief      Default constructor, creates un-observerd event.
    basic_evt() :
        m_store()
    {
    }

    /// \brief      Subscribes `callback` for notifications.
    auto observe(store_callback_type callback)
    {
        return m_store.subscribe(std::move(callback));
    }

    /// \brief      Subscribes `callback` for notifications.
    template<typename C>
    auto observe(C callback)
    {
        return m_store.subscribe(static_cast<store_callback_type>(callback));
    }

    /// \brief      Invokes each active `callback` with current value as argument.
    void notify(const T & value) const
    {
        m_store.invoke(value);
    }

private:
    store_type m_store;
};