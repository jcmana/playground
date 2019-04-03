#include <cstdint>

struct integer
{
	std::int64_t value;
};

integer operator +(integer left, integer right)
{
	return integer { left.value + right.value };
}

integer operator *(integer left, integer right)
{
	return integer{ left.value * right.value };
}

integer operator /(integer left, integer right)
{
	return integer{ left.value / right.value };
}

int main()
{
	// Overloaded operator precedence:
	auto res = integer{7} + integer{100} / integer{3} * integer{2};

	return 0;
}