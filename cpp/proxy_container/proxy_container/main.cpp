#include <iostream>
#include <vector>

#include "proxy_container.hpp"

int main()
{
	std::vector<int> v = { 1, 2, 3, 4 };


	for (auto n : proxy_container<std::vector<int>, double>(v))
	{
		n = n + 3;
	}

	std::getchar();  return 0;
}