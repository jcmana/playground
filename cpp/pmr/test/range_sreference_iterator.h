#pragma once

#include "range_sreference.h"

class range_sreference::iterator :
	public range_sreference_iterator_facade
{
public:
	iterator();
	iterator(int * value_ptr);

public:
	virtual void next() override;
	virtual int & read() override;
	virtual bool equal(const range_sreference_iterator_facade & other) const override;

	virtual std::unique_ptr<range_sreference_iterator_facade> copy() const override;

private:
	int * m_value_ptr;
};