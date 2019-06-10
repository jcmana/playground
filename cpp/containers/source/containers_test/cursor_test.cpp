#include <iostream>
#include <vector>

#include "containers/cursor.hpp"
#include "tests.h"

void cursor_test()
{
	std::vector<int> v = { 1, 2, 4, 8, 16 };
	containers::cursor<std::vector<int>::iterator> c(v.begin(), v.end(), v.begin());

	for (; c; ++c)
	{
		std::cout << *c << std::endl;
	}

	for (c.restart(); c; ++c)
	{
		std::cout << *c << std::endl;
	}

	for (c.finish(); c; ++c)
	{
		std::cout << *c << std::endl;
	}
}