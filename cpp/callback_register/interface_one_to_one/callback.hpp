#pragma once

#include <functional>
#include <utility>

template<typename T>
class callback
{
public:
	template<typename T>
	friend class callback_guard;

public:
	callback(T * inteface_ptr) :
		m_interface_ptr(inteface_ptr),
		m_guard_ptr(nullptr)
	{
	}

	callback(T * inteface_ptr, callback_guard<T> * guard_ptr) :
		m_interface_ptr(inteface_ptr),
		m_guard_ptr(guard_ptr)
	{
		if (m_guard_ptr)
		{
			m_guard_ptr->m_callback_ptr = this;
		}
	}

	callback(const callback & other) = delete;

	callback(callback && other) noexcept
	{
		m_interface_ptr = other.m_interface_ptr;

		m_guard_ptr = other.m_guard_ptr;
		other.m_guard_ptr = nullptr;

		if (m_guard_ptr)
		{
			m_guard_ptr->m_callback_ptr = this;
		}
	}

	~callback()
	{
		if (m_guard_ptr)
		{
			m_guard_ptr->m_callback_ptr = nullptr;
			m_guard_ptr = nullptr;
		}
	}

	callback & operator =(const callback & other) = delete;

	callback & operator =(callback && other) noexcept
	{
		m_interface_ptr = other.m_interface_ptr;

		m_guard_ptr = other.m_guard_ptr;
		other.m_guard_ptr = nullptr;

		if (m_guard_ptr)
		{
			m_guard_ptr->m_callback_ptr = this;
		}

		return (*this);
	}

	template<typename F, typename ... Args >
	void invoke(F method_ptr, Args && ... args)
	{
		if (m_guard_ptr)
		{
			std::invoke(method_ptr, m_interface_ptr, std::forward<Args>(args) ...);
		}
	}

private:
	T * m_interface_ptr;

	callback_guard<T> * m_guard_ptr;
};

#include "callback_guard.hpp"