#include <iostream>
#include <string>

#include "callback_register.hpp"
#include "callback_holder.hpp"

class callback_intf
{
public:
	virtual ~callback_intf() = default;

	virtual void action() const
	{
	}

	virtual void print(const std::string & text) const
	{
	}
};

class cb :
	public callback_intf
{
public:
	virtual void action() const override
	{
		std::cout << "cb::action()" << std::endl;
	}

	virtual void print(const std::string & text) const
	{
		std::cout << "cb::print() = " << text << std::endl;
	}
};

int main()
{
	cb c;

	callback_register<callback_intf> r;
	{
		callback_holder<callback_intf> h;
		h = r.subscribe(&c);

		r.notify(&callback_intf::action);
	}

	{
		callback_holder<callback_intf> h;
		h = r.subscribe(&c);

		r.notify(&callback_intf::action);
	}

	return 0;
}
