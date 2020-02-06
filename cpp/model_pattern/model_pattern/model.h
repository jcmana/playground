#pragma once

#include <utility>

#include "model_observable.hpp"
#include "model_waitable.hpp"

#include "model_accessor.hpp"
#include "model_modifier.hpp"

template<typename T>
class model :
    public model_observable,
    public model_waitable
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
        m_lock(other.m_mutex),
        m_value(other.m_value)
    {
        m_lock = std::unique_lock<std::mutex>(m_mutex, std::defer_lock);
    }

    /// \brief      Move constructor.
    model(model && other) :
        m_lock(other.m_mutex),
        m_value(std::move(other.m_value))
    {
        m_lock = std::unique_lock<std::mutex>(m_mutex, std::defer_lock);
    }

    /// \brief      Copy assignment.
    model & operator  =(const model & other)
    {
        m_lock = std::unique_lock<std::mutex>(other.m_mutex);

        {
            m_value = other.m_value;
        }
        return (*this);
    }

    /// \brief      Move assignment.
    model & operator  =(model && other) noexcept
    {
        m_value = std::move(other.m_value);
        trigger();
        return (*this);
    }

    const T & value() const
    {
        return m_value;
    }

    model_accessor<T> accessor()
    {
        return model_accessor<T>(*this);
    }

    /// \brief      Model modification interface.
    model_modifier<T> modifier()
    {
        return model_modifier<T>(*this);
    }

private:
    void trigger()
    {
        model_observable::trigger();
        model_waitable::trigger();
    }

private:
    mutable std::unique_lock<std::mutex> m_lock;
    mutable std::mutex m_mutex;

    /// \brief      Modelled value.
    T m_value;
};

