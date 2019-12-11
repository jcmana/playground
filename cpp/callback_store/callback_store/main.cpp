#include <iostream>

#include <thread>
#include <chrono>

#include "callback_store.hpp"
#include "atomic_callback_store.hpp"

struct callback_intf
{
	void method()
	{
		std::cout << "callback_intf::method()" << std::endl;
	}

	void method_slow()
	{
		std::cout << "callback_intf::method_slow()" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
};

int main()
{
	// Simple callback_store test:
	if (true)
	{
		callback_intf cia;
		callback_intf cib;

		callback_store<callback_intf> cs;
		cs.invoke(&callback_intf::method);

		{
			auto cg = cs.subscribe(cia);
			cs.invoke(&callback_intf::method);

			auto cg_move = std::move(cg);
			cs.invoke(&callback_intf::method);

			{
				auto cg = cs.subscribe(cib);
				cs.invoke(&callback_intf::method);

				cg.release();
				cs.invoke(&callback_intf::method);
			}

			cs.invoke(&callback_intf::method);

			const auto & cs_const_ref = cs;
			cs.invoke(&callback_intf::method);
		}

		auto cs_move = std::move(cs);
		cs.invoke(&callback_intf::method);
		cs_move.invoke(&callback_intf::method);
	}

	// Vector re-alloccation test:
	if (false)
	{
		std::size_t count = 100;

		std::vector<callback_intf> ci;
		callback_store<callback_intf> cs;

		for (std::size_t n = 0; n < count; ++n)
		{
			auto it = ci.emplace(ci.end());
			cs.subscribe(*it);
		}
	}

	// Simple atomic_callback_store test:
	if (false)
	{
		callback_intf ci;

		atomic_callback<callback_intf> ac(&ci);
		atomic_callback_guard<callback_intf> acg(&ac);

		ac.invoke(&callback_intf::method);
	}

	// Multi-thread atomic_callback_store test:
	if (false)
	{
		callback_intf ci;

		atomic_callback<callback_intf> ac(&ci);

		std::thread ta([&]
		{
			atomic_callback_guard<callback_intf> acg(&ac);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		});

		std::thread tb([&]
		{ 
			ac.invoke(&callback_intf::method_slow);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			ac.invoke(&callback_intf::method_slow);
		});

		auto ac_move = std::move(ac);
		ac = std::move(ac_move);

		ta.join();
		tb.join();
	}
	
	return 0;
}
