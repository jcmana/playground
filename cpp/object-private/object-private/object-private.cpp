#include "stdafx.h"
#include <iostream>

class MyClass
{
	public:
		void call(MyClass o)
		{
			o.method();
		}

	private:
		void method()
		{
			std::cout << "MyClass::method()" << std::endl;
		}
};


int main()
{
	MyClass A;
	MyClass B;

	A.call(B);
	B.call(A);

	//A.method();
	//B.method();

    return 0;
}

