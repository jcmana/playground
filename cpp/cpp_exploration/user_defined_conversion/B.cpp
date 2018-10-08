#include "A.h"
#include "B.h"

B::B(int value) :
	m_value(value)
{
}

B::operator A() const
{
	return A(m_value);
}