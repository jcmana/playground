#include <iostream>
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
	result.m_edges.emplace(std::numeric_limits<T>::lowest(), binary_signal::rise);

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

	// Initialize the iteration:
	auto it_lhs = lhs.m_edges.begin();
	auto it_rhs = rhs.m_edges.begin();

	auto level_lhs = false;
	auto level_rhs = false;
	auto level = level_lhs && level_rhs;

	auto value = std::numeric_limits<T>::lowest();

	// Visit edges from both 'lhs' and 'rhs' while keeping the ordering:
	while (it_lhs != lhs.m_edges.end() && it_rhs != rhs.m_edges.end())
	{
		if (level_lhs && level_rhs != level)
		{
			level = !level;

			if (level)
			{
				result.m_edges.emplace(value, binary_signal::rise); 
			}
			else
			{
				result.m_edges.emplace(value, binary_signal::fall);
			}
		}

		switch (binary_signal::compare(*it_lhs, *it_rhs))
		{
			case binary_signal::LT:
			{
				level_lhs = !level_lhs;
				value = it_lhs->first;
				++it_lhs;

				break;
			}

			case binary_signal::EQ:
			{
				level_lhs = !level_lhs;
				value = it_lhs->first;
				++it_lhs;

				level_rhs = !level_rhs;
				value = it_rhs->first;
				++it_rhs;

				break;
			}

			case binary_signal::GT: 
			{
				level_rhs = !level_rhs;
				value = it_rhs->first;
				++it_rhs;

				break;
			}
		}
	}

	result.m_edges.emplace(value, binary_signal::fall);

	// Visit the remaining edges from either 'lhs' or 'lhs':
	// (only one of the following loops will actually be entered)

	/*
	while (it_lhs != lhs.m_edges.end())
	{
		level_lhs = !level_lhs;
		++it_lhs;
	}

	while (it_rhs != rhs.m_edges.end())
	{
		level_rhs = !level_rhs;
		++it_rhs;
	}
	*/

	return result;
}

bool binary_signal::less::operator ()(const edge & lhs, const edge & rhs)
{
	return (compare(lhs, rhs) == binary_signal::LT);
}

binary_signal::comparison binary_signal::compare(const edge & lhs, const edge & rhs)
{
	if (lhs.first < rhs.first)
	{
		return binary_signal::LT;
	}

	if (lhs.first > rhs.first)
	{
		return binary_signal::GT;
	}

	if (lhs.first == rhs.first)
	{
		if (lhs.second == rhs.second)
		{
			return binary_signal::EQ;
		}

		if (lhs.second == binary_signal::rise)
		{
			return binary_signal::LT;
		}

		if (rhs.second == binary_signal::rise)
		{
			return binary_signal::GT;
		}
	}
}
