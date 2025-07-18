#include <iostream>

#include "my_impl.h"

int main()
{
	my_impl mi;

	auto it = mi.begin(); 
	auto it_end = mi.end();

	auto it_copy = it;
	std::cout << *it_copy << std::endl;

	++it;

	auto & ref = *it;
	ref = 200;

	it++;

	auto * ptr = it.operator->();
	(*ptr) = 8;

	if (it == it_end)
	{
		std::cout << "it == it_end" << std::endl;
	}

	if (it != it_end)
	{
		std::cout << "it != it_end" << std::endl;
	}

	for (const auto & n : mi)
	{
		std::cout << n << std::endl;
	}
}