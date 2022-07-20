#pragma once

#include <tuple>
#include <utility>

#include "../../link/link/atomic_link_element.hpp"
#include "atomic_callback_guard.hpp"

/// \brief      Thread-safe scope-guarded callback.
template<typename T, typename M = std::mutex>
class atomic_callback : private atomic_link_element<M>
{
public:
    using mutex_type = M;

public:
    /// \copydoc        callback<T>::callback()
    atomic_callback();

    /// \copydoc        callback<T>::callback(T, link_element)
    atomic_callback(T callable, atomic_link_element<M> element);

    bool active() const;

    /// \copydoc        callback<T>::invoke(F, A && ...)
    template<typename ... A>
    void invoke(const A & ... args) const;

    /// \brief      Creates active `atomic_callback` and appropriate `atomic_callback_guard`.
    /// \relates    atomic_callback<T>
    template<typename FT, typename FM>
    friend std::tuple<atomic_callback<FT, FM>, atomic_callback_guard<FT, FM>> make_atomic_callback(FT callable);

private:
    T m_callable;
};

#pragma region atomic_callback implementation:

template<typename T, typename M>
atomic_callback<T, M>::atomic_callback() :
    m_callable()
{
}

template<typename T, typename M>
atomic_callback<T, M>::atomic_callback(T callable, atomic_link_element<M> element) :
    atomic_link_element(std::move(element)),
    m_callable(std::move(callable))
{
}

template<typename T, typename M>
bool 
atomic_callback<T, M>::active() const
{
    return atomic_link_element::linked();
}

template<typename T, typename M>
template<typename ... A>
void 
atomic_callback<T, M>::invoke(const A & ... args) const
{
    std::unique_lock<const atomic_link_element> lock(*this);

    if (active())
    {
        m_callable(std::forward<const A &>(args) ...);
    }
}

template<typename T, typename M = std::mutex>
std::tuple<atomic_callback<T, M>, atomic_callback_guard<T, M>> 
make_atomic_callback(T callable)
{
    auto [link_a, link_b] = make_atomic_link<M>();

    return std::tuple 
    {
        atomic_callback<T, M>(std::move(callable), std::move(link_a)),
        atomic_callback_guard<T, M>(std::move(link_b))
    };
}

#pragma endregion
