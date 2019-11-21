#pragma once

#include <mutex>
#include <utility>

#include "callback_store.hpp"

template<typename T>
class atomic_callback_store:
	protected callback_store<T>
{
public:
	template<typename T> 
	friend class atomic_callback_guard;

public:
	atomic_callback_store()
	{
	}

	atomic_callback_store(atomic_callback_store && other) noexcept
	{
		// Critical section (other):
		std::lock_guard<std::mutex> lock_other(other.m_mutex);

		callback_store<T>::assign(std::move(other));
	}

	~atomic_callback_store()
	{
		// Critical section:
		std::lock_guard<std::mutex> lock(m_mutex);

		callback_store<T>::clear();
	}

	atomic_callback_guard<T> subscribe(T * callback_ptr)
	{
		// Critical section:
		std::lock_guard<std::mutex> lock(m_mutex);

		atomic_callback_guard<T> ach(this, callback_ptr);
		callback_store<T>::insert(&ach);
		return std::move(ach);
	}

	template<typename F, typename ... Args>
	void notify(F method_ptr, Args ... args) const
	{
		// Critical section:
		std::lock_guard<std::mutex> lock(m_mutex);

		callback_store<T>::notify(method_ptr, std::forward<Args>(args) ...);
	}

private:
	mutable std::mutex m_mutex;
};

#include "atomic_callback_guard.hpp"
