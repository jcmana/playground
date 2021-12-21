#pragma once

#include "iterator.hpp"
#include "my_iterator_intf.hpp"
#include "my_iterator_impl.hpp"

template<typename T>
class my_iterator : virtual public pmr::iterator::base<T>,
    public pmr::iterator::output<T>,
    public pmr::iterator::input<T>,
    public pmr::iterator::forward<T>,
    public pmr::iterator::backward<T>
{
public:
    using pmr::iterator::base<T>::base;
    using pmr::iterator::input<T>::operator *;
    using pmr::iterator::output<T>::operator *;
};

template<typename T, template<typename> typename ... A>
struct iterator_intf :
    A<T>::intf_type ...
{
    using value_type = T;
    using pointer = T *;
    using reference = T &;
};

template<typename T, template<typename> typename ... A>
struct iterator : virtual pmr::iterator::base<iterator_intf<T>>,
    A<iterator_intf<T>> ...
{
public:
    using intf_type = iterator_intf<T, A ...>;
};