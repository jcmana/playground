#include <iostream>
#include <string>

template<typename T>
class test
{
public:
	void method(const T & input);
};

template<typename T>
void
test<T>::method(const T & input)
{
	std::cout << input << std::endl;
}

int main()
{
	test<std::string> t;

	t.method("nothing");

	return 0;
}