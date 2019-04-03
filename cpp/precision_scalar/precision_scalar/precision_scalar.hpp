#pragma once

#include <cstdint>
#include <cmath>

constexpr std::int_fast64_t pow(std::int_fast64_t base, std::int_fast64_t exp)
{
	return  (exp == 1) ? (base) : (base * pow(base, exp - 1));
}

template<unsigned int Precision = 4>
class precision_scalar
{
public:
	precision_scalar() :
		value(0)
	{
	}

	explicit precision_scalar(long long int n) :
		value(n * precision_exponent)
	{
	}

	explicit precision_scalar(double n) :
		value(static_cast<long long int>(n * precision_exponent))
	{
	}

	explicit operator long long int()
	{
		return static_cast<long long int>(value / precision_exponent);
	}

	explicit operator double()
	{
		return static_cast<double>(value / precision_exponent);
	}

private:
	static const std::int_fast64_t precision_exponent;
	std::int_fast64_t value;

friend bool operator ==(const precision_scalar & left, const precision_scalar & right);
friend bool operator >(const precision_scalar & left, const precision_scalar & right);

friend precision_scalar add(const precision_scalar & left, const precision_scalar & right);
friend precision_scalar sub(const precision_scalar & left, const precision_scalar & right);
};

template<unsigned int Precision = 4>
const std::int_fast64_t precision_scalar<Precision>::precision_exponent = pow(10, Precision);

template<unsigned int Precision = 4>
bool 
operator ==(const precision_scalar<Precision> & left, const precision_scalar<Precision> & right)
{
	return (left.value == right.value);
}

/*
bool operator >(const precision_scalar & left, const precision_scalar & right)
{
	return (left.value > right.value);
}

precision_scalar add(const precision_scalar & left, const precision_scalar & right)
{
	precision_scalar result;
	result.value = (left.value + right.value);
	return result;
}

precision_scalar sub(const precision_scalar & left, const precision_scalar & right)
{
	precision_scalar result;
	result.value = (left.value - right.value);
	return result;
}

*/