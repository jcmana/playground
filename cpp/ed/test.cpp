#include <iostream>

#include "event_distributor_impl.h"


// Used to demonstrate passing param to to callback:
using random_value_t = int;

// Used to demonstrate working with context:
using context_t = std::string;


// Test callback interface:
struct callback_intf
{
	virtual void on_callback(random_value_t val) = 0;
};

// Test callback handling class:
struct test_class:
	callback_intf
{
	virtual void on_callback(random_value_t val) override
	{
		callback_called_value = val;
	}

	random_value_t callback_called_value = 0;
};


// Event distributors instancies:
ed::impl<callback_intf> event_distributor;
ed::impl_ctx<callback_intf, context_t> event_distributor_string_ctx;

// Helpers:
void notify_all(random_value_t val)
{
	event_distributor.notify_all([val](callback_intf* p) 
	{ 
		p->on_callback(val); 
	});
}
void notify_all_ctx(random_value_t val, const context_t& ctx)
{
	event_distributor_string_ctx.notify_all([val](callback_intf* p, const context_t& ctx)
	{
		p->on_callback(val);
	}, ctx);
}


int main(int argc, char ** argv)
{
	test_class t;

	/////////////////////////////////////////////////////////////
	// Event distributor without context:
	notify_all(1); // Nobody listening.
	_ASSERT(t.callback_called_value == 0);

	{
		// Register:
		auto reg = event_distributor.register_callback(t);
		notify_all(2); // There is listener.
		_ASSERT(t.callback_called_value == 2);
		
		// Unregister.
	}

	notify_all(3); // Nobody listening.
	_ASSERT(t.callback_called_value == 2); // Value does not changed.


	/////////////////////////////////////////////////////////////
	// Event distributor with context:
	notify_all_ctx(4, "1"); // Nobody listening.
	_ASSERT(t.callback_called_value == 2); // Value does not changed.

	{
		// Register under ctx "2":
		auto reg = event_distributor_string_ctx.register_callback(t, "2");
		notify_all_ctx(5, "1"); // Nobody listening on ctx "1".
		_ASSERT(t.callback_called_value == 2); // Value does not changed.

		notify_all_ctx(6, "2"); // There is listener for ctx "2".
		_ASSERT(t.callback_called_value == 6); // Value does not changed.

		// Unregister.
	}

	notify_all_ctx(7, "2"); // Nobody listening.
	_ASSERT(t.callback_called_value == 6); // Value does not changed.
}
