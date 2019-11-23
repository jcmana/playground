#include <string>

#include "../link/link.hpp"

int main()
{
	link_element a;
	link_element b;

	std::tie(a, b) = make_link();

	return 0;
}