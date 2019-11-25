#pragma once

#include <mutex>

#include "../../link/link/link_element.hpp"

template<typename T>
class atomic_callback;

/// \brief			RAII `callback` guard, limiting its activity to a scope.
template<typename T>
class atomic_callback_guard : private link_element
{
public:
	friend class atomic_callback<T>;

public:
	/// \brief			Default constructor, creates empty guard.
	atomic_callback_guard() = default;

	/// \brief			Constructor, creates guard for `callback_ptr`.
	explicit atomic_callback_guard(atomic_callback<T> * callback_ptr) :
		link_element(callback_ptr),
		m_lock(callback_ptr->m_mutex, std::defer_lock)
	{
	}

	~atomic_callback_guard()
	{
		std::lock_guard<std::unique_lock<std::mutex>> lock(m_lock);
	}

private:
	mutable std::unique_lock<std::mutex> m_lock;
};