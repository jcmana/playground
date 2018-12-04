#include <iostream>
#include <list>

int main()
{
	std::list<int> biglist(10e6);		// 313MB memory usage

	unsigned int sum = 0;
	for (int n : biglist)
	{
		sum += n;
	}

	std::cout << sum << std::endl;

	std::getchar(); return 0;
}
