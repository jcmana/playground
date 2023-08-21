#pragma once

#include <mutex>
#include <condition_variable>
#include <utility>
#include <functional>

#include "../../callback_store/callback_store/callback_store.hpp"
#include "../../callback_store/callback_store/callback_ref.hpp"

#include "model_observer_intf.h"
#include "model_accessor.hpp"
#include "model_modifier.hpp"
#include "model_guard.hpp"

/// \brief          Synchronization primitive, allowing to observe or wait for value modifications.
template<typename T>
class model
{
public:
    template<typename TT>
    friend class model_accessor;

    template<typename TT>
    friend class model_modifier;

    template<typename MM, typename TT>
    friend class model_guard;

    /// \brief      Modelled type.
    using type = T;

public:
    /// \brief      Default constructor.
    model();

    /// \brief      Copy constructor, copies model value only.
    model(const model & other);

    /// \brief      Move constructor, moves value only.
    model(model && other);

    /// \brief      Constructor, creates model value by copying.
    explicit model(const T & other);

    /// \brief      Constructor, creates model value by moving.
    explicit model(T && other);

    /// \brief      Copy assignment, copies model value only.
    model & operator  =(const model & other);

    /// \brief      Move assignment.
    model & operator  =(model && other) noexcept;

    /// \brief      Returns model access interface.
    model_accessor<T> accessor() const;

    /// \brief      Returns model modification interface.
    model_modifier<T> modifier();

    model_guard<model<T>, T> modifier_guard()
    {
        // Lock the mutex, model_accessor extends the lock ownership
        m_mutex.lock();

        return model_guard<model<T>, T>(*this);
    }

    model_guard<const model<T>, T> accessor_guard() const
    {
        // Lock the mutex, model_accessor extends the lock ownership
        m_mutex.lock();

        return model_guard<const model<T>, T>(*this);
    }

    /// \brief      Blocks until this object's modification.
    model_accessor<T> wait() const;

    /// \brief      Observer modifications of this object.
    callback_guard<std::function<void(T)>> observe(std::function<void(T)>) const;

private:
    /// \brief      Trigger the modification notification.
    ///
    /// First, all observers are notified. Second, waiting threads are woken up.
    void trigger();

private:
    mutable std::mutex m_mutex;
    mutable std::condition_variable m_cv;

    /// \brief      Represents the 'modification' condition.
    mutable std::size_t m_trigger;
    /// \brief      Registered observers.
    mutable callback_store<std::function<void(T)>> m_callback_store;

    /// \brief      Modelled value.
    T m_value;
};

#pragma region model implementation:

template<typename T>
model<T>::model() :
    m_value(T())
{
}

template<typename T>
model<T>::model(const T & other) :
    m_value(other)
{
}

template<typename T>
model<T>::model(T && other) :
    m_value(std::move(other))
{
}

template<typename T>
model<T>::model(const model & other) :
    m_value(other.accessor().value())
{
}

template<typename T>
model<T>::model(model && other) :
    m_value(std::move(other.m_value))
{
}

template<typename T>
model<T> &
model<T>::operator  =(const model & other)
{
    modifier().value() = other.accessor().value();
    return (*this);
}

template<typename T>
model<T> &
model<T>::operator  =(model && other) noexcept
{
    // Reset current model value and notify
    modifier().value() = T();

    std::unique_lock<std::mutex> lock_other(other.m_mutex);
    std::unique_lock<std::mutex> lock(m_mutex);

    m_value = std::move(other.m_value);
    m_callback_store = std::move(other.m_callback_store);

    return (*this);
}

template<typename T>
model_accessor<T>
model<T>::accessor() const
{
    // Lock the mutex, model_accessor extends the lock ownership
    m_mutex.lock();

    return model_accessor<T>(*this);
}

template<typename T>
model_modifier<T>
model<T>::modifier()
{
    // Lock the mutex, model_modifier extends the lock ownership
    m_mutex.lock();

    return model_modifier<T>(*this);
}

template<typename T>
model_accessor<T>
model<T>::wait() const
{
    std::unique_lock<std::mutex> lock(m_mutex);

    // To cover spurious wakeup, trigger counter is user here to exactly match
    // single trigger() to a single notification of each waiting thread
    const auto initial_trigger = m_trigger;

    while (m_trigger == initial_trigger)
    {
        m_cv.wait(lock);
    }

    // Leave the mutex locked, model_accessor<T> extends the lock ownership
    lock.release();

    return model_accessor<T>(*this);
}

template<typename T>
callback_guard<std::function<void(T)>> 
model<T>::observe(std::function<void(T)> callback) const
{
    return m_callback_store.subscribe(callback);
}

template<typename T>
void
model<T>::trigger()
{
    // Notify observing threads:
    //
    // Observer callback is synchronized by default, because each is executed in modifying thread, meaning
    // using model_modifier<T> which is a critical section by itself
    {
        m_callback_store.invoke(m_value);
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

#pragma endregion


