#include <iostream>
#include <memory>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

struct hierarchy_common
{
	int value_common = 0;
};

struct hierarchy_component_A
{
	int value_component_A = 6;
};

struct hierarchy_component_B
{
	int value_component_B = 60;
};

struct composite :
	hierarchy_common,
	hierarchy_component_A,
	hierarchy_component_B
{
};

int main()
{
	hierarchy_common * hierarchy_ptr = new composite;

	hierarchy_ptr->value_common = 1;

	delete hierarchy_ptr;

	_CrtDumpMemoryLeaks(); return 0;
}