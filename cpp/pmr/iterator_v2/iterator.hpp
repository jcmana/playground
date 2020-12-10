#pragma once

#include <type_traits>

#include "iterator_intf.hpp"

namespace pmr::iterator
{

template<typename T>
class iterator
{
public:
    using value_type = typename T::value_type;
    using pointer = typename T::value_type *;
    using reference = typename T::value_type &;

public:
    iterator(std::unique_ptr<T> up_iterator = nullptr) :
        m_up_iterator(std::move(up_iterator))
    {
    }

    iterator(const iterator & other) noexcept :
        m_up_iterator(
            static_cast<T *>(
                static_cast<const base_intf<value_type> &>(*other.m_up_iterator).copy().release()))
    {
        static_assert(std::is_base_of_v<base_intf<value_type>, T>, "missing 'base_intf' implementation");
    }

    iterator(iterator && other) noexcept :
        m_up_iterator(
            static_cast<T *>(
                static_cast<base_intf<value_type> &>(*other.m_up_iterator).move().release()))
    {
        static_assert(std::is_base_of_v<base_intf<value_type>, T>, "missing 'base_intf' implementation");
    }

    template<typename TF>
    friend bool operator ==(const iterator<TF> & lhs, const iterator<TF> & rhs);

    template<typename TF>
    friend bool operator !=(const iterator<TF> & lhs, const iterator<TF> & rhs);

    template<typename = std::enable_if_t<std::is_base_of_v<output_intf<value_type>, T>>>
    const typename T::value_type & operator  *() const
    {
        return static_cast<const output_intf<value_type> &>(*m_up_iterator).value();
    }

    template<typename = std::enable_if_t<std::is_base_of_v<input_intf<value_type>, T>>>
    typename T::value_type & operator  *()
    {
        return static_cast<input_intf<value_type> &>(*m_up_iterator).value();
    }

    template<typename = std::enable_if_t<std::is_base_of_v<forward_intf<value_type>, T>>>
    iterator & operator ++()
    {
        static_cast<forward_intf<value_type> &>(*m_up_iterator).increment();
        return (*this);
    }

    template<typename = std::enable_if_t<std::is_base_of_v<forward_intf<value_type>, T>>>
    iterator operator ++(int)
    {
        iterator copy = (*this);
        ++(*this);
        return copy;
    }

private:
    std::unique_ptr<T> m_up_iterator;
};

template<typename TF>
bool operator ==(const iterator<TF> & lhs, const iterator<TF> & rhs)
{
    const auto & lhs_base = static_cast<base_intf<TF::value_type> &>(*lhs.m_up_iterator);
    const auto & rhs_base = static_cast<base_intf<TF::value_type> &>(*rhs.m_up_iterator);

    return lhs_base.equals(rhs_base);
}

template<typename TF>
bool operator !=(const iterator<TF> & lhs, const iterator<TF> & rhs)
{
    return !(lhs == rhs);
}

}
