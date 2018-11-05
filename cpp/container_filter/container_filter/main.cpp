#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

#include "filter.hpp"

int main()
{
	// create large integer with linear number increments from zero
	std::vector<int> integers(100'000'000);
	std::generate(integers.begin(), integers.end(), [n = 0]() mutable { return n++; });

	// setup filter 
	auto f = make_filter(integers, [](const int & n){ return (n < 4 || n >=6); });

	// make sum of all numbers from filter
	long long int sum = 0;
	for (auto i : f)
	{
		sum += i;
	}

	std::cout << sum << std::endl;

	std::getchar(); return 0;
}
