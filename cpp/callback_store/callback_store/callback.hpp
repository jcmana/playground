#pragma once

#include <utility>

#include "../../link/link/link_element.hpp"

template<typename T>
class callback_guard;

/// \brief		Guarded interface callback.
template<typename T>
class callback : private link_element
{
public:
	friend class callback_guard<T>;

public:
	/// \brief		Constructor, creates inactive callback from `interface_ptr`.
	explicit callback(T * inteface_ptr);

	/// \brief		Invokes method from `T`, if the callback is still active.
	template<typename F, typename ... Args >
	void invoke(F method_ptr, Args && ... args);

private:
	T * m_interface_ptr;
};

#pragma region callback implementation:

template<typename T>
callback<T>::callback(T * inteface_ptr) :
	m_interface_ptr(inteface_ptr)
{
}

template<typename T>
template<typename F, typename ... Args >
void 
callback<T>::invoke(F method_ptr, Args && ... args)
{
	if (link_element::is_linked())
	{
		(m_interface_ptr->*method_ptr)(std::forward<Args>(args) ...);
	}
}

#pragma endregion
