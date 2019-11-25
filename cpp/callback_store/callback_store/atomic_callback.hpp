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

	atomic_callback(atomic_callback && other) :
		m_lock(other.m_mutex),
		link_element(std::move(other)),
		m_interface_ptr(other.m_interface_ptr)
	{
		m_lock = std::unique_lock<std::mutex>(m_mutex, std::defer_lock);
	}

	atomic_callback & operator  =(atomic_callback && other)
	{
		m_lock = std::unique_lock<std::mutex>(other.m_mutex);

		link_element::operator  =(std::move(other));
		m_interface_ptr = other.m_interface_ptr;

		m_lock = std::unique_lock<std::mutex>(m_mutex, std::defer_lock);

		return (*this);
	}

	~atomic_callback()
	{
		m_lock.lock();

		// Break the link to callback_guard
		link_element::operator  =(link_element());
	}

	template<typename F, typename ... Args >
	void invoke(F method_ptr, Args && ... args)
	{
		std::lock_guard<std::unique_lock<std::mutex>> lock(m_lock);

		if (link_element::is_linked())
		{
			(m_interface_ptr->*method_ptr)(std::forward<Args>(args) ...);
		}
	}


private:
	T * m_interface_ptr;

	mutable std::mutex m_mutex;
	mutable std::unique_lock<std::mutex> m_lock;
};