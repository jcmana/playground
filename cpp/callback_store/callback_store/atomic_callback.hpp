#pragma once

#include <mutex>
#include <utility>

#include "../../link/link/link_element.hpp"

template<typename T>
class atomic_callback_guard;

template<typename T>
class atomic_callback : private link_element
{
public:
	friend class atomic_callback_guard<T>;

public:
	explicit atomic_callback(T * interface_ptr) :
		m_lock(m_mutex, std::defer_lock)
	{
	}

	template<typename F, typename ... Args >
	void invoke(F method_ptr, Args && ... args)
	{
		std::lock_guard<std::unique_lock<std::mutex>> lock(m_lock);
		(m_interface_ptr->*method_ptr)(std::forward<Args>(args) ...);
	}

private:
	T * m_interface_ptr;

	mutable std::mutex m_mutex;
	mutable std::unique_lock<std::mutex> m_lock;
};