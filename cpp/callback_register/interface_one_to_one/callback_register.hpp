#pragma once

template<typename Interface>
class callback_holder;

template<typename Interface>
class callback_register
{
public:
	callback_register() :
		m_holder_ptr(nullptr)
	{
	}

	callback_register(const callback_register & other) = delete;
	callback_register(callback_register && other) noexcept
	{
		m_holder_ptr(other.m_holder_ptr);
		other.m_holder_ptr = nullptr;
	}

	~callback_register()
	{
		if (m_holder_ptr == nullptr)
		{
			return;
		}

		m_holder_ptr->m_register_ptr = nullptr;
		m_holder_ptr = nullptr;
	}

	callback_register & operator =(const callback_register & other) = delete;
	callback_register & operator =(callback_register && other) noexcept
	{
		return (*this);
	}

	callback_holder<Interface> subscribe(Interface * callback_ptr)
	{
		callback_holder<Interface> holder(this, callback_ptr);
		m_holder_ptr = &holder;
		return holder;
	}
	
	template<typename Function, typename ... Args >
	void notify(Function && method_ptr, Args && ... args) const
	{
		if (m_holder_ptr == nullptr)
		{
			return;
		}

		(m_holder_ptr->m_callback_ptr->*method_ptr)(args ...);
	}

private:
	callback_holder<Interface> * m_holder_ptr;

template<typename Interface>
friend class callback_holder;
};