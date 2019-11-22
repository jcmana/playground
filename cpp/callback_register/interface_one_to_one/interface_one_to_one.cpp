#include <iostream>
#include <vector>
#include <string>
#include <utility>

#include "callback.hpp"

struct cb
{
	void action() const
	{
		std::cout << "cb::action()" << std::endl;
	}

	void print(const std::string & text) const
	{
		std::cout << "cb::print() = " << text << std::endl;
	}
};

int main()
{
	// Construction/destruction order test:
	if (false)
	{
		cb i;

		{
			callback_guard<cb> cg;
			callback<cb> c(&i, &cg);
		}

		{
			callback<cb> c(&i);
			callback_guard<cb> cg(&c);
		}
	}

	// Move test:
	if (false)
	{
		cb i;
		callback_guard<cb> cg;
		callback<cb> c(&i, &cg);

		auto c_moved = std::move(c);
		auto cg_moved = std::move(cg);
	}

	// Notify test:
	if (false)
	{
		cb i;
		callback_guard<cb> cg;
		callback<cb> c(&i, &cg);

		c.invoke(&cb::print, "test");
	}

	// callback_guard dtor test:
	if (false)
	{
		cb i;
		callback<cb> c(&i);

		{
			callback_guard<cb> cg(&c);
			c.invoke(&cb::print, "test 1");
		}

		c.invoke(&cb::print, "test 2");
	}

	// callback dtor test:
	if (false)
	{
		cb i;
		callback_guard<cb> cg;

		{
			callback<cb> c(&i, &cg);
			c.invoke(&cb::print, "test 1");
		}
	}

	// std::vector test:
	if (true)
	{
		cb i;
		std::vector<callback<cb>> vc;
		vc.emplace_back(&i);
		callback_guard<cb> cg(&vc[0]);
		vc.reserve(300);
		vc[0].invoke(&cb::action);
		vc.shrink_to_fit();
		vc[0].invoke(&cb::action);
	}

	return 0;
}
