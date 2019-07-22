#include <iostream>

struct simple
{
	void method() const
	{
		std::cout << "A::method()" << std::endl;
	}
};

struct intf
{
	virtual void method() const = 0;
};

struct impl_a : intf
{
	virtual void method() const override
	{
		std::cout << "impl_a::method()" << std::endl;
	}
};

struct impl_b : intf
{
	virtual void method() const override
	{
		std::cout << "impl_b::method()" << std::endl;
	}
};

int main()
{
	// pointer to normal class method
	{
		simple a;

		//auto ptr = &simple::method;
		void(simple:: * ptr)() const = &simple::method;
		(a.*ptr)();
	}

	// pointer to polymorphic class method
	{
		intf * ia_ptr = new impl_a;
		intf * ib_ptr = new impl_b;

		void(intf:: * ptr)() const = &intf::method;
		(ia_ptr->*ptr)();
		(ib_ptr->*ptr)();

		delete ia_ptr;
		delete ib_ptr;
	}

	// WHAT IN THE ACTUAL FUCK?!

	return 0;
}