#include <iostream>

#include <thread>
#include <string>
#include <chrono>

#include "callback_store.hpp"
#include "atomic_callback_store.hpp"

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
		__nop();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		__nop();
	}
};

int main()
{
	// Single-thread sanity test:
	if (false)
	{
		callback_store<intf> cr;

		intf a;
		intf b;

		auto ch_a = cr.subscribe(&a);

		{
			callback_guard<intf> ch_assigned;

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

	if (true)
	{
		atomic_callback_store<intf> cr;

		intf a;
		auto h = cr.subscribe(&a);

		atomic_callback_store<intf> cr_moved = std::move(cr);
		cr_moved.notify(&intf::method);
	}

	// Multi-thread sync test:
	if (false)
	{
		callback_store<intf_slow> cr;

		callback_guard<intf_slow> ch_a;
		callback_guard<intf_slow> ch_c;

		std::thread a([&]
		{
			intf_slow cb;
			auto h = cr.subscribe(&cb);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			ch_a = std::move(h);
		});

		std::thread b([&]
		{
			cr.notify(&intf_slow::method);
		});

		std::thread c([&]
		{
			intf_slow cb;
			auto h = cr.subscribe(&cb);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			ch_c = std::move(h);
		});

		a.join();
		b.join();
		c.join();
	}

	return 0;
}
