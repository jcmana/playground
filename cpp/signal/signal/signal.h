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

public:
	std::multiset<std::pair<T, short int>> m_edges;
};