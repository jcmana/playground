#include <iostream>
#include <vector>

#include "containers/proxy.hpp"

void proxy_test()
{
	std::cout << "proxy test:" << std::endl;

	std::vector<int> v = { 1, 2, 3, 4 };

	double sum = 0;

	for (auto n : containers::proxy<std::vector<int>, double>(v))
	{
		sum = sum + n;
	}

	std::cout << "sum = " << sum << std::endl << std::endl;
}