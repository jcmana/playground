#pragma once

#include "../pair_guard/pair_guard.hpp"

template<typename T>
class callback_guard : private pair_element
{
public:
	template<typename T>
	friend class callback;

public:
	callback_guard()
	{
	}

	callback_guard(callback<T> * callback_ptr) :
		pair_element(callback_ptr)
	{
	}
};