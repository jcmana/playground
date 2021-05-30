#pragma once

#include <tuple>
#include <utility>

#include "../../link/link/atomic_link_element.hpp"
#include "atomic_callback_guard.hpp"

/// \brief      Thread-safe scope-guarded callback.
template<typename T>
class atomic_callback : private atomic_link_element
{
public:
    /// \copydoc        callback<T>::callback()
    atomic_callback();

    /// \copydoc        callback<T>::callback(T, link_element)
    atomic_callback(T callable, atomic_link_element element);

    bool active() const;

    /// \copydoc        callback<T>::invoke(F, A && ...)
    template<typename ... A>
    void invoke(A && ... args) const;

    /// \brief      Creates active `atomic_callback` and appropriate `atomic_callback_guard`.
    /// \relates    atomic_callback<T>
    template<typename T>
    friend std::tuple<atomic_callback<T>, atomic_callback_guard<T>> make_atomic_callback(T callable);

private:
    T m_callable;
};

#pragma region atomic_callback implementation:

template<typename T>
atomic_callback<T>::atomic_callback() :
    m_callable()
{
}

template<typename T>
atomic_callback<T>::atomic_callback(T callable, atomic_link_element element) :
    atomic_link_element(std::move(element)),
    m_callable(std::move(callable))
{
}

template<typename T>
bool 
atomic_callback<T>::active() const
{
    return atomic_link_element::linked();
}

template<typename T>
template<typename ... A>
void 
atomic_callback<T>::invoke(A && ... args) const
{
    std::unique_lock<const atomic_link_element> lock(*this);

    if (active())
    {
        m_callable(std::forward<A>(args) ...);
    }
}

template<typename T>
std::tuple<atomic_callback<T>, atomic_callback_guard<T>> 
make_atomic_callback(T callable)
{
    auto [link_a, link_b] = make_atomic_link();

    return std::tuple 
    {
        atomic_callback<T>(std::move(callable), std::move(link_a)),
        atomic_callback_guard<T>(std::move(link_b))
    };
}

#pragma endregion
