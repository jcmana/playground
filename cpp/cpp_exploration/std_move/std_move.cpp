#include <iostream>
#include <string>
#include <utility>

int main()
{
	std::string a("asdofmsadf");
	std::string b("vb2vb21vb21vb21");

	a = std::move(b);

	std::cout << "a: " << a << "\n";
	std::cout << "b: " << b << "\n";

	std::getchar(); return 0;
}