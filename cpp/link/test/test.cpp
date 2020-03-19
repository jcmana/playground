#include <iostream>
#include <string>

#include "../link/link.hpp"

int main()
{
	link_element a;
	link_element b;
	link_element c;

	std::tie(a, b) = make_link();
	std::tie(c, b) = make_link();

	std::cout << "a.is_linked() = " << a.is_linked() << std::endl;
	std::cout << "b.is_linked() = " << b.is_linked() << std::endl;
	std::cout << "c.is_linked() = " << c.is_linked() << std::endl;

	return 0;
}