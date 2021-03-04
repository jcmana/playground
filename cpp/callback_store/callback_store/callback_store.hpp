#pragma once

#include <vector>
#include <type_traits>

#include "callback.hpp"
#include "callback_guard.hpp"
#include "callback_factory.hpp"

/// \brief          Stores `callback`s in order and allows their invocation.
template<typename T>
class callback_store
{
public:
    /// \brief          Subscribes `invocation` for notifications.
    /// \param          invocation      Invocation receiving notifications.
    callback_guard<T> subscribe(T callable);

    /// \brief          Invokes each subscribed callback.
    /// \param          args            `callback` arguments.
    template<typename ... A>
    void invoke(A && ... args);

private:
    std::vector<callback<T>> m_callback_store;
};

#pragma region callback_store implementation:

template<typename T>
callback_guard<T>
callback_store<T>::subscribe(T callable)
{
    callback<T> c;
    callback_guard<T> g;
    std::tie(c, g) = make_callback(callable);

    for (auto && callback : m_callback_store)
    {
        if (callback.active() == false)
        {
            callback = std::move(c);
            break;
        }
    }

    // Emplace callback, if no inactive callback was replaced
    if (c.active())
    {
        m_callback_store.emplace_back(std::move(c));
    }

    return g;
}

template<typename T>
template<typename ... A>
void
callback_store<T>::invoke(A && ... args)
{
    for (auto & callback : m_callback_store)
    {
        callback.invoke(std::forward<A>(args) ...);
    }
}

#pragma endregion
