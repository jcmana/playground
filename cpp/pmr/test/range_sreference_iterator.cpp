#include "range_sreference_iterator.h"

range_sreference::iterator::iterator() :
	m_value_ptr(nullptr)
{
}

range_sreference::iterator::iterator(int * value_ptr) :
	m_value_ptr(value_ptr)
{
}

void range_sreference::iterator::next()
{
	m_value_ptr = nullptr;
}

int & range_sreference::iterator::read()
{
	return (*m_value_ptr);
}

bool range_sreference::iterator::equal(const range_sreference_iterator_facade & other) const
{
	// Cast back to the abstraction implementation:
	// (we can static_cast safely because this virtual method wouldn't be called if the implementations differs, I think, lol)
	const iterator & lhs = static_cast<const iterator &>(*this);
	const iterator & rhs = static_cast<const iterator &>(other);

	return (lhs.m_value_ptr == rhs.m_value_ptr);
}

std::unique_ptr<range_sreference_iterator_facade> range_sreference::iterator::copy() const
{
	return std::make_unique<iterator>(m_value_ptr);
}
