#pragma once

#include <memory>

namespace pmr::iterator
{

template<typename T>
struct base_intf
{
    virtual bool equals(const base_intf & other) const = 0;
    virtual std::unique_ptr<base_intf> copy() const = 0;
    virtual std::unique_ptr<base_intf> move() = 0;
};

template<typename T>
struct output_intf
{
    virtual const T & value() const = 0;
};

template<typename T>
struct input_intf
{
    virtual T & value() = 0;
};

template<typename T>
struct forward_intf
{
    virtual forward_intf & increment() = 0;
};

template<typename T>
struct backward_intf
{
    virtual backward_intf & decrement() = 0;
};

}
