#pragma once

#include <vector>

#include "my_intf.h"

class my_impl : public my_intf
{
public:
    class iterator;

public:
    my_impl();

public:
    // my_intf implmentation:
    virtual pmr::iterator_v3::iterator<iterator_intf> begin() override;
    virtual pmr::iterator_v3::iterator<iterator_intf> end() override;

private:
    std::vector<int> m_items;
};

class my_impl::iterator : public my_intf::iterator_intf
{
public:
    iterator();
    iterator(std::vector<int>::iterator it);

public:
    // iterator_intf implementation:
    virtual bool equals(const base_intf& other) const override;
    
    virtual std::unique_ptr<base_intf> copy() const override;
    virtual std::unique_ptr<base_intf> move() override;
    virtual int & value_reference() override;
    virtual void increment() override;


private:
    std::vector<int>::iterator m_it;
};