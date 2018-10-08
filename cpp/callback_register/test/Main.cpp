#include <iostream>
#include <functional>

#include "../callback_register/CallbackRegister.h"


void paramless()
{
	asw::callback_register::functional<> cr;

	try
	{
		cr.Register([]()
		{
			std::cout << "prvni" << std::endl;
		});

		cr.ExecuteAll();
	}
	catch (asw::callback_register::functional<int>::InvalidCallbackID & e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void parametric()
{
	asw::callback_register::functional<int> cr;

	try
	{
		auto i1 = cr.Register([](int n)
		{
			std::cout << "prvni " << n << std::endl;
		});

		auto i2 = cr.Register([](int n)
		{
			std::cout << "druhy " << n << std::endl;
		});

		cr.ExecuteAll(231321);

		cr.Execute(i1, 12);

		cr.Unregister(i1);

		cr.ExecuteAll(36541654165165);

		cr.Unregister(162);

		cr.Clear();

		cr.Execute(7, 12);
	}
	catch (asw::callback_register::functional<int>::InvalidCallbackID & e)
	{
		std::cerr << e.what() << std::endl;
	}
}

int main()
{
	paramless();

	parametric();

	return 0;
}
