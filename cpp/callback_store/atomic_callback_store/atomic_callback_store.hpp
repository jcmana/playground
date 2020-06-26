#pragma once

#include <mutex>
#include <vector>

#include "atomic_callback.hpp"
#include "atomic_callback_guard.hpp"

template<typename T>
class atomic_callback_store
{
public:
    /// \copydoc        callback::subscribe(T &)
    atomic_callback_guard<T> subscribe(T & interface_ref);

    /// \brief          Invokes a method from `T` on each subscribed callback.
    template<typename = std::enable_if_t<std::is_class<T>::value>, typename F, typename ... A>
    void invoke(F method_ptr, A && ... args) const;

private:
    mutable std::mutex m_mutex;
    std::vector<atomic_callback<T>> m_callback_store;
};

#pragma region atomic_callback_store implementation:

template<typename T>
atomic_callback_guard<T>
atomic_callback_store<T>::subscribe(T & interface_ref)
{
    atomic_callback<T> c;
    atomic_callback_guard<T> g;
    std::tie(c, g) = make_atomic_callback(interface_ref);

    // Critical section:
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_callback_store.emplace_back(std::move(c));
    }

    // JMTODO: don't emplace back, instead reuse inactive callbacks

    return g;
}

template<typename T>
template<typename, typename F, typename ... A>
void
atomic_callback_store<T>::invoke(F method_ptr, A && ... args) const
{
    std::unique_lock<std::mutex> lock(m_mutex);

    for (auto & callback : m_callback_store)
    {
        callback.invoke(method_ptr, std::forward<A>(args) ...);
    }
}

#pragma endregion
