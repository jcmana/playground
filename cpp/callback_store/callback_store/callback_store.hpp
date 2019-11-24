#pragma once

#include <vector>

#include "callback.hpp"
#include "callback_guard.hpp"

/// \brief			Stores callbacks and allows 
template<typename T>
class callback_store
{
public:
	/// \brief			Default constructor, creates store with no subscriptions.
	callback_store() = default;

	/// \brief			Subscribes `interface_ptr` for notifications.
	callback_guard<T> subscribe(T * interface_ptr);

	/// \brief			Invokes a method from `T` on subscribed callbacks.
	template<typename F, typename ... Args >
	void invoke(F method_ptr, Args && ... args);

private:
	std::vector<callback<T>> m_callback_store;
};

template<typename T>
callback_guard<T> 
callback_store<T>::subscribe(T * interface_ptr)
{
	auto it = m_callback_store.emplace(m_callback_store.end(), interface_ptr);
	return callback_guard<T>(&*it);
}

template<typename T>
template<typename F, typename ... Args >
void 
callback_store<T>::invoke(F method_ptr, Args && ... args)
{
	for (auto & callback : m_callback_store)
	{
		callback.invoke(method_ptr, std::forward<Args>(args) ...);
	}
}