#pragma once

#include <memory>

namespace pmr::iterator_v3
{

template<typename T>
struct base_intf
{
    using storage_type = std::unique_ptr<base_intf>;

    virtual ~base_intf() = default;

    virtual bool equals(const base_intf & other) const = 0;

    virtual storage_type copy() const = 0;
    virtual storage_type move() = 0;
};

template<typename T>
struct output_intf
{
    virtual const T & const_value_reference() const = 0;
};

template<typename T>
struct input_intf
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

}
