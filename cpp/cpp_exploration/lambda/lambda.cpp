#include <iostream>
#include <string>
#include <functional>
#include <memory>

#ifndef NDEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

using TFunction = std::function<void()>;

void executor(TFunction fn)
{
	std::cout << "executor() start" << std::endl;

	fn();

	std::cout << "executor() end" << std::endl;
}

int main()
{
	// capture by copy:
	if (false)
	{
		TFunction f;

		{
			std::string param = "hovno";

			f = [param]
			{
				std::string temp_param = param;

				std::cout << "lambda: " << param << std::endl;
			};
		}

		executor(f);
	}

	// caputuring shared_ptr by copy:
	if (true)
	{
		struct test
		{
			test()
			{
				std::cout << "test::ctor()" << std::endl;
			}

			~test()
			{
				std::cout << "test::dtor()" << std::endl;
			}
		};

		auto sp = std::make_shared<test>();

		auto lambda = [sp]
		{
			sp.get();
		};

		lambda();
	}

#ifndef NDEBUG
	_CrtDumpMemoryLeaks();
#endif
}