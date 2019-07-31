#include <iostream>
#include <cmath>

int main()
{
	double positive_zero = + 0.0;
	double negative_zero = - 0.0;

	if (positive_zero > negative_zero)
	{
		std::cout << "positive_zero > negative_zero" << std::endl;
	}

	if (positive_zero < negative_zero)
	{
		std::cout << "positive_zero < negative_zero" << std::endl;
	}

	if (positive_zero == negative_zero)
	{
		std::cout << "positive_zero == negative_zero" << std::endl;
	}

	if (std::signbit(positive_zero))
	{
		std::cout << "positive_zero is signed" << std::endl;
	}

	if (std::signbit(negative_zero))
	{
		std::cout << "negative_zero is signed" << std::endl;
	}

	if (std::signbit((positive_zero - positive_zero)))
	{
		std::cout << "(positive_zero - positive_zero) is signed" << std::endl;
	}

	if (std::signbit((positive_zero * (- 1.0))))
	{
		std::cout << "(positive_zero * (- 1.0)) is signed" << std::endl;
	}

	return 0;
}
