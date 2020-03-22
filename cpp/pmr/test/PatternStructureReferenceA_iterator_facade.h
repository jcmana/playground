#pragma once

#include "PatternStructureReferenceA.h"

class CPatternStructureReferenceA::iterator_facade :
	public pmr::forward_iterator<transformation>::facade
{
public:
	iterator_facade(int width, int height);
	iterator_facade(int width, int height, int curr_width, int curr_height);

public:
	virtual void next() override;
	virtual iterator::value_type & value() override;
	virtual bool equal(const pmr::iterator<transformation>::facade & other) const override;
	virtual std::unique_ptr<pmr::iterator<transformation>::facade> copy() const override;

private:
	const int m_width;
	const int m_height;
	int m_curr_width;
	int m_curr_height;

	transformation m_value;
};