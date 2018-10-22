#include <iostream>
#include <string>

struct composite
{
	int & a;
	int & b;
};

void handle_composite(composite ref)
{
	ref.a += 7;
	ref.b += 7;
}

int main()
{
	int local_a = 14;
	int local_b = 3;

	composite x { local_a, local_b };

	std::cout << local_a << ", " << local_b << std::endl;
	handle_composite(x);
	std::cout << local_a << ", " << local_b << std::endl;

	std::getchar(); return 0;
}