#pragma once

#include <set>
#include <utility>

using T = double;

/// \brief		Binary signal representation.
class binary_signal
{
public:
	/// \brief		Signal transition edge.
	enum
	{
		rise = +1,
		fall = -1,
	};

	/// \brief		Pulse (uniform period of signal holding `true`).
	struct pulse
	{
		T rise;
		T fall;
	};

	/// \brief		Edge type.
	using edge = std::pair<T, short int>;

public:
	/// \brief		Constructs signal with `false` everywhere.
	binary_signal() = default;

	/// \brief		Constructs pulse signal.
	binary_signal(T rise, T fall);


	/// \brief		Binary NOT.
	friend binary_signal operator  ~(const binary_signal & other);
	/// \brief		Binary AND.
	friend binary_signal operator  &(const binary_signal & lhs, const binary_signal & rhs);
	/// \brief		Binary OR.
	friend binary_signal operator  |(const binary_signal & lhs, const binary_signal & rhs);

private:
	/// \brief		Three-way comparison relation of `lhs` operand to `rhs`.
	enum comparison
	{
		LT = -1,	// `lhs` operand is less-than `rhs`
		EQ =  0,	// `lhs` operand is equal `rhs`
		GT = +1		// `lhs` operand is greater-than `rhs`
	};

	/// \brief		Comparator functor 
	struct less
	{
		bool operator ()(const edge & lhs, const edge & rhs);
	};

private:
	/// \brief		Compares two `edge`s.
	static comparison compare(const edge & lhs, const edge & rhs);

public:
	std::set<edge, less> m_edges;
};
