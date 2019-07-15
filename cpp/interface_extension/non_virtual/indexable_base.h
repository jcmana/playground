#pragma once

class indexable_base
{
public:
	indexable_base(int b) :
		m_boundary(b)
	{
	}

	int boundary()
	{
		return m_boundary;
	}

private:
	int m_boundary;
};