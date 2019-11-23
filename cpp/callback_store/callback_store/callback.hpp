#pragma once

#include <utility>

#include "../../link/link/link_element.hpp"

template<typename T>
class callback : private link_element
{
public:
	template<typename TT>
	friend class callback_store;

	template<typename TT>
	friend class callback_guard;

public:
	explicit callback(T * inteface_ptr) :
		m_interface_ptr(inteface_ptr)
	{
	}

	template<typename F, typename ... Args >
	void invoke(F method_ptr, Args && ... args)
	{
		if (link_element::is_linked())
		{
			(m_interface_ptr->*method_ptr)(std::forward<Args>(args) ...);
		}
	}

private:
	T * m_interface_ptr;
};
