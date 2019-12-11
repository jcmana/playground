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

	/// \brief			Subscribes `interface_ref` for notifications.
	callback_guard<T> subscribe(T & interface_ref);

	/// \brief			Invokes a method from `T` on each subscribed callback.
	template<typename F, typename ... Args >
	void invoke(F method_ptr, Args && ... args) const;

private:
	std::vector<callback<T>> m_callback_store;
};

#pragma region callback_store implementation:

template<typename T>
callback_guard<T> 
callback_store<T>::subscribe(T & interface_ref)
{
	//auto it = m_callback_store.emplace(m_callback_store.end(), interface_ref);
	//return callback_guard<T>(&*it);

	callback<T> c(interface_ref);
	callback_guard<T> g(&c);
	m_callback_store.push_back(std::move(c));
	return g;
}

template<typename T>
template<typename F, typename ... Args >
void 
callback_store<T>::invoke(F method_ptr, Args && ... args) const
{
	for (auto & callback : m_callback_store)
	{
		callback.invoke(method_ptr, std::forward<Args>(args) ...);
	}
}

#pragma endregion
