#pragma once

#include "../../link/link/link_element.hpp"

/// \brief          Scope-guard for `callback`, limiting its activity.
template<typename T>
class callback_guard : private link_element
{
public:
	/// \brief          Default constructor, creates empty guard.
	callback_guard();

	/// \brief          Constructor, creates `callback` guard, linked by `link_element`.
    /// \param          link_element_rref       `link_element` to the `callback` counterpart.
	callback_guard(link_element && link_element_rref);
};

#pragma region callback_guard implementation:

template<typename T>
callback_guard<T>::callback_guard() :
    link_element()
{
}

template<typename T>
callback_guard<T>::callback_guard(link_element && link_element_rref) :
	link_element(std::move(link_element_rref))
{
}

#pragma endregion
