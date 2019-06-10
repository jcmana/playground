#pragma once

#include "PatternStructureReferenceA.h"

class CPatternStructureReferenceA::iterator_facade :
	public iterator::facade
{
public:
	iterator_facade(int width, int height);
	iterator_facade(int width, int height, int curr_width, int curr_height);

public:
	virtual void next() override;
	virtual transformation & read() override;
	virtual bool equal(const iterator::facade & other) const override;
	virtual std::unique_ptr<iterator::facade> copy() const override;

private:
	const int m_width;
	const int m_height;
	int m_curr_width;
	int m_curr_height;

	transformation m_value;
};