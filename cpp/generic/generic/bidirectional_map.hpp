#pragma once

#include <type_traits>

#include "bidirectional_map_base.hpp"

namespace generic {

/// \brief		Bidirectional map of values of different types.
template<typename A, typename B, typename Different = void>
class bidirectional_map :
	public detail::bidirectional_map_base<A, B>
{
public:
	using detail::bidirectional_map_base<A, B>::bidirectional_map_base;

	/// \brief		Mapped value for `a`.
	const B & operator [](const A & a) const;

	/// \brief		Mapped value for `b`.
	const A & operator [](const B & b) const;
};

/// \brief		Bidirectional map of values of the same type.
template<typename A, typename B>
class bidirectional_map<A, B, typename std::enable_if<std::is_same<A, B>::value>::type> :
	public detail::bidirectional_map_base<A, B>
{
public:
	using detail::bidirectional_map_base<A, B>::bidirectional_map_base;
};

#pragma region bidirectional_map implementation:

template<typename A, typename B, typename Different = void>
const B &
bidirectional_map<A, B, Different>::operator [](const A & a) const
{
	return atob(a);
}

template<typename A, typename B, typename Different = void>
const A & 
bidirectional_map<A, B, Different>::operator [](const B & b) const
{
	return btoa(b);
}

#pragma endregion

} // namepsace generic
