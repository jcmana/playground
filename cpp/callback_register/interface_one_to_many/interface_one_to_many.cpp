#include <iostream>

#include <thread>
#include <string>
#include <chrono>

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

struct intf_slow
{
	void method()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::cout << "intf::method()" << std::endl;
	}
};

int main()
{
	// Single-thread sanity test:
	if (false)
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
	}

	// Multi-thread sync test:
	if (true)
	{
		callback_register<intf_slow> cr;

		std::thread a([&]
		{
			cr.notify(&intf_slow::method);

		});

		std::thread b([&]
		{

			intf_slow cb;
			auto h = cr.subscribe(&cb);
		});

		a.join();
		b.join();
	}

	return 0;
}
