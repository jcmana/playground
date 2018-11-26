#include <iostream>

#include "filter_test.h"
#include "proxy_test.h"
#include "graph_test.h"

int main()
{
	filter_test();
	proxy_test();
	graph_test();

	std::cout << "Press key to exit ..." << std::endl;
	std::getchar();
	
	return 0;
}