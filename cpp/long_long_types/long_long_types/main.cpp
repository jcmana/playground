#include <cstddef>

int main(void)
{
	int a;
	long int b;
	long long int c;

	std::size_t size;
	size = sizeof(a);
	size = sizeof(b);
	size = sizeof(c);

	return 0;
}