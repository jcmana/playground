#pragma once

#include "my_intf.h"

class my_impl : public my_intf
{
public:
    class iterator;

public:
    // my_intf implmentation:
    virtual void begin() override;
    virtual void end() override;
};

class my_impl::iterator : public my_intf::iterator_intf
{
public:
    virtual bool equals(const base_intf& other) const override;
    
    virtual storage_type copy() const override;
    virtual storage_type move() override;

    virtual const int & const_value_reference() const override;
    
    virtual void increment() override;
};