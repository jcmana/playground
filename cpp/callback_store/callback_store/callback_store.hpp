#pragma once

#include "callback.hpp"
#include "callback_guard.hpp"

template<typename T>
class callback_store
{
public:
	callback_store() :
		m_callback(nullptr)
	{
	}

	callback_guard<T> subscribe(T * interface_ptr)
	{
		m_callback = callback<T>(interface_ptr);
		return callback_guard<T>(&m_callback);
	}

	template<typename F, typename ... Args >
	void notify(F method_ptr, Args && ... args)
	{
		m_callback.invoke(method_ptr, std::forward<Args>(args) ...);
	}

private:
	callback<T> m_callback;
};