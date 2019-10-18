#include <iostream>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "tests.h"


int main()
{
	//filter_test();
	//proxy_test();
	graph_test();
	//limited_deque_test();
	//nonuniform_limited_queue_test();
	//cursor_test();

	std::cout << "Press key to exit ..." << std::endl;
	std::getchar();
	
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}