#include <atomic>


int main()
{
	std::atomic<long> a;

	a;

	a = 6;

	a;

	return 0;
}