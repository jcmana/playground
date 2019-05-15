#pragma once

#include <vector>
#include <functional>

#include "callback_holder.h"

//
// MISSING THREAD-SAFETY 
//

class callback_register
{
public:
	callback_register() = default;
	callback_register(const callback_register & other) = delete;
	callback_register(callback_register && other) noexcept;

	~callback_register();

	callback_register & operator =(const callback_register & other) = delete;
	callback_register & operator =(callback_register && other) noexcept;

	callback_holder subscribe(callback_holder::functor f);
	void notify() const;

private:
	std::vector<callback_holder *> holder_store;

friend class callback_holder;
};
