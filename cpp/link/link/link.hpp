#pragma once

#include <tuple>

#include "link_element.hpp"

/// \brief		Creates pair of linked elements.
auto make_link()
{
	link_element a;
	link_element b(&a);

	return std::make_tuple(std::move(a), std::move(b));
}