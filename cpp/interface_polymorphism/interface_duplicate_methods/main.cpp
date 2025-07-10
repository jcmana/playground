#include <iostream>
#include <memory>

class ILeft
{
public:
	virtual void Method() = 0;
};

class IRight
{
public:
	virtual void Method() = 0;
};

class ISingular
{
public:
	virtual void Method() = 0;
};

class Impl :
	public ILeft,
	public IRight,
	public ISingular
{
public:
	/*
	virtual void ILeft::Method() override
	{
		std::cout << "ILeft::Method()" << std::endl;
	}

	virtual void IRight::Method() override
	{
		std::cout << "IRight::Method()" << std::endl;
	}
	*/

	virtual void Method() override
	{
		std::cout << "Impl::Method()" << std::endl;
	}
};

int main()
{
	const auto up = std::make_unique<Impl>();

	Impl & impl = (*up);
	ILeft & left = (*up);
	IRight & right = (*up);

	//impl.Method();
	left.Method();
	right.Method();

	return 0;
}