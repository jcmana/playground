#pragma once

#include "../containers/forward_iterator.hpp"

struct transformation
{
	int val;
};

class IPatternStructureReference
{
public:
	using iterator = pmr::forward_iterator<transformation>;

public:
	virtual ~IPatternStructureReference() = default;

	virtual iterator begin() = 0;
	virtual iterator end() = 0;
};
