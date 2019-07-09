#include <iostream> 

constexpr long int fib(int n)
{
	return (n <= 1) ? n : fib(n - 1) + fib(n - 2);
}

struct constant;

struct constant
{
	constexpr constant(int v) :
		value(v)
	{
	}

	int value;
};

static constexpr constant c = 6;

int main()
{
	// value of res is computed at compile time
	const long int res = fib(10);
	std::cout << res;
	return 0;
}