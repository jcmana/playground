#include <iostream>
#include <string>

#include "../bidirectional_conversion_table/bidirectional_conversion_table.h"

int main()
{
	int a;
	std::string b;

	bidirectional_conversion_table<int, std::string> ct(
	{
		{ 1, "none" },
		{ 2, "http" },
		{ 3, "socks4" }
	});

	a = ct.convert("http");
	b = ct.convert(a);

	std::cout << a << " <=> " << b << std::endl;

	b = ct[3];
	a = ct[b];

	std::cout << a << " <=> " << b << std::endl;

	return 0;
}
