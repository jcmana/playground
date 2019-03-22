#include "B.h"

#define _ITERATOR_DEBUG_LEVEL 2

#include <iostream>
#include <vector>

void TestB()
{
	std::vector<int> v = { 1, 2, 3 };

	for (int & i : v)
	{
		std::cout << i << " ";
	}

	std::cout << v[3] << " ";
}