#pragma once

#include "iterator.hpp"
#include "iterator_intf.hpp"

class my_intf
{
public:
    class iterator_intf;

public:
    virtual ~my_intf() = default;

    virtual pmr::iterator_v3::iterator<iterator_intf> begin() = 0;
    virtual pmr::iterator_v3::iterator<iterator_intf> end() = 0;
};

class my_intf::iterator_intf :
    public pmr::iterator_v3::base_intf<int>,
    public pmr::iterator_v3::output_intf<int>,
    public pmr::iterator_v3::forward_intf<int>
{
public:
	using value_type = int;
	using pointer = value_type *;
	using reference = value_type &;
};
