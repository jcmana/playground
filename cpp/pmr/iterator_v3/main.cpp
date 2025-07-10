#include <iostream>

#include "my_impl.h"

int main()
{
	my_impl mi;

	auto it = mi.begin(); 
	auto it_end = mi.end();

	++it;

	auto & ref = *it;
	ref = 200;

	it++;

	auto * ptr = it.operator->();
	(*ptr) = 8;

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