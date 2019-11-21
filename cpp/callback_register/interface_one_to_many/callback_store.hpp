#pragma once

#include <vector>
#include <algorithm>
#include <utility>

/// \brief		Stores callbacks for 1:N notification dispatch.
template<typename T>
class callback_store
{
public:
	template<typename T> 
	friend class callback_guard;

public:
	/// \brief		Default constructor, creates empty callback store.
	callback_store() = default;

	/// \brief		Move constructor, moves all stored callbacks.
	callback_store(callback_store && other) noexcept;

	/// \brief		Destructor, removes all stored callbacks.
	~callback_store();

	/// \brief		Move assignment, moves all registered callbacks.
	callback_store & operator  =(callback_store && other) noexcept;

	/// \brief		Subscribes interface `T` for notifications.
	callback_guard<T> subscribe(T * callback_ptr);

	/// \brief		Notifies all registered interfaces using method `F`.
	///
	/// Notifies all registered interfaces by calling method `F` on each with specified arguments. Each
	/// interface is notified in order they were registered. Calling this method on empty callback
	/// register is allowed and results in no calls.
	template<typename F, typename ... Args>
	void notify(F method_ptr, Args ... args) const;

	/// \brief		Removes all stored callbacks.
	void clear()
	{
		for (auto * guard_ptr : m_guard_store)
		{
			guard_ptr->release();
		}
	}

	void assign(callback_store && other) noexcept
	{
		m_guard_store = std::move(other.m_guard_store);

		for (auto * guard_ptr : m_guard_store)
		{
			guard_ptr->m_store_ptr = this;
		}
	}

protected:
	/// \brief		Inserts `holder_ptr` into notification sequence as last.
	void insert(callback_guard<T> * guard_ptr);

	/// \brief		Removes `holder_ptr` from notification sequence.
	void remove(callback_guard<T> * guard_ptr);

	/// \brief		Replaces `old_holder_ptr` with `new_holder_ptr`.
	void replace(callback_guard<T> * old_guard_ptr, callback_guard<T> * new_guard_ptr);

private:
	std::vector<callback_guard<T> *> m_guard_store;
};

#include "callback_guard.hpp"

template<typename T>
callback_store<T>::callback_store(callback_store && other) noexcept
{
	m_guard_store = std::move(other.m_guard_store);

	for (auto * guard_ptr : m_guard_store)
	{
		guard_ptr->m_store_ptr = this;
	}
}

template<typename T>
callback_store<T>::~callback_store()
{
	for (auto * guard_ptr : m_guard_store)
	{
		guard_ptr->release();
	}
}

template<typename T>
callback_store<T> & 
callback_store<T>::operator  =(callback_store && other) noexcept
{
	if (this == &other)
	{
		return (*this);
	}

	for (auto * guard_ptr : m_guard_store)
	{
		guard_ptr->release();
	}

	m_guard_store = std::move(other.m_guard_store);

	for (auto * guard_ptr : m_guard_store)
	{
		guard_ptr->m_store_ptr = this;
	}

	return (*this);
}

template<typename T>
callback_guard<T> 
callback_store<T>::subscribe(T * callback_ptr)
{
	callback_guard<T> ch(this, callback_ptr);
	insert(&ch);
	return std::move(ch);
}

template<typename T>
template<typename F, typename ... Args>
void 
callback_store<T>::notify(F method_ptr, Args ... args) const
{
	for (auto * guard_ptr : m_guard_store)
	{
		guard_ptr->invoke(method_ptr, std::forward<Args>(args) ...);
	}
}

template<typename T>
void 
callback_store<T>::insert(callback_guard<T> * guard_ptr)
{
	m_guard_store.insert(m_guard_store.end(), guard_ptr);
}

template<typename T>
void 
callback_store<T>::remove(callback_guard<T> * guard_ptr)
{
	m_guard_store.erase(std::find(m_guard_store.begin(), m_guard_store.end(), guard_ptr));
}

template<typename T>
void 
callback_store<T>::replace(callback_guard<T> * old_guard_ptr, callback_guard<T> * new_guard_ptr)
{	
	m_guard_store.erase(std::find(m_guard_store.begin(), m_guard_store.end(), old_guard_ptr));
	m_guard_store.insert(m_guard_store.end(), new_guard_ptr);
}
