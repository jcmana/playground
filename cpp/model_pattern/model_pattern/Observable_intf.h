#pragma once

#include "../../callback_store/callback_store/callback_guard.hpp"

class IObserver
{
public:
    virtual ~IObserver() = default;

    /// \brief      Called when observable object is modificed.
    virtual void OnModified()
    {
    }
};

class IObservable
{
public:
    virtual ~IObservable() = default;

    /// \brief      Observer modifications of this object.
    virtual callback_guard<IObserver> Observe(IObserver & pObserver) = 0;
};