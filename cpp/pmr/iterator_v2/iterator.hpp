#pragma once

#include <type_traits>

#include "iterator_intf.hpp"

namespace pmr::iterator
{

template<typename T>
class base
{
public:
    using value_type = typename T::value_type;
    using pointer = typename T::value_type *;
    using reference = typename T::value_type &;

public:
    base()
    {
        void();
    }

    base(std::unique_ptr<T> up_iterator) :
        m_up_iterator(std::move(up_iterator))
    {
    }

    base(const base & other) noexcept :
        m_up_iterator(
            dynamic_cast<T *>((*other.m_up_iterator).copy().release()))
    {
    }

    base(base && other) noexcept :
        m_up_iterator(
            dynamic_cast<T *>((*other.m_up_iterator).move().release()))
    {
    }

    template<typename TF>
    friend bool operator ==(const base<TF> & lhs, const base<TF> & rhs);

    template<typename TF>
    friend bool operator !=(const base<TF> & lhs, const base<TF> & rhs);

protected:
    std::unique_ptr<T> m_up_iterator;
};


template<typename T>
class ouput :
    virtual protected base<T>
{
public:
    const typename T::value_type & operator  *() const
    {
        return static_cast<const output_intf<value_type> &>(*m_up_iterator).value();
    }
};

template<typename T>
class iterator_input :
    virtual protected base<T>
{
public:
    typename T::value_type & operator  *()
    {
        return static_cast<input_intf<value_type> &>(*m_up_iterator).value();
    }
};

template<typename T>
class forward :
    virtual protected base<T>
{
public:
    forward & operator ++()
    {
        static_cast<forward_intf<value_type> &>(*m_up_iterator).increment();
        return (*this);
    }

    forward operator ++(int)
    {
        auto copy = (*this);
        ++(*this);
        return copy;
    }
};

template<typename TF>
bool operator ==(const base<TF> & lhs, const base<TF> & rhs)
{
    const auto & lhs_base = static_cast<base_intf<TF::value_type> &>(*lhs.m_up_iterator);
    const auto & rhs_base = static_cast<base_intf<TF::value_type> &>(*rhs.m_up_iterator);

    return lhs_base.equals(rhs_base);
}

template<typename TF>
bool operator !=(const base<TF> & lhs, const base<TF> & rhs)
{
    return !(lhs == rhs);
}

}
