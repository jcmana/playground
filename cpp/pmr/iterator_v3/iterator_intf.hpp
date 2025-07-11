#pragma once

#include <memory>

namespace pmr::iterator_v3
{

template<typename T>
struct base_intf
{
    using value_type = T;

    virtual ~base_intf() = default;

    virtual bool equal(const base_intf & other) const = 0;

    /// \returns        Owning raw pointer of a copy.
    virtual base_intf * copy() const = 0;
};

template<typename T>
struct input_intf
{
    virtual const T & const_value_reference() const = 0;
};

template<typename T>
struct output_intf
{
    virtual T & value_reference() = 0;
};

template<typename T>
struct forward_intf
{
    virtual void increment() = 0;
};

template<typename T>
struct backward_intf
{
    virtual void decrement() = 0;
};

template<typename T>
struct random_access_intf
{
    virtual void increment(std::size_t n) = 0;
    virtual void decrement(std::size_t n) = 0;

    virtual bool less(const base_intf<T> & other) const = 0;
    virtual bool more(const base_intf<T> & other) const = 0;
};

}
