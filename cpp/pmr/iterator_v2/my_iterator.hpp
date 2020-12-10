#pragma once

#include "my_iterator_intf.hpp"

template<typename T>
class my_iterator :
    public my_iterator_intf<T>
{
public:
    my_iterator()
    {
    }

    my_iterator(const T & value) noexcept :
        m_value(value)
    {
    }

public:
    virtual bool equals(const pmr::iterator::base_intf<T> & other) const override
    {
        return (static_cast<const my_iterator &>(other).m_value == m_value);
    }

    virtual std::unique_ptr<pmr::iterator::base_intf<T>> copy() const override
    {
        return std::make_unique<my_iterator>(m_value);
    }

    virtual std::unique_ptr<pmr::iterator::base_intf<T>> move() override
    {
        auto copy = m_value;
        m_value = 0;
        return std::make_unique<my_iterator>(std::move(copy));
    }

    virtual const T & value() const override
    {
        return m_value;
    }

    virtual pmr::iterator::forward_intf<T> & increment() override
    {
        return (*this);
    }

private:
    T m_value;
};
