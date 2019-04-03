#pragma once

#include <cstddef>
#include <cstdint>
#include <cmath>

class fixed
{
public:
	static constexpr std::int64_t precision = 1'000;

public:
	explicit fixed() :
		value(0)
	{
	}

	explicit fixed(long long int n) :
		value(static_cast<std::int64_t>(n) * precision)
	{
	}

	explicit fixed(double n) :
		value(static_cast<std::int64_t>(std::round(n * static_cast<double>(precision))))
	{
	}

	operator double() const
	{
		return static_cast<double>(value) / static_cast<double>(precision);
	}

	operator long long int() const
	{
		return static_cast<long long int>(value / precision);
	}

private:
	std::int64_t value;

friend fixed add(fixed left, fixed right);
friend fixed sub(fixed left, fixed right);
friend fixed mul(fixed left, fixed right);
friend fixed div(fixed left, fixed right);
};

fixed add(fixed left, fixed right)
{
	fixed result;
	result.value = (left.value + right.value);
	return result;
}

fixed sub(fixed left, fixed right)
{
	fixed result;
	result.value = (left.value - right.value);
	return result;
}

fixed mul(fixed left, fixed right)
{
	fixed result;
	result.value = ((left.value * right.value) / fixed::precision);
	return result;
}

fixed div(fixed left, fixed right)
{
	fixed result;
	result.value = ((left.value / right.value) * fixed::precision);
	return result;
}