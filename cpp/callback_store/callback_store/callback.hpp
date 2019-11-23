#pragma once

#include <functional>
#include <utility>

#include "../../link/link/link_element.hpp"

template<typename T>
class callback : private link_element
{
public:
	template<typename T>
	friend class callback_store;

	template<typename T>
	friend class callback_guard;

private:
	callback(T * inteface_ptr) :
		m_interface_ptr(inteface_ptr)
	{
	}

	callback(T * inteface_ptr, callback_guard<T> * guard_ptr) :
		link_element(guard_ptr),
		m_interface_ptr(inteface_ptr)
	{
	}

	template<typename F, typename ... Args >
	void invoke(F method_ptr, Args && ... args)
	{
		if (is_linked())
		{
			(m_interface_ptr->*method_ptr)(std::forward<Args>(args) ...);
		}
	}

private:
	T * m_interface_ptr;
};
