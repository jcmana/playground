#include <iostream>

#include "callback_store.hpp"

struct callback_intf
{
	void method()
	{
		std::cout << "callback_intf::method()" << std::endl;
	}
};

int main()
{
	callback_intf ci;

	callback_store<callback_intf> cs;
	cs.notify(&callback_intf::method);

	{
		auto cg = cs.subscribe(&ci);
		cs.notify(&callback_intf::method);

		auto cg_move = std::move(cg);
		cs.notify(&callback_intf::method);
	}

	cs.notify(&callback_intf::method);

	auto cs_move = std::move(cs);
	cs.notify(&callback_intf::method);
	cs_move.notify(&callback_intf::method);
	
	return 0;
}
