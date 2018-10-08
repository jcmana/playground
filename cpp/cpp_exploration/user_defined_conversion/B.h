#pragma once

class A;

class B
{
public:
	B(int value);

	explicit operator A() const;

private:
	int m_value;
};
