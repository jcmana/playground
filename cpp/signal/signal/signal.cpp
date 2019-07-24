#include <algorithm>
#include <limits>

#include "signal.h"

binary_signal::binary_signal(T rise, T fall)
{
	m_edges.emplace(rise, binary_signal::rise);
	m_edges.emplace(fall, binary_signal::fall);
}

binary_signal operator  ~(const binary_signal & other)
{
	binary_signal result;

	// Rising edge in negative infinity
	result.m_edges.emplace(std::numeric_limits<T>::min(), binary_signal::rise);

	for (const auto & edge : other.m_edges)
	{
		if (edge.second == binary_signal::rise)
		{
			result.m_edges.emplace(edge.first, binary_signal::fall);
			continue;
		}

		if (edge.second == binary_signal::fall)
		{
			result.m_edges.emplace(edge.first, binary_signal::rise);
			continue;
		}
	}

	// Falling edge in positive infinity
	result.m_edges.emplace(std::numeric_limits<T>::max(), binary_signal::fall);

	return result;
}

binary_signal operator  &(const binary_signal & lhs, const binary_signal & rhs)
{
	binary_signal result;

	if (lhs.m_edges.empty() || rhs.m_edges.empty())
	{
		return result;
	}

	auto it_lhs = lhs.m_edges.begin();
	auto it_rhs = rhs.m_edges.begin();

	// Copy edges from both 'lhs' and 'rhs' while keeping the ordering:

	while (it_lhs != lhs.m_edges.end() && it_rhs != rhs.m_edges.end())
	{
		if (it_lhs->first <= it_rhs->first)
		{
			result.m_edges.insert(*it_lhs);
			++it_lhs;
		}
		else
		{
			result.m_edges.insert(*it_rhs);
			++it_rhs;
		}
	}

	// Copy the remaining edges from either 'lhs' or 'lhs':
	// (only one of the following loops will actually be entered)

	while (it_lhs != lhs.m_edges.end())
	{
		result.m_edges.insert(*it_lhs);
		++it_lhs;
	}

	while (it_rhs != rhs.m_edges.end())
	{
		result.m_edges.insert(*it_rhs);
		++it_rhs;
	}

	return result;
}