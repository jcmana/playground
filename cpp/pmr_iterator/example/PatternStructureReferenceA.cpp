#include "PatternStructureReferenceA.h"

CPatternStructureReferenceA::CPatternStructureReferenceA(int width, int height) :
	m_width(width),
	m_height(height)
{
}

IPatternStructureReference::iterator CPatternStructureReferenceA::begin() const
{
	return iterator(std::make_unique<iterator_facade>(m_width, m_height, 0, 0));
}

IPatternStructureReference::iterator CPatternStructureReferenceA::end() const
{
	return iterator(std::make_unique<iterator_facade>(m_width, m_height));
}
