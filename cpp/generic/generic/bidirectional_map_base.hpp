#pragma once

#include <exception>
#include <vector>
#include <utility>

namespace generic {
namespace detail {

/// \brief		Base bidirectional map.
/// \note		Use specializations `bidirectional_map`, not directly this class.
template<typename A, typename B>
class bidirectional_map_base
{
public:
	/// \brief		Storage for the mapped pairs.
	using container = std::vector<std::pair<A, B>>;

public:
	explicit bidirectional_map_base(container && definition);
	
	/// \brief		Mapped value for `a`.
	/// \throws		`std::out_of_range` if the mapping doesn't exist.
	const B & atob(const A & a) const;
	
	/// \brief		Mapped value for `b`.
	/// \throws		`std::out_of_range` if the mapping doesn't exist.
	const A & btoa(const B & b) const;

	typename container::const_iterator begin() const;
	typename container::const_iterator end() const;

private:
	container m_definition;
};

#pragma region bidireactional_map_base implementation:

template<typename A, typename B>
bidirectional_map_base<A, B>::bidirectional_map_base(std::vector<std::pair<A, B>> && definition)
{
	m_definition = std::move(definition);
}

template<typename A, typename B>
const B & 
bidirectional_map_base<A, B>::atob(const A & a) const
{
	for (const auto & pair : m_definition)
	{
		if (pair.first == a)
		{
			return pair.second;
		}
	}

	throw std::out_of_range("Value mapping doesn't exist");
}

template<typename A, typename B>
const A &
bidirectional_map_base<A, B>::btoa(const B & b) const
{
	for (const auto & pair : m_definition)
	{
		if (pair.second == b)
		{
			return pair.first;
		}
	}

	throw std::out_of_range("Value mapping doesn't exist");
}

template<typename A, typename B>
typename bidirectional_map_base<A, B>::container::const_iterator
bidirectional_map_base<A, B>::begin() const
{
	return m_definition.cbegin();
}

template<typename A, typename B>
typename bidirectional_map_base<A, B>::container::const_iterator
bidirectional_map_base<A, B>::end() const
{
	return m_definition.cend();
}

#pragma endregion

} // namespace detail
} // namespace generic
