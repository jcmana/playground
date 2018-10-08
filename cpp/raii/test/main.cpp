#include "raii/raii_impl.h"

int main()
{
	auto r = raii_impl::factory().create_raii();

	return 0;
}