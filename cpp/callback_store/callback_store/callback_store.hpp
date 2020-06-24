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
    /// \brief          Default constructor, creates store with no subscriptions.
    callback_store() = default;

    /// \brief          Subscribes `interface_ref` for notifications.
    callback_guard<T> subscribe(T & interface_ref);

    /// \brief          Invokes a method from `T` on each subscribed callback.
    template<typename = std::enable_if_t<std::is_class<T>::value>, typename F, typename ... Args >
    void invoke(F method_ptr, Args && ... args) const;

    /// \brief          Invokes a method from `T` on each subscribed callback.
    template<typename = std::enable_if_t<std::is_function<T>::value>, typename ... Args >
    void invoke(Args && ... args) const;

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

    return g;
}

template<typename T>
template<typename, typename F, typename ... Args>
void
callback_store<T>::invoke(F method_ptr, Args && ... args) const
{
    for (auto & callback : m_callback_store)
    {
        callback.invoke(method_ptr, std::forward<Args>(args) ...);
    }
}

template<typename T>
template<typename, typename ... Args>
void
callback_store<T>::invoke(Args && ... args) const
{
    for (auto & callback : m_callback_store)
    {
        callback.invoke(std::forward<Args>(args) ...);
    }
}

#pragma endregion
