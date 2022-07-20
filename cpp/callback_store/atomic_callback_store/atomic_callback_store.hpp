#pragma once

#include <mutex>
#include <vector>

#include "atomic_callback.hpp"
#include "atomic_callback_guard.hpp"

template<typename T, typename M = std::mutex>
class atomic_callback_store
{
public:
    using mutex_type = M;

public:
    /// \brief          Default constructor, initializes empty store.
    atomic_callback_store();

    /// \brief          Move constructor, initilizes from `other`.
    atomic_callback_store(atomic_callback_store && other) noexcept;

    /// \brief          Move operator, moves from `other`.
    atomic_callback_store & operator  =(atomic_callback_store && other) noexcept;

    /// \copydoc        callback::subscribe(T &)
    atomic_callback_guard<T, M> subscribe(T callable);

    /// \brief          Invokes a method from `T` on each subscribed callback.
    template<typename ... A>
    void invoke(const A & ... args) const;

    template<typename FT, typename FM>
    friend void swap(atomic_callback_store<FT, FM> & lhs, atomic_callback_store<FT, FM> & rhs);

private:
    mutable mutex_type m_mutex;
    std::vector<atomic_callback<T, M>> m_callback_store;
};

#pragma region atomic_callback_store implementation:

template<typename T, typename M>
atomic_callback_store<T, M>::atomic_callback_store()
{
}

template<typename T, typename M>
atomic_callback_store<T, M>::atomic_callback_store(atomic_callback_store && other) noexcept :
    atomic_callback_store()
{
    swap(*this, other);
}

template<typename T, typename M>
atomic_callback_store<T, M> & 
atomic_callback_store<T, M>::operator  =(atomic_callback_store && other) noexcept
{
    atomic_callback_store empty;
    swap(*this, empty);
    swap(*this, other);

    return (*this);
}

template<typename T, typename M>
atomic_callback_guard<T, M>
atomic_callback_store<T, M>::subscribe(T callable)
{
    atomic_callback<T, M> c;
    atomic_callback_guard<T, M> g;
    std::tie(c, g) = make_atomic_callback<T, M>(std::move(callable));

    // Critical section:
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_callback_store.emplace_back(std::move(c));
    }

    // JMTODO: don't emplace back, instead reuse inactive callbacks

    return g;
}

template<typename T, typename M>
template<typename ... A>
void
atomic_callback_store<T, M>::invoke(const A & ... args) const
{
    std::unique_lock<std::mutex> lock(m_mutex);

    for (auto & callback : m_callback_store)
    {
        callback.invoke(std::forward<const A &>(args) ...);
    }
}

template<typename T, typename M>
void swap(atomic_callback_store<T, M> & lhs, atomic_callback_store<T, M> & rhs)
{
    std::unique_lock lock_lhs(lhs.m_mutex);
    std::unique_lock lock_rhs(rhs.m_mutex);

    using std::swap;
    swap(lhs.m_callback_store, rhs.m_callback_store);
}

#pragma endregion
