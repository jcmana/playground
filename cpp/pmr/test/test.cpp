#include <iostream>
#include <memory>
#include <vector>

#include "PatternStructureReferenceA.h"

int main()
{
	std::unique_ptr<IPatternStructureReference> upStructureReference = std::make_unique<CPatternStructureReferenceA>(2, 3);

	for (const auto & transformation : (*upStructureReference))
	{
		std::cout << transformation.val << std::endl;
	}

	{
		pmr::containers::forward_iterator<int> a;
		pmr::containers::forward_iterator<int> b = std::move(a);
	}

	std::getchar(); return 0;
}