#include <iostream>
#include <type_traits>

template<typename T, class Enable = void>
class customizable
{
public:
	void normal()
	{
		std::cout << "A::normal()" << std::endl;
	}
};

template<typename T>
class customizable<T, typename std::enable_if<std::is_same<T, bool>::value>::type>
{
public:
	void normal()
	{
		std::cout << "A::normal()" << std::endl;
	}

	void complex()
	{
		std::cout << "A::complex()" << std::endl;
	}
};


int main()
{
	//customizable<false> a;
	//customizable<true> b;
	customizable<void> a;
	customizable<bool> b;

	a.normal();
	a.complex();

	b.normal();
	b.complex();

	return 0;
}