#include <iostream>
#include <string>

struct value
{
	int a = 7;
	int b = 143;
};

struct iterator_base
{
	value * operator->()
	{
		return &v;
	}
	
	value v;
};

struct iterator
{
	iterator_base & operator->()
	{
		return itb;
	}

	iterator_base itb;
};

struct pair
{
	double * first;
	double * second;
};

struct blob
{
	int a;
	int b;
	pair * c;
};

int main()
{
	// operator->() chaining:
	{
		// for structs (won't work):
		if (false)
		{
			pair x = { new double(3.2), new double(7.5) };
			pair * ptr_x = &x;
		
			blob y = { 1, 134, ptr_x };
			blob * ptr_y = &y;

			blob ** ptr_ptr_y = &ptr_y;

			std::cout << *ptr_y->c->second;
		}

		// for custom types:
		if (true)
		{
			iterator it;

			std::cout << it->a << std::endl;
		}
	}

	std::getchar(); return 0;
}
