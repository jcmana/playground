#include "PatternStructureReferenceA.h"

CPatternStructureReferenceA::CPatternStructureReferenceA(int width, int height) :
	m_width(width),
	m_height(height)
{
}

IPatternStructureReference::iterator CPatternStructureReferenceA::begin()
{
	auto up = std::make_unique<iterator_facade>(m_width, m_height, 0, 0);

	return iterator(std::move(std::make_unique<iterator_facade>(m_width, m_height, 0, 0)));
}

IPatternStructureReference::iterator CPatternStructureReferenceA::end()
{
	return iterator(std::make_unique<iterator_facade>(m_width, m_height));
}
