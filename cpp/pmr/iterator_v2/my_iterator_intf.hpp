#pragma once

#include "iterator.hpp"

template<typename T>
class my_iterator_intf :
    public pmr::iterator::base_intf<T>,
    public pmr::iterator::output_intf<T>,
    public pmr::iterator::forward_intf<T>
{
public:
    using value_type = T;
    using pointer = T *;
    using reference = T &;
};
