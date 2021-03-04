#pragma once

#include "../../link/link/link_element.hpp"

/// \brief          Scope-guard for `callback`, limiting its activity.
template<typename T>
class callback_guard : private link_element
{
public:
    /// \brief          Default constructor, creates empty guard.
    //callback_guard() noexcept = default;
    callback_guard() noexcept;

    /// \brief          Constructor, creates `callback` guard, linked by `link_element`.
    /// \param          element       `link_element` to the `callback` counterpart.
    callback_guard(link_element element) noexcept;
};

#pragma region callback_guard implementation:

template<typename T>
callback_guard<T>::callback_guard() noexcept :
    link_element()
{
}

template<typename T>
callback_guard<T>::callback_guard(link_element element) noexcept :
    link_element(std::move(element))
{
}

#pragma endregion
