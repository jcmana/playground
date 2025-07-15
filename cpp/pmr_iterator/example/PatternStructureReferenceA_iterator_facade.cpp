#include "PatternStructureReferenceA_iterator_facade.h"

CPatternStructureReferenceA::iterator_facade::iterator_facade(int width, int height) :
	m_width(width),
	m_height(height),
	m_curr_width(width),
	m_curr_height(height + 1)
{
}

CPatternStructureReferenceA::iterator_facade::iterator_facade(int width, int height, int curr_width, int curr_height) :
	m_width(width),
	m_height(height),
	m_curr_width(curr_width),
	m_curr_height(curr_height)
{
}

void CPatternStructureReferenceA::iterator_facade::next()
{
	m_curr_width = (m_curr_width + 1);

	if (m_curr_width > m_width)
	{
		m_curr_width = 0;
		m_curr_height = (m_curr_height + 1);
	}
}

transformation & CPatternStructureReferenceA::iterator_facade::value()
{
	m_value = { m_curr_width + m_curr_height * m_height };

	return m_value;
}

bool CPatternStructureReferenceA::iterator_facade::equal(const pmr::iterator<transformation>::facade & other) const
{
	const iterator_facade & other_this = static_cast<const iterator_facade &>(other);

	return
		m_width == other_this.m_width &&
		m_height == other_this.m_height &&
		m_curr_width == other_this.m_curr_width &&
		m_curr_height == other_this.m_curr_height;
}

std::unique_ptr<pmr::iterator<transformation>::facade> CPatternStructureReferenceA::iterator_facade::copy() const
{
	return std::make_unique<iterator_facade>(m_width, m_height, m_curr_width, m_curr_height);
}
