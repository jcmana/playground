#include "A.h"

#define _ITERATOR_DEBUG_LEVEL 0

#include <iostream>
#include <vector>

void TestA()
{
	std::vector<int> v = { 1, 2, 3 };

	for (int & i : v)
	{
		std::cout << i << " ";
	}

	std::cout << v[3] << " ";
}