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

struct pod
{
	int a;
	double b;
	std::string c;
};


int main()
{
	// aggregate initializtion:
	if (false)
	{
		aggregate x = { 3, "abcd" };
		x.print();
	}

	// struct comparison:
	if (true)
	{
		pod a = { 1, 6.5, "omg" };
		pod b = { 1, 6.5, "omg" };

		// compile error: no operator ==() found
		//std::cout << (a == b ? "equal" : "not equal") << std::endl;
	}

    std::getchar(); return 0;
}

