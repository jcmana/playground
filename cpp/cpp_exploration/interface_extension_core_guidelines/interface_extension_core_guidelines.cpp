#include <iostream>

class a_intf
{
public:
	virtual void method_a() const = 0;
};

class a_impl :
	public virtual a_intf
{
public:
	virtual void method_a() const override
	{
		std::cout << "method_a()" << std::endl;
	}
};

class b_intf :
	public virtual a_intf
{
	virtual void method_b() const = 0;
};

class b_impl :
	public virtual b_intf,
	public a_impl
{
public:
	virtual void method_b() const override
	{
		std::cout << "method_b()" << std::endl;
	}
};


int main()
{
	return 0;
}