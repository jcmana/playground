#pragma once

#include <mutex>
#include <algorithm>

/// \brief		Thread-safe callback registration holder, deactivates it on destruction.
template<typename T>
class callback_holder
{
public:
	template<typename T> friend class callback_register;

public:
	/// \brief		Default constructor, creates empty callback holder.
	callback_holder();

	/// \brief		Move constructor, copies `other` callback holder and deactivates it.
	callback_holder(callback_holder && other) noexcept;

	/// \brief		Destructor, deactivates the callback holder.
	~callback_holder();

	/// \brief		Move assignment, copies `other` callback holder and deactivates it.
	callback_holder & operator  =(callback_holder && other) noexcept;

private:
	/// \brief		Constructor, creates active callback holder.
	callback_holder(callback_register<T> * register_ptr, T * callback_ptr);

	/// \brief		Invokes a method from callback interface with given `args`,
	template<typename F, typename ... Args>
	void invoke(F method_ptr, Args ... args);

	/// \brief		Deactivates this callback holder.
	void dismiss();

private:
	mutable std::mutex m_mutex;

	callback_register<T> * m_register_ptr;
	T * m_callback_ptr;
};

template<typename T>
callback_holder<T>::callback_holder() :
	m_register_ptr(nullptr),
	m_callback_ptr(nullptr)
{
}

template<typename T>
callback_holder<T>::callback_holder(callback_holder && other) noexcept
{
	// Critical section:
	std::lock_guard<std::mutex> lock(m_mutex);

	m_register_ptr = other.m_register_ptr;
	m_callback_ptr = other.m_callback_ptr;

	other.dismiss();

	if (m_register_ptr == nullptr)
	{
		return;
	}

	m_register_ptr->replace(&other, this);
}

template<typename T>
callback_holder<T>::~callback_holder()
{
	// Critical section:
	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_register_ptr == nullptr)
	{
		return;
	}

	m_register_ptr->remove(this);
	dismiss();
}

template<typename T>
callback_holder<T> &
callback_holder<T>::operator  =(callback_holder && other) noexcept
{
	// Critical section:
	std::lock_guard<std::mutex> lock(m_mutex);

	m_register_ptr = other.m_register_ptr;
	m_callback_ptr = other.m_callback_ptr;

	other.dismiss();

	if (m_register_ptr == nullptr)
	{
		return (*this);
	}

	m_register_ptr->replace(&other, this);

	return (*this);
}

template<typename T>
callback_holder<T>::callback_holder(callback_register<T> * register_ptr, T * callback_ptr) :
	m_register_ptr(register_ptr),
	m_callback_ptr(callback_ptr)
{
}

template<typename T>
template<typename F, typename ... Args>
void 
callback_holder<T>::invoke(F method_ptr, Args ... args)
{
	// Critical section:
	std::lock_guard<std::mutex> lock(m_mutex);

	std::invoke(method_ptr, m_callback_ptr, std::forward<Args>(args) ...);
}

template<typename T>
void 
callback_holder<T>::dismiss()
{
	m_register_ptr = nullptr;
	m_callback_ptr = nullptr;
}
