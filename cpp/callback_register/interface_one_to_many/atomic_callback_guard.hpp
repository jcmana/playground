#pragma once

#include <mutex>
#include <utility>

#include "callback_guard.hpp"

template<typename T>
class atomic_callback_guard : 
	protected callback_guard<T>
{
public:
	template<typename T>
	friend class atomic_callback_store;

public:
	/// \brief		Default constructor, creates empty callback holder.
	atomic_callback_guard()
	{
	}

	/// \brief		Move constructor, copies `other` callback holder and deactivates it.
	atomic_callback_guard(atomic_callback_guard && other) noexcept :
		//atomic_callback_guard(std::move(other), std::lock_guard<std::mutex>(other.m_store_ptr->m_mutex)),
		callback_guard<T>(std::move(other))
	{
	}

	/// \brief		Destructor, deactivates the callback holder.
	~atomic_callback_guard()
	{
		callback_guard<T>::release();
	}

	/// \brief		Move assignment, copies `other` callback holder and deactivates it.
	atomic_callback_guard & operator  =(atomic_callback_guard && other) noexcept
	{
		callback_guard<T>::assign(std::move(other));

		return (*this);

	}

private:
	/*
	atomic_callback_guard(atomic_callback_guard<T> && other, std::lock_guard<std::mutex> & lock) noexcept :
		callback_guard<T>(std::move(other))
	{
	}
	*/

	atomic_callback_guard(atomic_callback_store<T> * store_ptr, T * callback_ptr) :
		callback_guard<T>(store_ptr, callback_ptr)
	{
	}
};

#include "atomic_callback_store.hpp"
