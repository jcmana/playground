#pragma once

#include <utility>

#include "../../link/link/link_element.hpp"

template<typename T>
class callback_guard;

/// \brief		Guarded interface callback.
///
/// Callback is always mapped to a interface pointer and allows invoking methods
/// from it. Callback requires `callback_guard` to be alive, otherwise it can't
/// be invoked.
template<typename T>
class callback : private link_element
{
public:
	friend class callback_guard<T>;

public:
	/// \brief		Constructor, creates inactive callback from `T`.
	explicit callback(T & inteface_ref);

	/// \brief		Invokes method from `T`, if the callback is still active.
	template<typename F, typename ... Args >
	void invoke(F method_ptr, Args && ... args) const;

private:
	T & m_interface_ref;
};

#pragma region callback implementation:

template<typename T>
callback<T>::callback(T & inteface_ref) :
	m_interface_ref(inteface_ref)
{
}

template<typename T>
template<typename F, typename ... Args >
void 
callback<T>::invoke(F method_ptr, Args && ... args) const
{
	if (link_element::is_linked())
	{
		(m_interface_ref.*method_ptr)(std::forward<Args>(args) ...);
	}
}

#pragma endregion
