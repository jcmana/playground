#pragma once

#include "my_iterator_intf.hpp"

template<typename T>
class my_iterator_impl :
    public my_iterator_intf<T>
{
public:
    my_iterator_impl()
    {
    }

    my_iterator_impl(const T & value) noexcept :
        m_value(value)
    {
    }

public:
    virtual bool equals(const pmr::iterator::base_intf<T> & other) const override
    {
        return (dynamic_cast<const my_iterator_impl &>(other).m_value == m_value);
    }

    virtual std::unique_ptr<pmr::iterator::base_intf<T>> copy() const override
    {
        return std::make_unique<my_iterator_impl>(m_value);
    }

    virtual std::unique_ptr<pmr::iterator::base_intf<T>> move() override
    {
        auto copy = m_value;
        m_value = 0;
        return std::make_unique<my_iterator_impl>(std::move(copy));
    }

    virtual const T & const_value_reference() const override
    {
        return m_value;
    }

    virtual T & value_reference() override
    {
        return m_value;
    }

    virtual void increment() override
    {
        ++m_value;
    }

    virtual void decrement() override
    {
        --m_value;
    }

private:
    T m_value;
};
