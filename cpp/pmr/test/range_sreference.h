#pragma once

#include "../containers/range.hpp"
#include "../containers/forward_iterator.hpp"

using range_sreference_value = int;
using range_sreference_iterator = pmr::containers::forward_iterator<range_sreference_value>;
using range_sreference_iterator_facade = pmr::containers::forward_iterator<range_sreference_value>::facade;
using range_sreference_intf = pmr::containers::range<range_sreference_value, range_sreference_iterator>;

class range_sreference :
	public range_sreference_intf
{
public:
	range_sreference(int value);

public:
	virtual range_sreference_iterator begin() override;
	virtual range_sreference_iterator end() override;

private:
	class iterator;

private:
	int m_value;
};

#include "range_sreference_iterator.h"
