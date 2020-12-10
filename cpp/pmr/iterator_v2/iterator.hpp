#pragma once

#include <type_traits>

#include "iterator_intf.hpp"

namespace pmr::iterator
{

template<typename T>
class iterator
{
public:
    iterator(std::unique_ptr<T> up_iterator = nullptr) :
        m_up_iterator(std::move(up_iterator))
    {
    }

    iterator(const iterator & other) noexcept :
        m_up_iterator(
            static_cast<T *>(
                static_cast<const base_intf<T::value_type> &>(*other.m_up_iterator).copy().release()))
    {
    }

    iterator(iterator && other) noexcept :
        m_up_iterator(
            static_cast<T *>(
                static_cast<base_intf<T::value_type> &>(*other.m_up_iterator).move().release()))
    {
    }

    template<typename TF>
    friend bool operator ==(const iterator<TF> & lhs, const iterator<TF> & rhs);

    template<typename TF>
    friend bool operator !=(const iterator<TF> & lhs, const iterator<TF> & rhs);

    /*
    //template<typename = std::enable_if_t<std::is_base_of_v<base_intf<T>, output_intf<T>>>>
    const T & 
    operator  *() const
    {
        return dynamic_cast<const output_intf<T> &>(*m_up_iterator).value();
    }
    */

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
