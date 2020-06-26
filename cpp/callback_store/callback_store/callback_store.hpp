#pragma once

#include <vector>
#include <type_traits>

#include "callback.hpp"
#include "callback_guard.hpp"

/// \brief          Stores callbacks and allows their invocation.
template<typename T>
class callback_store
{
public:
    /// \brief          Subscribes `interface_ref` for notifications.
    /// \param          interface_ref   Refernce to interface (function or class) receiving notifications.
    callback_guard<T> subscribe(T & interface_ref);

    /// \brief          Invokes a method from `T` on each subscribed callback.
    /// \param          method_ptr      Function-pointer from `T`.
    /// \param          args            `method_ptr` arguments.
    template<typename F, typename ... A>
    typename std::enable_if_t<std::is_class<T>::value == true>
    invoke(F method_ptr, A && ... args) const;

    /// \brief          Invokes a method from `T` on each subscribed callback.
    /// \param          args            Subrscribed functions arguments.
    template<typename ... A>
    typename std::enable_if_t<std::is_class<T>::value != true>
    invoke(A && ... args) const;

private:
	std::vector<callback<T>> m_callback_store;
};

#pragma region callback_store implementation:

template<typename T>
callback_guard<T>
callback_store<T>::subscribe(T & interface_ref)
{
    callback<T> c;
    callback_guard<T> g;
    std::tie(c, g) = make_callback(interface_ref);

    m_callback_store.emplace_back(std::move(c));

    // JMTODO: don't emplace back, instead reuse inactive callbacks

    return g;
}

template<typename T>
template<typename F, typename ... A>
typename std::enable_if_t<std::is_class<T>::value == true>
callback_store<T>::invoke(F method_ptr, A && ... args) const
{
    for (auto & callback : m_callback_store)
    {
        callback.invoke(method_ptr, std::forward<A>(args) ...);
    }
}

template<typename T>
template<typename ... A>
typename std::enable_if_t<std::is_class<T>::value != true>
callback_store<T>::invoke(A && ... args) const
{
    for (auto & callback : m_callback_store)
    {
        callback.invoke(std::forward<A>(args) ...);
    }
}

#pragma endregion
