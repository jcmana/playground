#include "range_sreference.h"

#include <memory>

range_sreference::range_sreference(int value) :
	m_value(value)
{
}

range_sreference_iterator range_sreference::begin()
{
	return range_sreference_iterator(std::move(std::make_unique<iterator>(&m_value)));
}

range_sreference_iterator range_sreference::end()
{
	return range_sreference_iterator(std::move(std::make_unique<iterator>()));
}
