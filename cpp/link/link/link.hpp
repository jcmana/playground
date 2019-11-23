#pragma once

#include <tuple>

#include "link_element.hpp"

std::tuple<link_element, link_element> make_link()
{
	link_element a;
	link_element b(&a);

	return std::forward_as_tuple(std::move(a), std::move(b));
}