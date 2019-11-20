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
		callback_holder<intf> ch_assigned;

		{
			auto ch_b = cr.subscribe(&b);
			cr.notify(&intf::method);
		
			auto ch_b_moved = std::move(ch_b);		
			cr.notify(&intf::parametric_method, "test 1");

			ch_assigned = std::move(ch_b_moved);
			cr.notify(&intf::parametric_method, "test 2");
		}

		cr.notify(&intf::method);
	}

	cr.notify(&intf::parametric_method, "test 3");

	auto cr_moved = std::move(cr);
	cr_moved.notify(&intf::parametric_method, "test 4");

	cr.notify(&intf::method);

	return 0;
}
