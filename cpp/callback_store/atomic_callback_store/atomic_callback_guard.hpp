#pragma once

#include <utility>

#include "../../link/link/atomic_link_element.hpp"

template<typename T>
class atomic_callback;

/// \brief          Thread-safe scope-guard for `atomic_callback`, limiting its activity.
template<typename T>
class atomic_callback_guard : private atomic_link_element
{
public:
    friend class atomic_callback<T>;

public:
    /// \copydoc        callback_guard<T>::callback_guard()
    atomic_callback_guard();

    /// \copydoc        callback_guard<T>::callback_guard(atomic_link_element &&)
    atomic_callback_guard(atomic_link_element && link_element_rref);
};

#pragma region atomic_callback_guard implementation:

template<typename T>
atomic_callback_guard<T>::atomic_callback_guard() :
    atomic_link_element()
{
}

template<typename T>
atomic_callback_guard<T>::atomic_callback_guard(atomic_link_element && link_element_rref) :
    atomic_link_element(std::move(link_element_rref))
{
}

#pragma endregion
