#pragma once

#include "iterator.hpp"
#include "my_iterator_intf.hpp"
#include "my_iterator_impl.hpp"

template<typename T>
class my_iterator :
    virtual public pmr::iterator::base<T>,
    virtual public pmr::iterator::ouput<T>,
    virtual public pmr::iterator::forward<T>
{
public:
    using pmr::iterator::base<T>::base;
};