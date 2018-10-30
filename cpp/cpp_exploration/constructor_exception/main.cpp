#include <iostream>

class SomeType
{
	double val;
	int val2 = 4;
public:
	SomeType() : 
		val(3.2)
	{
		throw int(7);

		val2 = 100 + 3 + val;
	}

	~SomeType()
	{
		std::cout << "SomeType::~SomeType()" << std::endl << val2;
	}
};

int main()
{
	{
		SomeType t;
	}

	return 0;
}