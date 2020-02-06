#pragma once

class model_observer_intf
{
public:
    virtual ~model_observer_intf() = default;

    virtual void on_modification()
    {
    }
};