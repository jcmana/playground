#pragma once

#include <tuple>
#include <utility>

#include "../../link/link/atomic_link_element.hpp"

template<typename T>
class atomic_callback_guard;

/// \brief      Thread-safe scope-guarded callback.
template<typename T>
class atomic_callback : private atomic_link_element
{
public:
    friend class atomic_callback_guard<T>;

public:
    /// \copydoc        callback<T>::callback()
    atomic_callback();

    /// \copydoc        callback<T>::callback(T &, link_element &&)
    atomic_callback(T & interface_ref, atomic_link_element && link_element_rref);

    /// \copydoc        callback<T>::invoke(F, A && ...)
    template<typename F, typename ... A>
    void invoke(F method_ptr, A && ... args) const;

    /// \brief      Creates active `atomic_callback` and appropriate `atomic_callback_guard`.
    /// \relates    atomic_callback<T>
    template<typename T>
    friend std::tuple<atomic_callback<T>, atomic_callback_guard<T>> make_atomic_callback(T & interface_ref);

private:
    T * m_interface_ptr;
};

#pragma region atomic_callback implementation:

template<typename T>
atomic_callback<T>::atomic_callback() :
    atomic_link_element(),
    m_interface_ptr(nullptr)
{
}

template<typename T>
atomic_callback<T>::atomic_callback(T & interface_ref, atomic_link_element && link_element_rref) :
    atomic_link_element(std::move(link_element_rref)),
    m_interface_ptr(&interface_ref)
{
}

template<typename T>
template<typename F, typename ... A>
void 
atomic_callback<T>::invoke(F method_ptr, A && ... args) const
{
    std::unique_lock<const atomic_link_element> lock(*this);

    if (atomic_link_element::linked())
    {
        (m_interface_ptr->*method_ptr)(std::forward<A>(args) ...);
    }
}

template<typename T>
std::tuple<atomic_callback<T>, atomic_callback_guard<T>> 
make_atomic_callback(T & interface_ref)
{
    auto link = make_atomic_link();

    atomic_callback<T> callback(interface_ref, std::move(std::get<0>(link)));
    atomic_callback_guard<T> callback_guard(std::move(std::get<1>(link)));

    return std::make_tuple(std::move(callback), std::move(callback_guard));
}

#pragma endregion
