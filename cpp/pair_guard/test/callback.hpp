#pragma once

#include <functional>
#include <utility>

#include "../pair_guard/pair_guard.hpp"

template<typename T>
class callback : private pair_element
{
public:
	template<typename T>
	friend class callback_guard;

public:
	callback(T * inteface_ptr) :
		m_interface_ptr(inteface_ptr)
	{
	}

	callback(T * inteface_ptr, callback_guard<T> * guard_ptr) :
		pair_element(guard_ptr),
		m_interface_ptr(inteface_ptr)
	{
	}

	template<typename F, typename ... Args >
	void invoke(F && method_ptr, Args && ... args)
	{
		if (linked())
		{
			std::invoke(method_ptr, m_interface_ptr, std::forward<Args>(args) ...);
		}
	}

private:
	T * m_interface_ptr;
};

#include "callback_guard.hpp"