#include <iostream>
#include <utility>

template<class T>
int f(T && x) 
{			
	// x is a forwarding reference and so can be forwarded
	return g(std::forward<T>(x));
}

template<class T>
int g(T && x)					
{
	return x;
}

int main()
{
	int i;

	// argument is lvalue, calls f<int&>(int&), std::forward<int&>(x) is lvalue
	f(i);
	// argument is rvalue, calls f<int>(int&&), std::forward<int>(x) is rvalue
	f(0);

	return 0;
}