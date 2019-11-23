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
	callback_guard() :
		m_callback_ptr(nullptr)
	{
	}

private:
	callback_guard(callback<T> * callback_ptr) :
		link_element(callback_ptr),
		m_callback_ptr(callback_ptr)
	{
	}

private:
	callback<T> * m_callback_ptr;
};
