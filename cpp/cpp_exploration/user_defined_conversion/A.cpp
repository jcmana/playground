#include "B.h"
#include "A.h"

A::A(int value) :
	m_value(value)
{
}

A::operator B() const
{
	return B(m_value);
}
