#include <iostream>

#include "element_impl.h"

element_impl::element_impl(unsigned int type) :
	m_type(type)
{
}

unsigned int element_impl::type()
{
	return m_type;
}