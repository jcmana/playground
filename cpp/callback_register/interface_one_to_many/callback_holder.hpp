#pragma once

#include <algorithm>

/// \brief		Holds the registered callback and deactivates it on destruction.
template<typename T>
class callback_holder
{
public:
	template<typename T> friend class callback_register;

public:
	/// \brief		Default constructor, creates empty callback holder.
	callback_holder();

	/// \brief		Constructor, creates active callback holder.
	callback_holder(callback_register<T> * register_ptr, T * callback_ptr);

	/// \brief		Move constructor, copies `other` callback holder and deactivates it.
	callback_holder(callback_holder && other) noexcept;

	/// \brief		Destructor, deactivates the callback holder.
	~callback_holder();

	/// \brief		Move assignment, copies `other` callback holder and deactivates it.
	callback_holder & operator  =(callback_holder && other) noexcept;

private:
	/// \brief		Deactivates this callback holder.
	void dismiss();

private:
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
callback_holder<T>::callback_holder(callback_register<T> * register_ptr, T * callback_ptr) :
	m_register_ptr(register_ptr),
	m_callback_ptr(callback_ptr)
{
}

template<typename T>
callback_holder<T>::callback_holder(callback_holder && other) noexcept
{
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
void 
callback_holder<T>::dismiss()
{
	m_register_ptr = nullptr;
	m_callback_ptr = nullptr;
}
