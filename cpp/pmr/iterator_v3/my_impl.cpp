#include "my_impl.h"

my_impl::my_impl()
{
    m_items.push_back(7);
    m_items.push_back(2);
    m_items.push_back(42);
    m_items.push_back(15);
}

pmr::iterator_v3::iterator<my_intf::iterator_intf> my_impl::begin()
{
    return pmr::iterator_v3::iterator<iterator_intf>(
        std::make_unique<iterator>(
            m_items.begin()));
}

pmr::iterator_v3::iterator<my_intf::iterator_intf> my_impl::end()
{
    return pmr::iterator_v3::iterator<iterator_intf>(
        std::make_unique<iterator>(
            m_items.end()));
}

my_impl::iterator::iterator() :
    m_it()
{
}

my_impl::iterator::iterator(std::vector<int>::iterator it) :
    m_it(it)
{
}

bool my_impl::iterator::equal(const base_intf & other) const
{
    return (static_cast<const iterator &>(other).m_it == m_it);
}

my_impl::iterator::base_intf * my_impl::iterator::copy() const
{
    return new iterator(m_it);
}

int & my_impl::iterator::value_reference()
{
    return (*m_it);
}

void my_impl::iterator::increment()
{
    ++m_it;
}