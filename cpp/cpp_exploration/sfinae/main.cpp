#include <string>

#pragma warning(disable: 4101)

#include "sfinae.hpp"

int main()
{
	{
		// compiles:
		outter<char, int>::inner<int, double>::type t;

		// fails: 
		//outter<char>::inner<int, double>::type t;
	}

	{
		// compiles:
		type_array_fail<char *>();

		// fails: array of void:
		//type_array_fail<void>();

		// fails: array of reference
		//type_array_fail<char &>();

		// compiles:
		size_array_fail<5>();

		// fails: zero array size:
		//size_array_fail<0>();

		// fails: negative array size:
		//size_array_fail<-6>();
	}

	{
		// compiles:
		struct member_type_present
		{
			struct iterator
			{
			};
		};
		member_fail<member_type_present> m1;

		// fails: type 'iterator' is missing:
		struct member_type_missing
		{
			struct const_iterator
			{
			};
		};
		//member_fail<member_type_missing> m2;
	}

	return 0;
}