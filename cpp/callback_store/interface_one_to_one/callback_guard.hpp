#pragma once

template<typename T>
class callback_guard
{
public:
	template<typename T>
	friend class callback;

public:
	callback_guard() :
		m_callback_ptr(nullptr)
	{
	}

	callback_guard(callback<T> * callback_ptr) :
		m_callback_ptr(callback_ptr)
	{
		if (m_callback_ptr)
		{
			m_callback_ptr->m_guard_ptr = this;
		}
	}

	callback_guard(const callback_guard & other) = delete;

	callback_guard(callback_guard && other) noexcept
	{
		m_callback_ptr = other.m_callback_ptr;
		other.m_callback_ptr = nullptr;

		if (m_callback_ptr)
		{
			m_callback_ptr->m_guard_ptr = this;
		}
	}

	~callback_guard()
	{
		if (m_callback_ptr)
		{
			m_callback_ptr->m_guard_ptr = nullptr;
			m_callback_ptr = nullptr;
		}
	}

	callback_guard & operator  =(const callback_guard & other) = delete;

	callback_guard & operator  =(callback_guard && other) noexcept
	{
		m_callback_ptr = other.m_callback_ptr;
		other.m_callback_ptr = nullptr;

		if (m_callback_ptr)
		{
			m_callback_ptr->m_guard_ptr = this;
		}

		return (*this);
	}

private:
	callback<T> * m_callback_ptr;
};