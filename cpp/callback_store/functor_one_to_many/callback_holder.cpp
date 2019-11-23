#include <algorithm>
#include <utility>
#include <exception>

#include "callback_register.h"
#include "callback_holder.h"

callback_holder::callback_holder() :
	register_ptr(nullptr)
{
}

callback_holder::callback_holder(callback_register * ptr, functor f) :
	register_ptr(ptr),
	callback_f(f)
{
}

callback_holder::callback_holder(callback_holder && other) noexcept :
	register_ptr(nullptr)
{
	std::swap(register_ptr, other.register_ptr);
	std::swap(callback_f, other.callback_f);

	if (register_ptr == nullptr)
	{
		return;
	}

	// Find other's holder pointer in register
	std::replace(register_ptr->holder_store.begin(), register_ptr->holder_store.end(), &other, this);
}

callback_holder::~callback_holder()
{
	if (register_ptr == nullptr)
	{
		return;
	}

	// Find this holder in register
	auto it = std::find(register_ptr->holder_store.begin(), register_ptr->holder_store.end(), this);

	// Remove this holder from register 
	register_ptr->holder_store.erase(it);
}

callback_holder & callback_holder::operator  =(callback_holder && other) noexcept
{
	std::swap(register_ptr, other.register_ptr);
	std::swap(callback_f, other.callback_f);

	if (register_ptr == nullptr)
	{
		return (*this);
	}

	// Replace other's holder pointer in register
	std::replace(register_ptr->holder_store.begin(), register_ptr->holder_store.end(), &other, this);

	return (*this);
}
