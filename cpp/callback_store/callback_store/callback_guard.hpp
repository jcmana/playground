#pragma once

#include "../../link/link/link_element.hpp"

template<typename T>
class callback_guard : private link_element
{
public:
	template<typename TT>
	friend class callback_store;

	template<typename TT>
	friend class callback;

public:
	callback_guard() = default;

public:
	explicit callback_guard(callback<T> * callback_ptr) :
		link_element(callback_ptr)
	{
	}
};

#include "callback.hpp"
