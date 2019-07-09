#include <iostream>
#include <utility>

int main()
{
	// Binding to lvalue:
	{
		int n = 1;
		int & l = n;
		//int && r = n;		// error
	}

	// Binding to rvalue:
	{
		//int& l = 1;		// error
		int && r = 1;

		r = r + 3;
	}

	return 0;
}