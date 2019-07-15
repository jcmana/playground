#pragma once

class rectangle_base
{
public:
	rectangle_base(int x, int y, int width, int height) :
		m_x(x),
		m_y(y),
		m_width(width),
		m_height(height)
	{
	}

	int width()
	{
		return m_width;
	}

	int height()
	{
		return m_height;
	}

private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
};
