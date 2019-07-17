#include <utility>

#include "callback_register.h"

callback_register::callback_register(callback_register && other) noexcept
{
	std::swap(other.holder_store, holder_store);
}

callback_register::~callback_register()
{
	holder_store.clear();
}

callback_register & callback_register::operator =(callback_register && other) noexcept
{
	std::swap(other.holder_store, holder_store);

	return (*this);
}

callback_holder callback_register::subscribe(callback_holder::functor f)
{
	callback_holder holder(this, f);
	holder_store.push_back(&holder);
	return std::move(holder);
}

void callback_register::notify() const
{
	for (callback_holder * hp : holder_store)
	{
		hp->callback_f();
	}
}
