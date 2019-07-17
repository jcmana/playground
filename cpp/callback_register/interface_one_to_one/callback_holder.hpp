#pragma once

template<typename Interface>
class callback_register;

template<typename Interface>
class callback_holder
{
public:
	callback_holder() :
		m_callback_ptr(nullptr),
		m_register_ptr(nullptr)
	{
	}

	callback_holder(callback_register<Interface> * register_ptr, Interface * callback_ptr) :
		m_register_ptr(register_ptr),
		m_callback_ptr(callback_ptr)
	{
	}

	callback_holder(const callback_holder & other) = delete;
	callback_holder(callback_holder && other) noexcept
	{
		m_register_ptr = other.m_register_ptr;
		other.m_register_ptr = nullptr;

		m_callback_ptr = other.m_callback_ptr;
		other.m_callback_ptr = nullptr;

		m_register_ptr->m_holder_ptr = this;
	}

	~callback_holder()
	{
		if (m_register_ptr != nullptr)
		{
			m_register_ptr->m_holder_ptr = nullptr;
		}

		m_register_ptr = nullptr;
		m_callback_ptr = nullptr;
	}

	callback_holder & operator =(const callback_holder & other) = delete;
	callback_holder & operator =(callback_holder && other) noexcept
	{
		m_register_ptr = other.m_register_ptr;
		other.m_register_ptr = nullptr;

		m_callback_ptr = other.m_callback_ptr;
		other.m_callback_ptr = nullptr;

		m_register_ptr->m_holder_ptr = this;

		return (*this);
	}

private:
	callback_register<Interface> * m_register_ptr;
	Interface * m_callback_ptr;

template<typename Interface>
friend class callback_register;
};