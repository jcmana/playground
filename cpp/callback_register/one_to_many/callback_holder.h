#pragma once

class callback_register;

//
// MISSING THREAD-SAFETY 
//

class callback_holder
{
public:
	using functor = std::function<void()>;

public:
	callback_holder();
	callback_holder(callback_register * ptr, functor f);
	callback_holder(const callback_holder & other) = delete;
	callback_holder(callback_holder && other) noexcept;

	~callback_holder();

	callback_holder & operator =(const callback_holder & other) = delete;
	callback_holder & operator =(callback_holder && other) noexcept;

private:
	callback_register * register_ptr;
	functor callback_f;

friend class callback_register;
};
