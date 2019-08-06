#include <iostream>
#include <exception>

void func() try
{
	throw std::exception("wtf");
}
catch(const std::exception & e)
{
	std::cout << "caught std::exception: " << e.what() << std::endl;
}

int main()
{
	func();
}