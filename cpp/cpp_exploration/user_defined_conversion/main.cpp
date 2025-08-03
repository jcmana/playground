#include "A.h"
#include "B.h"

template<typename T>
struct xy
{
	xy() = default;

	template<typename TT>
	xy(const xy<TT> & other)
	{
	}

	template<typename TT>
	operator xy<TT>() const
	{
		return {};
	}
};

struct X;
struct Y;

struct X
{
	X() = default;
	X(const Y & other)
	{
	}

	X & operator =(const Y & other);

	operator Y();
};

struct Y
{
	operator X();
};

X & X::operator =(const Y & other)
{
	return (*this);
}

X::operator Y()
{
	return {};
}

Y::operator X()
{
	return {};
}

int main()
{
	// Simple types:
	if (false)
	{
		A a(7);
		B b(14);

		A aa = static_cast<A>(b);	// explicit conversion (explicit operator A() const)
		B bb = a;		// implicit conversion (operator B() const)
	}

	// Templated types:
	if (false)
	{
		//xy<int> i = xy<double>();
	}

	if (true)
	{
		X x;
		Y y;

		x = y;

		auto function = [](X x)
		{
		};

		function(y);
	}

	return 0;
}
