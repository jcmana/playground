#pragma once

//#include "B.hpp"

class B;

class A
{
public:
	A(int value);

	operator B() const;

private:
	int m_value;
};