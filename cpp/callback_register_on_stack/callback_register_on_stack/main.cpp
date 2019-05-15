#include <iostream>

#include <atomic>
#include <vector>
#include <functional>
#include <algorithm>

#include "callback_holder.h"
#include "callback_register.h"

int main()
{
	callback_holder a;

	{
		callback_register r;

		r.notify();

		a = r.subscribe([]
		{
			std::cout << "omg a" << std::endl;
		});

		{	
			callback_holder b = r.subscribe([]
			{
				std::cout << "omg b" << std::endl;
			});

			r.notify();
		}

		r.notify();
	}

	// move constructor, move assignment
	{
		callback_holder a;
		callback_holder b;

		a = std::move(b);

		callback_holder c(std::move(a));
	}

	std::getchar(); return 0;
}