#include "indexable_impl.h"

indexable_impl::indexable_impl(int boundary) :
	m_boundary(boundary)
{
}

int indexable_impl::boundary()
{
	return m_boundary;
}
