#pragma once

#include "PatternStructureReference_intf.h"

class CPatternStructureReferenceA :
	public IPatternStructureReference
{
public:
	CPatternStructureReferenceA(int width, int height);

public:
	// IPatternStructureReference implementation:
	virtual iterator begin() const override;
	virtual iterator end() const override;

private:
	class iterator_facade;

private:
	int m_width;
	int m_height;
};

#include "PatternStructureReferenceA_iterator_facade.h"
