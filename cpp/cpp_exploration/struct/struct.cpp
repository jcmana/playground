#include <iostream>
#include <memory>
#include <list>
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
		aggregate x = {3, "abcd"};
		x.print();

        x = {7, "defg"};
        x.print();
	}

	// struct comparison:
	if (false)
	{
		pod a = { 1, 6.5, "omg" };
		pod b = { 1, 6.5, "omg" };

		// compile error: no operator ==() found
		//std::cout << (a == b ? "equal" : "not equal") << std::endl;
	}

	// const struct * and member access:
	if (true)
	{
		pod a = { 1, 6.5, "omg" };

		const pod * const_ptr = &a;

		// reading is allowed
		const int val = const_ptr->a;

		// modification is illegal
		//const_ptr->a = 3;
	}

    std::getchar(); return 0;
}

