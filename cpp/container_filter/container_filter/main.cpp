#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

#include "filter_if.hpp"

int main()
{
	// create large integer with linear number increments from zero
	std::vector<int> integers(100'000'000);
	std::generate(integers.begin(), integers.end(), [n = 0]() mutable { return n++; });

	// make sum of all numbers filtered by predicate
	long long int sum = 0;
	for (auto i : filter_if(integers, [](const int & n) { return (n < 4 || n >= 6); }))
	{
		sum += i;
	}

	std::cout << sum << std::endl;

	std::getchar(); return 0;
}
