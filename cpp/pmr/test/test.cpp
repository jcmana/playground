#include <iostream>
#include <memory>
#include <vector>

//#include "range_sreference.h"
#include "PatternStructureReferenceA.h"

int main()
{
/*
	std::unique_ptr<range_sreference_intf> up_container = std::make_unique<range_sreference>(7);

	for (auto val : *up_container)
	{
		std::cout << val << std::endl;
	}

	auto it = up_container->begin();
	std::cout << *it++ << std::endl;
*/

	std::unique_ptr<IPatternStructureReference> upStructureReference = std::make_unique<CPatternStructureReferenceA>(2, 3);

	for (const auto & transformation : (*upStructureReference))
	{
		std::cout << transformation.val << std::endl;
	}

	std::getchar(); return 0;
}