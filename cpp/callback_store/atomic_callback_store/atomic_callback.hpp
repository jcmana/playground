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
	explicit atomic_callback(T & interface_ref) :
		m_lock(m_mutex, std::defer_lock),
        m_interface_ref(interface_ref)
	{
	}

	atomic_callback(atomic_callback && other) :
		m_lock(other.m_mutex),
		link_element(std::move(other)),
        m_interface_ref(other.m_interface_ref)
	{
		m_lock = std::unique_lock<std::mutex>(m_mutex, std::defer_lock);
	}

	atomic_callback & operator  =(atomic_callback && other)
	{
		m_lock = std::unique_lock<std::mutex>(other.m_mutex);

		link_element::operator  =(std::move(other));
		m_interface_ref = other.m_interface_ref;

		m_lock = std::unique_lock<std::mutex>(m_mutex, std::defer_lock);

		return (*this);
	}

	~atomic_callback()
	{
		std::unique_lock<std::unique_lock<std::mutex>> lock(m_lock);
        link_element::release();
	}

	template<typename F, typename ... Args >
	void invoke(F method_ptr, Args && ... args)
	{
		std::unique_lock<std::unique_lock<std::mutex>> lock(m_lock);

		if (link_element::is_linked())
		{
			(m_interface_ref.*method_ptr)(std::forward<Args>(args) ...);
		}
	}

private:
	mutable std::mutex m_mutex;
	mutable std::unique_lock<std::mutex> m_lock;

	T & m_interface_ref;
};