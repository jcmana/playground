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
	
	return 0;
}
