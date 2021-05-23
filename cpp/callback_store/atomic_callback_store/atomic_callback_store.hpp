#pragma once

#include <mutex>
#include <vector>

#include "atomic_callback.hpp"
#include "atomic_callback_guard.hpp"

template<typename T>
class atomic_callback_store
{
public:
    /// \brief          Default constructor, initializes empty store.
    atomic_callback_store();

    /// \brief          Move constructor, initilizes from `other`.
    atomic_callback_store(atomic_callback_store && other) noexcept;

    /// \brief          Move operator, moves from `other`.
    atomic_callback_store & operator  =(atomic_callback_store & other) noexcept;

    /// \copydoc        callback::subscribe(T &)
    atomic_callback_guard<T> subscribe(T callable);

    /// \brief          Invokes a method from `T` on each subscribed callback.
    template<typename ... A>
    void invoke(A && ... args) const;

    template<typename T>
    friend void swap(atomic_callback_store<T> & lhs, atomic_callback_store<T> & rhs);

private:
    mutable std::mutex m_mutex;
    std::vector<atomic_callback<T>> m_callback_store;
};

#pragma region atomic_callback_store implementation:

template<typename T>
atomic_callback_store<T>::atomic_callback_store()
{
}

template<typename T>
atomic_callback_store<T>::atomic_callback_store(atomic_callback_store && other) noexcept :
    atomic_callback_store()
{
    swap(*this, other);
}

template<typename T>
atomic_callback_store<T> & 
atomic_callback_store<T>::operator  =(atomic_callback_store & other) noexcept
{
    atomic_callback_store empty;
    swap(*this, empty);
    swap(*this, other);

    return (*this);
}

template<typename T>
atomic_callback_guard<T>
atomic_callback_store<T>::subscribe(T callable)
{
    atomic_callback<T> c;
    atomic_callback_guard<T> g;
    std::tie(c, g) = make_atomic_callback(std::move(callable));

    // Critical section:
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_callback_store.emplace_back(std::move(c));
    }

    // JMTODO: don't emplace back, instead reuse inactive callbacks

    return g;
}

template<typename T>
template<typename ... A>
void
atomic_callback_store<T>::invoke(A && ... args) const
{
    std::unique_lock<std::mutex> lock(m_mutex);

    for (auto & callback : m_callback_store)
    {
        callback.invoke(std::forward<A>(args) ...);
    }
}

template<typename T>
void swap(atomic_callback_store<T> & lhs, atomic_callback_store<T> & rhs)
{
    std::unique_lock lock_lhs(lhs.m_mutex);
    std::unique_lock lock_rhs(rhs.m_mutex);

    using std::swap;
    swap(lhs.m_callback_store, rhs.m_callback_store);
}

#pragma endregion
