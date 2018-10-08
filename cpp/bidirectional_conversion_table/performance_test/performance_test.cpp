#include <string>
#include <chrono>
#include <ctime>
#include <iostream>

#include "../bidirectional_conversion_table/bidirectional_conversion_table.h"

static constexpr int iterations = 100'000;

int main()
{
	int a;
	double b;

	for (int i = 0; i < iterations; ++i)
	{
		bidirectional_conversion_table<int, double> ct(
		{
			{ 1, 5.2 },
			{ 2, 8.4 },
			{ 3, 1.0 }
		});

		a = ct.convert(5.2);
		a = ct.convert(8.4);
		a = ct.convert(1.0);

		b = ct.convert(1);
		b = ct.convert(2);
		b = ct.convert(3);
	}

	// HOW TO HECK???

	return 0;
}