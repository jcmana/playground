#pragma once

#include <mutex>
#include <utility>

#include "../../link/link/atomic_link_element.hpp"

/// \brief          Thread-safe scope-guard for `atomic_callback`, limiting its activity.
template<typename T, typename M = std::mutex>
class atomic_callback_guard : private atomic_link_element<M>
{
public:
    /// \copydoc        callback_guard<T>::callback_guard()
    atomic_callback_guard();

    /// \copydoc        callback_guard<T>::callback_guard(atomic_link_element &&)
    atomic_callback_guard(atomic_link_element<M> element);
};

#pragma region atomic_callback_guard implementation:

template<typename T, typename M>
atomic_callback_guard<T, M>::atomic_callback_guard() :
    atomic_link_element<M>()
{
}

template<typename T, typename M>
atomic_callback_guard<T, M>::atomic_callback_guard(atomic_link_element<M> element) :
    atomic_link_element<M>(std::move(element))
{
}

#pragma endregion
