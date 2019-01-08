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

class virtual_class
{
private:
	void empty_normal()
	{
	}

	virtual void empty_virtual()
	{
	}

public:
	void execute()
	{
		std::cout << "executing" << std::endl;

		//empty_normal();
		empty_virtual();
	}
};

void test_empty_virtual_method()
{
	virtual_class vc;
	vc.execute();
}

int main()
{
//	test_math_duplicities();
	test_empty_virtual_method();

	return 0;
}