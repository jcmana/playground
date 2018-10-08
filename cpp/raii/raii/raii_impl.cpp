#include "raii_impl.h"

std::shared_ptr<raii_intf> raii_impl::factory::create_raii() const
{
	std::shared_ptr<raii_impl> sp_impl = std::make_shared<raii_impl>();
	return sp_impl;
}

raii_impl::raii_impl()
{
	m_pointer = new int();
}

raii_impl::~raii_impl()
{
	delete m_pointer;
}

void raii_impl::execute()
{
	std::cout << "raii_intf::execute() - Executing." << std::endl;
}