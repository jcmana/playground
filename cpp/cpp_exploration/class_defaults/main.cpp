#include <stdexcept>

#include <exception>

class base
{
public:
	// forced default constructor
	base() = default;

	// user defined non-default constructor
	base(int x)
	{
	}

private:
	//const int m_integer;	// const members can't be default initialized
	//int & m_integer_ref;	// reference members can't be default initialized
};

int main()
{
	base b;
	base c(7);

	return 0;
}
