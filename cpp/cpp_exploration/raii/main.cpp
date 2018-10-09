#include "../raii/raii_impl.h"

class global_factory :
	public raii_impl::factory
{
public:
	std::shared_ptr<raii_intf> create_raii()
	{
		return raii_impl::factory::create_raii();
	};
};


int main()
{
	global_factory f;

	auto r = f.create_raii();

	return 0;
}