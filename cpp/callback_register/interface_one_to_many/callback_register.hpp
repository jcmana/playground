#pragma once

#include <vector>
#include <algorithm>
#include <utility>

/// \brief		Thread-safe callback register for 1:N relation.
template<typename T>
class callback_register
{
public:
	template<typename T> friend class callback_holder;

public:
	/// \brief		Default constructor, creates empty callback register.
	callback_register() = default;

	/// \brief		Move constructor, moves all registered callbacks.
	callback_register(callback_register && other) noexcept;

	/// \brief		Destructor, removes all registered callbacks.
	~callback_register();

	/// \brief		Move assignment, moves all registered callbacks.
	callback_register & operator  =(callback_register && other) noexcept;

	/// \brief		Subscribes interface `T` for notifications.
	callback_holder<T> subscribe(T * ptr);

	/// \brief		Notifies all registered interfaces using method `F`.
	///
	/// Notifies all registered interfaces by calling method `F` on each with specified arguments. Each
	/// interface is notified in order they were registered. Calling this method on empty callback
	/// register is allowed and results in no calls.
	template<typename F, typename ... Args>
	void notify(F method_ptr, Args ... args) const;

private:
	/// \brief		Inserts `holder_ptr` into notification sequence as last.
	void insert(callback_holder<T> * holder_ptr);

	/// \brief		Removes `holder_ptr` from notification sequence.
	void remove(callback_holder<T> * holder_ptr);

	/// \brief		Replaces `old_holder_ptr` with `new_holder_ptr`.
	void replace(callback_holder<T> * old_holder_ptr, callback_holder<T> * new_holder_ptr);

private:
	std::vector<callback_holder<T> *> m_holder_store;
};

template<typename T>
callback_register<T>::callback_register(callback_register && other) noexcept
{
	m_holder_store = std::move(other.m_holder_store);

	for (auto * holder_ptr : m_holder_store)
	{
		holder_ptr->m_register_ptr = this;
	}
}

template<typename T>
callback_register<T>::~callback_register()
{
	for (auto * holder_ptr : m_holder_store)
	{
		holder_ptr->dismiss();
	}
}

template<typename T>
callback_register<T> & 
callback_register<T>::operator  =(callback_register && other) noexcept
{
	for (auto * holder_ptr : m_holder_store)
	{
		holder_ptr->dismiss();
	}

	m_holder_store = std::move(other.m_holder_store);

	for (auto * holder_ptr : m_holder_store)
	{
		holder_ptr->m_register_ptr = this;
	}

	return (*this);
}

template<typename T>
callback_holder<T> 
callback_register<T>::subscribe(T * ptr)
{
	callback_holder<T> ch(this, ptr);
	insert(&ch);
	return std::move(ch);
}

template<typename T>
template<typename F, typename ... Args>
void 
callback_register<T>::notify(F method_ptr, Args ... args) const
{
	for (auto * holder_ptr : m_holder_store)
	{
		std::invoke(method_ptr, holder_ptr->m_callback_ptr, std::forward<Args>(args) ...);
	}
}

template<typename T>
void 
callback_register<T>::insert(callback_holder<T> * holder_ptr)
{
	m_holder_store.insert(m_holder_store.end(), holder_ptr);
}

template<typename T>
void 
callback_register<T>::remove(callback_holder<T> * holder_ptr)
{
	m_holder_store.erase(std::find(m_holder_store.begin(), m_holder_store.end(), holder_ptr));
}

template<typename T>
void 
callback_register<T>::replace(callback_holder<T> * old_holder_ptr, callback_holder<T> * new_holder_ptr)
{
	m_holder_store.erase(std::find(m_holder_store.begin(), m_holder_store.end(), old_holder_ptr));
	insert(new_holder_ptr);
}
