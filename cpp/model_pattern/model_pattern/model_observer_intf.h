#pragma once

template<typename T>
class model_accessor;

template<typename T>
class model_observer_intf
{
public:
    virtual ~model_observer_intf() = default;

    virtual void on_modification(model_accessor<T> accessor)
    {
    }
};