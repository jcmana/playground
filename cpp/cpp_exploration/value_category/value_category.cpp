#include <iostream>
#include <utility>

int main()
{
	// Binding to lvalue:
	{
		int n = 1;
		int& l = n;
		//int&& r = n;		// error

		__asm nop;
	}

	// Binding to rvalue:
	{
		//int& l = 1;		// error
		int&& r = 1;

		r = r + 3;

		__asm nop;
	}

	return 0;
}