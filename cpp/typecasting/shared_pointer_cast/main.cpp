#include <iostream>
#include <memory>
#include <string>

class base
{
public:
	virtual std::string method() = 0;
};

class base_impl :
	public base
{
public:
	virtual std::string method() override
	{
		return "base_impl::method()";
	}
};

class spec :
	public base
{
public:
	virtual void print() = 0;
};

class spec_impl :
	public spec
{
public:
	virtual std::string method() override
	{
		return "base_impl::method()";
	}

	virtual void print() override
	{
		std::cout << method() << std::endl;
	}
};

int main()
{
	std::shared_ptr<base> spBase = nullptr;

	// refcount = null (refcounting base not created yet)

	{
		std::shared_ptr<spec> spSpec = std::make_shared<spec_impl>();

		// refcount = 1 (spec_impl instance created and referenced from spSpec)

		spBase = std::static_pointer_cast<base>(spSpec);

		// refcount = 2 (spec_impl referenced from shared_ptr<base>)
	}

	// refcount = 1 (spSpec deleted, spBase still referencing)

	return 0;
}