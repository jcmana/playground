#include <cmath>

#include <iostream>

void test_math_duplicities()
{
	double a, b, c;

	a = std::pow(2.6, 2);
	b = std::pow(2.4, 2);
	c = 3.14 * std::sqrt(16);

	std::cout << a << std::endl;
	std::cout << b << std::endl;
	std::cout << c << std::endl;
}

class virtual_class
{
private:
	void empty_normal()
	{
	}

	virtual void empty_virtual()
	{
	}

public:
	void execute()
	{
		std::cout << "executing" << std::endl;

		//empty_normal();
		empty_virtual();
	}
};

void test_empty_virtual_method()
{
	virtual_class vc;
	vc.execute();
}

class unused_member_class
{
public:
	using type = int;

public:
	unused_member_class() :
		m_()
	{
	}

	type execute() const
	{
		return m_;
	}

	void modify(type value)
	{
		m_ = value;
	}

private:
	type m_;
	type m_unused;

	std::string m_padding;
};


void test_unused_member()
{
	volatile unused_member_class c;
//	c.modify(6);
//	volatile auto x = c.execute();
//	c.modify(x + 12);
//	volatile auto y = c.execute();
}

int main()
{
	//test_math_duplicities();
	//test_empty_virtual_method();
	//test_unused_member();
	volatile unused_member_class c;

	constexpr auto s = sizeof(unused_member_class);

	return 0;
}