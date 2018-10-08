#include "indexable_impl.h"
#include "element_impl.h"

#include "rectangle_impl.h"

rectangle_impl::rectangle_impl(int x, int y, int width, int height) :
	m_x(x),
	m_y(y),
	m_width(width),
	m_height(height)
{
}

int rectangle_impl::width()
{
	return m_width;
}

int rectangle_impl::height()
{
	return m_height;
}
