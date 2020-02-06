#pragma once

#include <mutex>
#include <condition_variable>
#include <utility>

#include "../../callback_store/callback_store/callback_store.hpp"

#include "model_observer_intf.h"
#include "model_accessor.hpp"
#include "model_modifier.hpp"

template<typename T>
class model
{
public:
    template<typename TT>
    friend class model_accessor;

    template<typename TT>
    friend class model_modifier;

public:
    /// \brief      Default constructor.
    model() :
        m_lock(m_mutex, std::defer_lock),
        m_value(T())
    {
    }

    /// \brief      Copy constructor.
    model(const model & other) :
        m_value(other.m_value)
    {
    }

    /// \brief      Move constructor.
    model(model && other) :
        m_value(std::move(other.m_value))
    {
    }

    /// \brief      Copy assignment.
    model & operator  =(const model & other)
    {
        m_value = other.m_value;
        return (*this);
    }

    /// \brief      Move assignment.
    model & operator  =(model && other) noexcept
    {
        m_value = std::move(other.m_value);
        trigger();
        return (*this);
    }

    model_accessor<T> accessor()
    {
        // Lock the mutex, model_accessor extends the lock ownership
        m_mutex.lock();

        return model_accessor<T>(*this);
    }

    /// \brief      Model modification interface.
    model_modifier<T> modifier()
    {
        // Lock the mutex, model_accessor extends the lock ownership
        m_mutex.lock();

        return model_modifier<T>(*this);
    }

    /// \brief      Blocks until this object's modification.
    model_accessor<T> wait() const
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        // To cover spurious wakeup, trigger counter is user here to exactly match
        // single trigger() to a single notification of each waiting thread
        const auto initial_trigger = m_trigger;

        while (m_trigger == initial_trigger)
        {
            m_cv.wait(lock);
        }

        // Leave the mutex locked, `model_accessor` extends the lock ownership
        lock.release();

        return model_accessor<T>(*this);
    }

    /// \brief      Observer modifications of this object.
    callback_guard<model_observer_intf<T>> observe(model_observer_intf<T> & observer) const
    {
        return m_callback_store.subscribe(observer);
    }

private:
    /// \brief      Trigger the modification notification.
    ///
    /// First, all observers are notified. Second, waiting threads are woken up.
    void trigger()
    {
        m_mutex.lock();

        // Prepare accessor (therefore extending the locked mutex)
        auto accessor = model_accessor<T>(*this);

        // Notify observing threads
        m_callback_store.invoke(&model_observer_intf<T>::on_modification, accessor);

        // Notify waiting threads
        m_trigger++;
        m_cv.notify_all();
    }

private:
    mutable std::unique_lock<std::mutex> m_lock;
    mutable std::mutex m_mutex;
    mutable std::condition_variable m_cv;

    /// \brief      Represents the 'modification' condition.
    mutable std::size_t m_trigger;
    /// \brief      Registered observers.
    mutable callback_store<model_observer_intf<T>> m_callback_store;

    /// \brief      Modeled value.
    T m_value;
};

