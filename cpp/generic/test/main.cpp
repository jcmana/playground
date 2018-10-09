#include <iostream>
#include <string>

#include "generic/scope_guard.h"
#include "generic/bidirectional_map.h"

int main()
{
	/* generic::scope_guard example */

	auto g = generic::make_scope_guard([]
	{
		std::cout << "scope guard execution" << std::endl;
	});


	/* generic::bidirectional_map example */

	generic::bidirectional_map<int, std::string> m(
	{
		{ 0, "a" },
		{ 1, "b" },
		{ 2, "c" }
	});

	std::cout << m[0] << std::endl;
	std::cout << m["c"] << std::endl;

	//std::cout << m[13] << std::endl;					// read-access violation due to invalid index

	generic::bidirectional_map<std::string, std::string> n(
	{
		{ "hello", "ahoj" },
		{ "goodbye", "nashledanou" }
	});

	//std::cout << n["hello"] << std::endl;				// generic::bidirectional_map<std::string, std::string> does not define operator[]

	std::cout << n.atob("hello") << std::endl;
	std::cout << n.btoa("nashledanou") << std::endl;

	//std::cout << n.btoa("unknown") << std::endl;		// read-access violation due to invalid index

	return 0;
}