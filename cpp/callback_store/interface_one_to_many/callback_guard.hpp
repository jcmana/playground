#pragma once

#include <mutex>
#include <algorithm>

/// \brief		Callback notification subscription guard, deactivates it on destruction.
template<typename T>
class callback_guard
{
public:
	template<typename T> 
	friend class callback_store;

public:
	/// \brief		Default constructor, creates empty callback holder.
	callback_guard();

	/// \brief		Move constructor, copies `other` callback holder and deactivates it.
	callback_guard(callback_guard && other) noexcept;

	/// \brief		Destructor, deactivates the callback holder.
	~callback_guard();

	/// \brief		Move assignment, copies `other` callback holder and deactivates it.
	callback_guard & operator  =(callback_guard && other) noexcept;

protected:
	/// \brief		Constructor, creates active callback holder.
	callback_guard(callback_store<T> * store_ptr, T * callback_ptr);

	/// \brief		Invokes a method from callback interface with given `args`,
	template<typename F, typename ... Args>
	void invoke(F method_ptr, Args ... args);

	void assign(callback_guard && other) noexcept
	{
		m_store_ptr = other.m_store_ptr;
		m_callback_ptr = other.m_callback_ptr;

		other.release();

		m_store_ptr->replace(&other, this);
	}

	/// \brief		Release the held callback.
	void release();

private:
	callback_store<T> * m_store_ptr;
	T * m_callback_ptr;
};

template<typename T>
callback_guard<T>::callback_guard() :
	m_store_ptr(nullptr),
	m_callback_ptr(nullptr)
{
}

template<typename T>
callback_guard<T>::callback_guard(callback_guard && other) noexcept
{
	m_store_ptr = other.m_store_ptr;
	m_callback_ptr = other.m_callback_ptr;

	other.release();

	if (m_store_ptr == nullptr)
	{
		return;
	}

	m_store_ptr->replace(&other, this);
}

template<typename T>
callback_guard<T>::~callback_guard()
{
	if (m_store_ptr == nullptr)
	{
		return;
	}

	m_store_ptr->remove(this);
	release();
}

template<typename T>
callback_guard<T> &
callback_guard<T>::operator  =(callback_guard && other) noexcept
{
	m_store_ptr = other.m_store_ptr;
	m_callback_ptr = other.m_callback_ptr;

	other.release();

	if (m_store_ptr == nullptr)
	{
		return (*this);
	}

	m_store_ptr->replace(&other, this);

	return (*this);
}

template<typename T>
callback_guard<T>::callback_guard(callback_store<T> * store_ptr, T * callback_ptr) :
	m_store_ptr(store_ptr),
	m_callback_ptr(callback_ptr)
{
}

template<typename T>
template<typename F, typename ... Args>
void 
callback_guard<T>::invoke(F method_ptr, Args ... args)
{
	std::invoke(method_ptr, m_callback_ptr, std::forward<Args>(args) ...);
}

template<typename T>
void 
callback_guard<T>::release()
{
	m_store_ptr = nullptr;
	m_callback_ptr = nullptr;
}
