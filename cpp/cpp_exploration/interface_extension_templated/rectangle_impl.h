#pragma once

#include "element_impl.h"
#include "rectangle.h"

class rectangle_impl :
	public element_impl<rectangle>
{
public:
	rectangle_impl(int x, int y, int width, int height);
	rectangle_impl(const rectangle_impl & other) = default;
	rectangle_impl(rectangle_impl && other) noexcept = default;

public:
	virtual int width() const override;
	virtual int height() const override;

private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
};

rectangle_impl::rectangle_impl(int x, int y, int width, int height) :
	element_impl(0, 3),
	m_x(x),
	m_y(y),
	m_width(width),
	m_height(height)
{
}

int rectangle_impl::width() const
{
	return m_width;
}

int rectangle_impl::height() const
{
	return m_height;
}

