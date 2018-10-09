#pragma once

#include <vector>
#include <utility>
#include <memory>

namespace {


template<typename A, typename B>
class bidirectional_map_base
{
public:
	explicit bidirectional_map_base(std::vector<std::pair<A, B>> && definition)
	{
		m_definition = std::move(definition);
	}

	B atob(const A & a)
	{
		for (const auto & pair : m_definition)
		{
			if (pair.first == a)
			{
				return pair.second;
			}
		}
	}

	A btoa(const B & b)
	{
		for (const auto & pair : m_definition)
		{
			if (pair.second == b)
			{
				return pair.first;
			}
		}
	}

private:
	std::vector<std::pair<A, B>> m_definition;
};


} // namespace


namespace generic {


/// <summary>bidirectional_map specialization for conversion between two different types.</summary>
template<typename A, typename B, typename Different = void>
class bidirectional_map :
	public bidirectional_map_base<A, B>
{
public:
	explicit bidirectional_map(std::vector<std::pair<A, B>> && definition):
		bidirectional_map_base<A, B>(std::forward<std::vector<std::pair<A, B>>>(definition))
	{
	}

	B operator[](const A & a)
	{
		return atob(a);
	}

	A operator[](const B & b)
	{
		return btoa(b);
	}
};

/// <summary>bidirectional_map specialization for conversion between two identical types.</summary>
template<typename A, typename B>
class bidirectional_map<A, B, typename std::enable_if<std::is_same<A, B>::value>::type>:
	public bidirectional_map_base<A, B>
{
public:
	explicit bidirectional_map(std::vector<std::pair<A, B>> && definition):
		bidirectional_map_base<A, B>(std::forward<std::vector<std::pair<A, B>>>(definition))
	{
	}
};


} // namepsace generic