#pragma once

#include "../../callback_store/callback_store/callback_store.hpp"

/// \brief      
class IObserver
{
public:
    virtual ~IObserver() = default;

    /// \brief      Called when model_observable object is modificed.
    virtual void OnModification()
    {
    }
};

class model_observable
{
public:
    /// \brief      Observer modifications of this object.
    callback_guard<IObserver> observe(IObserver & observer) const
    {
        return m_callback_store.subscribe(observer);
    }

    /// \brief      Trigger the modification notification.
    ///
    /// Observers receive a callback.
    void trigger() const
    {
        m_callback_store.invoke(&IObserver::OnModification);
    }

private:
    mutable callback_store<IObserver> m_callback_store;
};