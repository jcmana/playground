#include <iostream>

#include "my_impl.h"

int main()
{
	my_impl mi;
	auto it = mi.begin(); 
	auto it_end = mi.end();

	++it;
	//--it;
	const auto v = *it;

	if (it == it_end)
	{
	}

	if (it != it_end)
	{
	}

	for (const auto & n : mi)
	{
		std::cout << n << std::endl;
	}
}