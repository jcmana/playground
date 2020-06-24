#pragma once

#include <vector>

#include "atomic_callback.hpp"
#include "atomic_callback_guard.hpp"

template<typename T>
class atomic_callback_store
{
public:
    /// \brief          Subscribes `interface_ref` for notifications.
    atomic_callback_guard<T> subscribe(T & interface_ref);

    /// \brief          Invokes a method from `T` on each subscribed callback.
    template<typename = std::enable_if_t<std::is_class<T>::value>, typename F, typename ... Args >
    void invoke(F method_ptr, Args && ... args) const;

private:
    std::vector<atomic_callback<T>> m_callback_store;
};

template<typename T>
atomic_callback_guard<T>
atomic_callback_store<T>::subscribe(T & interface_ref)
{
    atomic_callback<T> c;
    atomic_callback_guard<T> g;
    std::tie(c, g) = make_atomic_callback(interface_ref);

    m_callback_store.emplace_back(std::move(c));

    return g;
}

template<typename T>
template<typename, typename F, typename ... Args>
void
atomic_callback_store<T>::invoke(F method_ptr, Args && ... args) const
{
    for (auto & callback : m_callback_store)
    {
        callback.invoke(method_ptr, std::forward<Args>(args) ...);
    }
}
