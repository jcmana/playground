#include <iostream>
#include <string>

#include "callback_holder.hpp"
#include "callback_register.hpp"

struct intf
{
	void method()
	{
		std::cout << "intf::method()" << std::endl;
	}

	void parametric_method(const std::string & text)
	{
		std::cout << "intf::parametric_method(): " << text << std::endl;
	}
};

int main()
{
	callback_register<intf> cr;

	intf a;
	intf b;

	auto ch_a = cr.subscribe(&a);

	{
		auto ch_b = cr.subscribe(&b);

		cr.notify(&intf::method);
		cr.notify(&intf::parametric_method, "test");
	}

	cr.notify(&intf::method);

	auto cr_moved = std::move(cr);

	return 0;
}
