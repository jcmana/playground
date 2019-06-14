#include <iostream>
#include <vector>

#include "containers/cursor.hpp"
#include "tests.h"

void cursor_test()
{
	std::vector<int> a = { 1, 2, 3 };
	std::vector<int> b = { 10 };
	std::vector<int> c = { 100 };
	std::vector<int> d = { 1000, 1001 };

	using iterator = std::vector<int>::iterator;

	std::vector<containers::cursor<iterator>> stack = 
	{
		{ a.begin(), a.end(), a.begin() },
		{ b.begin(), b.end(), b.begin() }
	};

	using stack_iterator = std::vector<containers::cursor<iterator>>::iterator;

	containers::cursor<stack_iterator> stack_c(stack.begin(), stack.end(), stack.begin());

	for (;;)
	{
		if (stack_c == false)
		{
			stack_c.restart();
		}

		auto cursor = (*stack_c);

		++cursor;

		if (cursor == false)
		{
			cursor.restart();
			++stack_c;
		}

		std::cout << **stack_c << std::endl;
	}
}