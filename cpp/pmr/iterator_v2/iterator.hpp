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

    const typename T::value_type & operator  *() const
    {
        static_assert(std::is_base_of_v<base_intf<value_type>, T>, "missing 'outpu_intf' implementation");

        return static_cast<const output_intf<value_type> &>(*m_up_iterator).value();
    }

    typename T::value_type & operator  *()
    {
        static_assert(std::is_base_of_v<base_intf<value_type>, T>, "missing 'input_intf' implementation");

        return static_cast<input_intf<value_type> &>(*m_up_iterator).value();
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
