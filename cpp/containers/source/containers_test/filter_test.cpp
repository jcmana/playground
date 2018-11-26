#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

#include "containers/filter_if.hpp"

void filter_test()
{
	std::cout << "filter test:" << std::endl;

	// create large integer with linear number increments from zero
	std::vector<int> integers(100'000'000);
	std::generate(integers.begin(), integers.end(), [n = 0]() mutable { return n++; });

	// make sum of all numbers filtered by predicate
	long long int sum = 0;
	for (auto i : containers::filter_if(integers, [](const int & n) { return (n < 4 || n >= 6); }))
	{
		sum += i;
	}

	std::cout << "sum = " << sum << std::endl << std::endl;
}
