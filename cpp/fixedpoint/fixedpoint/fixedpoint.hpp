#pragma once

#include <cmath>

/// \brief			Represents fixed-point number: `Mantissa * Base ^ Exponent`.
template<typename Mantissa, int Base, int Exponent>
class fixedpoint
{
public:
	explicit fixedpoint(double value) :
		m_mantissa(value / std::pow(Base, Exponent))
	{
	}

	operator double() const
	{
		return (m_mantissa * std::pow(Base, Exponent));
	}

	template<typename Mantissa, int Base, int Exponent>
	friend 
	fixedpoint<Mantissa, Base, Exponent> 
	operator  +(fixedpoint<Mantissa, Base, Exponent> lhs, fixedpoint<Mantissa, Base, Exponent> rhs);

private:
	Mantissa m_mantissa;
};

template<typename Mantissa, int Base, int Exponent>
fixedpoint<Mantissa, Base, Exponent>
operator  +(fixedpoint<Mantissa, Base, Exponent> lhs, fixedpoint<Mantissa, Base, Exponent> rhs)
{
	return fixedpoint<Mantissa, Base, Exponent>(lhs.m_mantissa + rhs.m_mantissa);
}
