#include <iostream>
#include <string>
#include <functional>

using TFunction = std::function<void()>;

void executor(TFunction fn)
{
	std::cout << "executor() start" << std::endl;

	fn();

	std::cout << "executor() end" << std::endl;
}

int main()
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

	//std::cout << "main: " << param << std::endl;

	getchar();	return 0;
}