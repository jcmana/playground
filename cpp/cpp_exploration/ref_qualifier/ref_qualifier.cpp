#include <iostream>

struct S 
{
	void f() &
	{ 
		std::cout << "lvalue\n";
	}

	void f() &&
	{
		std::cout << "rvalue\n";
	}
};

int main()
{
	S s;

	// l-value overload
	S & lvalue = s;
	lvalue.f();

	// r-value overload
	std::move(s).f();
}