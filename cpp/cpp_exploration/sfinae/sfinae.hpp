#pragma once

#include <iostream>
#include <utility>
#include <tuple>

// Type SFINAE
// The following type errors are SFINAE errors.


// attempting to instantiate a pack expansion containing multiple packs of different lengths (since C++11)

template<typename ...outter_paramp>
struct outter
{
	template<typename ...inner_paramp>
	struct inner
	{
		using type = std::tuple<std::pair<outter_paramp, inner_paramp>...>;
	};
};


// attempting to create an array of void, array of reference, array of function, array of abstract class types,
// array of negative size, or array of size zero

template<typename T>
void type_array_fail()
{
	T a[10];
}

template<size_t N = 10>
void size_array_fail()
{
	char a[N];
}


// attempting to use a member of a type, where
// - the type does not contain the specified member
// - the specified member is not a type where a type is required
// - the specified member is not a template where a template is required
// - the specified member is not a non - type where a non - type is required

template<typename T>
struct member_fail
{
	typename T::iterator i;
};
