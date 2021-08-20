#pragma once

/// \brief      Callback interface, invoked when `model` is modified.
template<typename T>
class model_observer_intf
{
public:
    virtual ~model_observer_intf() = default;

    virtual void on_modification(const T & value)
    {
    }
};
