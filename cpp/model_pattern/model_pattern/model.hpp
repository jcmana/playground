#pragma once

#include <mutex>
#include <condition_variable>
#include <utility>

#include "../../callback_store/callback_store/callback_store.hpp"

#include "model_observer_intf.h"
#include "model_accessor.hpp"
#include "model_modifier.hpp"

/// \brief          Synchronization primitive, allowing to observe or wait for value modifications.
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
    explicit model() :
        m_value(T())
    {
    }

    /// \brief      Constructor, creates model value by copying.
    explicit model(const T & other) :
        m_value(other)
    {
    }

    /// \brief      Constructor, creates model value by moving.
    explicit model(T && other) :
        m_value(std::move(other))
    {
    }

    /// \brief      Copy constructor, copies model value only.
    model(const model & other) :
        m_value(other.accessor().value())
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
        modifier().value() = other.accessor().value();
        return (*this);
    }

    /// \brief      Move assignment.
    model & operator  =(model && other) noexcept
    {
        // Reset current model value and notify
        modifier().value() = T();

        std::unique_lock<std::mutex> lock_other(other.m_mutex);
        std::unique_lock<std::mutex> lock(m_mutex);

        m_value = std::move(other.m_value);
        m_callback_store = std::move(other.m_callback_store);

        return (*this);
    }

    model_accessor<T> accessor() const
    {
        // Lock the mutex, model_accessor extends the lock ownership
        m_mutex.lock();

        return model_accessor<T>(*this);
    }

    /// \brief      Model modification interface.
    model_modifier<T> modifier()
    {
        // Lock the mutex, model_modifier extends the lock ownership
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
            __nop();
        }

        // Leave the mutex locked, model_accessor<T> extends the lock ownership
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
        // Notify observing threads:
        //
        // Observer callback is synchronized by default, because each is executed in modifying thread, meaning
        // using model_modifier<T> which is a critical section by itself
        {
            m_callback_store.invoke(&model_observer_intf<T>::on_modification, m_value);
        }

        // Notify waiting threads:
        //
        // Waiting threads are synchronized by extending critical section using model_accessor<T>, which is 
        // locked on the condition_variable::wait(), extended and then disposed in model_accessor<T>::dtor().
        {
            m_trigger++;
            m_cv.notify_all();
        }
    }

private:
    mutable std::mutex m_mutex;
    mutable std::condition_variable m_cv;

    /// \brief      Represents the 'modification' condition.
    mutable std::size_t m_trigger;
    /// \brief      Registered observers.
    mutable callback_store<model_observer_intf<T>> m_callback_store;

    /// \brief      Modeled value.
    T m_value;
};

