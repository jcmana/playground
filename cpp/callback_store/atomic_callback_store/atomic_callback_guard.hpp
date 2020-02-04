#pragma once

#include <mutex>

#include "../../link/link/link_element.hpp"

template<typename T>
class atomic_callback;

template<typename T>
class atomic_callback_guard : private link_element
{
public:
	friend class atomic_callback<T>;

public:
	atomic_callback_guard() = default;

	explicit atomic_callback_guard(atomic_callback<T> * callback_ptr) :
		link_element(callback_ptr),
		m_lock(callback_ptr->m_mutex, std::defer_lock)
	{
	}

	~atomic_callback_guard()
	{
		std::lock_guard<std::unique_lock<std::mutex>> lock(m_lock);

		// Break the link to atomic_callback (disabling the callback)
		link_element::operator  =(link_element());
	}

private:
	mutable std::unique_lock<std::mutex> m_lock;
};