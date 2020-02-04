#pragma once

#include <utility>
#include <type_traits>

#include "../../link/link/link_element.hpp"

template<typename T>
class callback_guard;

/// \brief		Guarded interface callback.
///
/// Callback is always mapped to a interface pointer and allows invoking methods
/// from it. Callback requires `callback_guard` to be alive, otherwise it can't
/// be invoked.
///
/// Implementation refuses lambdas or binders, because in this context they are 
/// typical source of issues.
template<typename T>
class callback : private link_element
{
public:
	friend class callback_guard<T>;

public:
	/// \brief		Constructor, creates inactive callback from `T`.
	explicit callback(T & inteface_ref);

	/// \brief		Invokes method from `T`, if the callback is still active.
	template<typename = std::enable_if_t<std::is_class<T>::value>, typename F, typename ... Args>
	void invoke(F method_ptr, Args && ... args) const;

    /// \brief		Invokes function `T`, if the callback is still active.
    template<typename = std::enable_if_t<std::is_function<T>::value>, typename ... Args>
    void invoke(Args && ... args) const;

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
template<typename, typename F, typename ... Args>
void 
callback<T>::invoke(F method_ptr, Args && ... args) const
{
	if (link_element::is_linked())
	{
		(m_interface_ref.*method_ptr)(std::forward<Args>(args) ...);
	}
}

template<typename T>
template<typename, typename ... Args>
void 
callback<T>::invoke(Args && ... args) const
{
    if (link_element::is_linked())
    {
        m_interface_ref(std::forward<Args>(args) ...);
    }
}

#pragma endregion
