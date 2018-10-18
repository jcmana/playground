#include <iostream>
#include <string>

struct aggregate
{
	int a;
	std::string b;

	void print()
	{
		std::cout << "{ a = " << a << ", b = " << b << " }" << std::endl;
	}
};


int main()
{
	aggregate x = { 3, "abcd" };	// aggregate initializtion

	x.print();

    std::getchar(); return 0;
}

