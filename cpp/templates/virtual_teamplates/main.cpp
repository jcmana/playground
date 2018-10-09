#include <iostream>
#include <string>

template<typename T>
class base
{
public:
	base()
	{
		std::cout << "base<T>::base()" << std::endl;
	}

	//template<typename T>
	virtual void method()
	{
		std::cout << "base<T>::method()" << std::endl;
	}
};

int main()
{
	base<void> b;
	b.method();
	//b.method<void>();

	std::getchar();

	return 0;
}