#pragma once

#include "../../link/link/link_element.hpp"

template<typename T>
class callback;

template<typename T>
class callback_guard : private link_element
{
public:
	friend class callback<T>;

public:
	callback_guard() = default;

	explicit callback_guard(callback<T> * callback_ptr) :
		link_element(callback_ptr)
	{
	}
};
