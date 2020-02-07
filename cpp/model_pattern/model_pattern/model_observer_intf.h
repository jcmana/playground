#pragma once

#include <type_traits>

template<typename T>
class model_accessor;

template<typename T>
class model_observer_intf
{
public:
    virtual ~model_observer_intf() = default;

    virtual void on_modification(const T & value)
    {
    }
};