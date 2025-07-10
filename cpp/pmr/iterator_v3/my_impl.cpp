#include "my_impl.h"

void my_impl::begin()
{
    const auto it = iterator();
}

void my_impl::end()
{
    const auto it = iterator();
}


bool my_impl::iterator::equals(const base_intf & other) const
{
    return false;
}

my_impl::iterator::storage_type my_impl::iterator::copy() const
{
    return nullptr;
}

my_impl::iterator::storage_type my_impl::iterator::move()
{
    return nullptr;
}

const int & my_impl::iterator::const_value_reference() const
{
}

void my_impl::iterator::increment()
{
}