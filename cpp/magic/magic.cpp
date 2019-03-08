#include <iostream>

class A
{
public:
	void __cdecl print()
	{
		std::cout << "this = 0x" << std::hex << reinterpret_cast<unsigned int>(this) << std::endl;

		std::cout << "Ok.\n";
	}
};

int main()
{
	{
		int a[3] = { 10, 11, 12 };
		std::cout << 2[a] << '\n';
	}

	{
		A * a = nullptr; // Yes NULL!
		a->print(); // Oops, what is this?
	}

	return 0;
}
