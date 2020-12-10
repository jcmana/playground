#pragma once

#include "iterator.hpp"

template<typename T>
class my_iterator_intf :
    public pmr::iterator::output_intf<T>,
    public pmr::iterator::input_intf<T>,
    public pmr::iterator::forward_intf<T>,
    public pmr::iterator::backward_intf<T>
{
public:
    using value_type = T;
    using pointer = T *;
    using reference = T &;

};
