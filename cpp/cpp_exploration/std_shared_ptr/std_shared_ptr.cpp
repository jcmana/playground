#include <iostream>
#include <memory>

class intf
{
public:
	intf()
	{
		std::cout << "intf::intf()" << std::endl;
	}

	~intf()
	{
		std::cout << "intf::~intf()" << std::endl;
	}
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
};

int main()
{
	// manually building shared_ptr:
	{
		std::shared_ptr<intf> sp(new impl);
	}

	// using make:
	{
		std::shared_ptr<intf> sp = std::make_shared<impl>();
	}

	return 0;
}