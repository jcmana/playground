#include <iostream>
#include <memory>

class intf
{
public:
	// if virtual destructor is missing, impl::~impl() won't be called
	virtual ~intf()
	{
	}

	virtual void method() = 0;
};

class impl :
	public intf
{
public:
	impl()
	{
		std::cout << "impl::impl()" << std::endl;
	}

	~impl()
	{
		std::cout << "impl::~impl()" << std::endl;
	}

	virtual void method() override
	{
		std::cout << "impl::method()" << std::endl;
	}
};

int main()
{
	std::unique_ptr<intf> up = std::make_unique<impl>();

	return 0;
}