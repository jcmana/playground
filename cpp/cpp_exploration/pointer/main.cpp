#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


void a(const int * ptr)
{
	int local_int = 0;

	ptr = &local_int;
	//*ptr = 6;				// illegal operation
}

void b(int * const ptr)
{
	int local_int = 0;

	//ptr = &local_int;		// illegal operation
	*ptr = 6;
}

int main()
{
	// const typename * vs. typename * const
	{
		int my_int = 2;

		a(&my_int);
		b(&my_int);
	}

	// typename * const vs. typename[]
	{
		short int * const	int_ptr = new short int[3];
		short int			int_arr[3];

		// pointer/array access/modification:
		*(int_ptr) = 2;
		*(int_ptr + 1) = 4;
		int_ptr[2] = 6;

		*(int_arr) = 2;
		*(int_arr + 1) = 4;
		int_arr[2] = 6;

		// pointer/array reallocation:
		//int_ptr = nullptr;	// nullation not possible
		//int_arr = nullptr;	// nullation not possible

		delete[] int_ptr;

		// Outcome:
		// typename[] will be always compile-time known size, not reallocatable, but with accessible/modifiable items
		// typename * const is runtime allocated, not reallocatable, also with accessible/modifiable items
	}

#ifdef _DEBUG
	// memory-leak check result
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}