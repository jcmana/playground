#pragma once

#include "rectangle.h"

class rectangle_impl :
	public rectangle
{
public:
	rectangle_impl(int x, int y, int width, int height);

public:
	virtual int width() override;
	virtual int height() override;

private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
};
