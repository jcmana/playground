#include <cmath>

#include <iostream>

void test_math_duplicities()
{
	double a, b, c;

	a = std::pow(2.6, 2);
	b = std::pow(2.4, 2);
	c = 3.14 * std::sqrt(16);

	std::cout << a << std::endl;
	std::cout << b << std::endl;
	std::cout << c << std::endl;
}

int main()
{
	test_math_duplicities();

	return 0;
}