#include <cassert>
#include <iostream>
#include <algorithm>

struct vector
{
	double x;
	double y;
};

inline bool comp(vector a, vector b)
{
	if (a.x == b.x) return (a.y < b.y);
	return (a.x < b.x);
}

inline bool equiv(vector a, vector b)
{
	return (!comp(a, b) && !comp(b, a));
}


inline void test(vector a, vector b, vector c)
{
	// comp()
	{
		// a < a == false
		assert(comp(a, a) == false);
		assert(comp(b, b) == false);
		assert(comp(c, c) == false);

		// a < b == true => b < a == false
		if (comp(a, b) == true) 
			assert(comp(b, a) == false);

		// a < b && b < c => a < c
		if (comp(a, b) == true && comp(b, c) == true) 
			assert(comp(a, c) == true);
	}

	// equiv()
	{
		assert(equiv(a, a) == true);
		assert(equiv(b, b) == true);
		assert(equiv(c, c) == true);

		if (equiv(a, b) == true)
			assert(equiv(b, a) == true);

		if (equiv(a, b) == true && equiv(b, c) == true)
			assert(equiv(a, c) == true);
	}
}

int main()
{
	// 'Compare' concept test:
	{
		// a == b, b == c:
		{
			vector a = { 1.0, 1.0 };
			vector b = a;
			vector c = b;
			test(a, b, c);
		}

		// a < b, b < c (by x comparison):
		{
			vector a = { 1.0, 1.0 };
			vector b = { 2.0, 1.0 };
			vector c = { 3.0, 1.0 };
			test(a, b, c);
		}

		// a < b, b < c (by y comparison):
		{
			vector a = { 0.0, 1.0 };
			vector b = { 0.0, 2.0 };
			vector c = { 0.0, 3.0 };
			test(a, b, c);
		}

		// a > b, b > c (by x comparison):
		{
			vector a = { 3.0, 0.0 };
			vector b = { 2.0, 0.0 };
			vector c = { 1.0, 0.0 };
			test(a, b, c);
		}

		// a > b, b > c (by y comparison):
		{
			vector a = { 5.0, 0.0 };
			vector b = { 5.0, 1.0 };
			vector c = { 5.0, 2.0 };
			test(a, b, c);
		}



		{
			vector a = { -11.2859, -10.6 };
			vector b = { -11.3, -10.5859 };
			vector c = { 5.0, 2.0 };
			test(a, b, c);
		}
	}

	return 0;
}