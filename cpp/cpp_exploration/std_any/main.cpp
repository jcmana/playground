#if __cplusplus >= 201703L

#include <iostream>
#include <vector>
#include <any>

int main()
{
	std::any value;

	value = 35;
	std::cout << std::any_cast<int>(value) << std::endl;
	
	value = "asdf";
	std::cout << std::any_cast<const char *>(value) << std::endl;

	try
	{
		std::any_cast<double>(value);
	}
	catch (...)
	{
		// exception: std::bad_any_cast 
	}

	std::getchar();  return 0;
}

#else

int main()
{
	return 0;
}

#endif