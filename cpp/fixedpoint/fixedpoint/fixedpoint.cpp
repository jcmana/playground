#include <iostream>

#include "fixedpoint.hpp"

int main()
{
	fixedpoint<unsigned int, 2, -24> f(16'777'216);
	//f = 32.78;

	std::cout << double(f) << std::endl;

	auto s = f + f;

	return 0;
}
