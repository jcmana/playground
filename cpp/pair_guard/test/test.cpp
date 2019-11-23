#include <iostream>
#include <string>

#include "callback.hpp"

struct cb
{
	void method(const std::string & text)
	{
		std::cout << "cb::method() : " << text << std::endl;
	}
};

int main()
{
	cb i;
	callback<cb> c(&i);

	{
		callback_guard<cb> cg(&c);
		c.invoke(&cb::method, "test 1");
	}

	c.invoke(&cb::method, "test 2");

	return 0;
}