#pragma once

#include "iterator.hpp"
#include "my_iterator_intf.hpp"
#include "my_iterator_impl.hpp"

template<typename T>
class my_iterator :
    virtual public pmr::iterator::base<T>,
    virtual public pmr::iterator::output<T>,
    virtual public pmr::iterator::input<T>,
    virtual public pmr::iterator::forward<T>,
    virtual public pmr::iterator::backward<T>
{
public:
    using pmr::iterator::base<T>::base;
    using pmr::iterator::input<T>::operator *;
    using pmr::iterator::output<T>::operator *;
};


template<typename T, template<typename X = T> typename ... A>
class iterator_intf :
    virtual public pmr::iterator::base_intf<T>,
    virtual public A ...
{
public:
    using pmr::iterator::base<T>::base;
};

template<typename T, template<typename X = T> typename ... A>
class iterator :
    virtual public pmr::iterator::base<T>,
    virtual public A ...
{
public:
    using pmr::iterator::base<T>::base;
};