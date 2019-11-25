#include <iostream>

#include "callback_store.hpp"
#include "atomic_callback_store.hpp"

struct callback_intf
{
	void method()
	{
		std::cout << "callback_intf::method()" << std::endl;
	}
};

int main()
{
	// Simple callback_store test:
	if (false)
	{
		callback_intf cia;
		callback_intf cib;

		callback_store<callback_intf> cs;
		cs.invoke(&callback_intf::method);

		{
			auto cg = cs.subscribe(&cia);
			cs.invoke(&callback_intf::method);

			auto cg_move = std::move(cg);
			cs.invoke(&callback_intf::method);

			{
				auto cg = cs.subscribe(&cib);
				cs.invoke(&callback_intf::method);
			}

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
			cs.subscribe(&*it);
		}
	}

	// atomic_callback_store test:
	if (true)
	{
		callback_intf ci;

		atomic_callback<callback_intf> ac(&ci);
		atomic_callback_guard<callback_intf> acg(&ac);

		ac.invoke(&callback_intf::method);
	}
	
	return 0;
}
