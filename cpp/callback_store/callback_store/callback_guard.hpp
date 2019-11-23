#pragma once

#include "../../link/link/link_element.hpp"

template<typename T>
class callback_guard : private link_element
{
public:
	template<typename T>
	friend class callback_store;

	template<typename T>
	friend class callback;

public:
	callback_guard()
	{
	}

public:
	callback_guard(callback<T> * callback_ptr) :
		link_element(callback_ptr)
	{
	}
};
