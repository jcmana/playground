#include <iostream>
#include <utility>

template<class T>
int f(T && x) 			// x is a forwarding reference and so can be forwarded
{			
	return g(std::forward<T>(x));
}

template<class T>
int g(T && x)			// x is a forwarding reference and so can be forwarded			
{
	return x;
}

template<class T>
int h(const T && x)		// x is not a forwarding reference, because cv-qualification
{
	return x;
}

int add(int & a, int & b)
{
	const auto result = a + b;
	a = 0;
	b = 0;
	return result;
}

template<typename ... A>
int packing_references(A && ... args)
{
	return add(std::forward<A>(args) ...);
}

int main()
{
	int i;

	// argument is lvalue, calls f<int&>(int&), std::forward<int&>(x) is lvalue
	f(i);
	// argument is rvalue, calls f<int>(int&&), std::forward<int>(x) is rvalue
	f(0);

	int x = 7;
	auto y = std::make_unique<int>();
	*y = 42;
	packing_references(x, *y);

	return 0;
}