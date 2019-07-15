#pragma once

class element_base
{
public:
	element_base(unsigned int t) :
		m_type(t)
	{
	}

	unsigned int type()
	{
		return m_type;
	}

private:
	unsigned int m_type;
};