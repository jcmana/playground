#pragma once

#include <algorithm>

template<typename T>
class callback_holder
{
public:
	template<typename T> friend class callback_register;

public:
	callback_holder() :
		m_register_ptr(nullptr),
		m_callback_ptr(nullptr)
	{
	}
	callback_holder(callback_register<T> * register_ptr, T * callback_ptr) :
		m_register_ptr(register_ptr),
		m_callback_ptr(callback_ptr)
	{
	}

	callback_holder(const callback_holder & other) = delete;
	callback_holder(callback_holder && other) noexcept :
		m_register_ptr(nullptr),
		m_callback_ptr(nullptr)
	{
		std::swap(m_register_ptr, other.m_register_ptr);
		std::swap(m_callback_ptr, other.m_callback_ptr);

		if (m_register_ptr == nullptr)
		{
			return;
		}

		m_register_ptr->replace(&other, this);
	}

	~callback_holder()
	{
		if (m_register_ptr == nullptr)
		{
			return;
		}

		m_register_ptr->remove(this);
		dismiss();
	}

	callback_holder & operator  =(const callback_holder & other) = delete;
	callback_holder & operator  =(callback_holder && other) noexcept;

private:
	void dismiss()
	{
		m_register_ptr = nullptr;
		m_callback_ptr = nullptr;
	}

private:
	callback_register<T> * m_register_ptr;
	T * m_callback_ptr;

	template<typename T> friend class callback_register;
};
