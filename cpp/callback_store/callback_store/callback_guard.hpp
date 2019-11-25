#pragma once

#include "../../link/link/link_element.hpp"

template<typename T>
class callback;

/// \brief			RAII `callback` guard, limiting its activity to a scope.
template<typename T>
class callback_guard : private link_element
{
public:
	friend class callback<T>;

public:
	/// \brief			Default constructor, creates empty guard.
	callback_guard() = default;

	/// \brief			Constructor, creates guard for `callback_ptr`.
	explicit callback_guard(callback<T> * callback_ptr);
};

#pragma region callback_guard implementation:

template<typename T>
callback_guard<T>::callback_guard(callback<T> * callback_ptr) :
	link_element(callback_ptr)
{
}

#pragma endregion
