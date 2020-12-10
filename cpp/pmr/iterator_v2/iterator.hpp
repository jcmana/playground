#pragma once

#include "iterator_intf.hpp"

namespace pmr::iterator
{

template<typename T>
class iterator
{
public:
    iterator(std::unique_ptr<base_intf<T>> up_iterator = nullptr) :
        m_up_iterator(std::move(up_iterator))
    {
    }

    iterator(const iterator & other) noexcept :
        m_up_iterator(other.m_up_iterator->copy())
    {
    }

    iterator(iterator && other) noexcept :
        m_up_iterator(other.m_up_iterator->move())
    {
    }

    template<typename TF>
    friend bool operator ==(const iterator<TF> & lhs, const iterator<TF> & rhs);

    template<typename TF>
    friend bool operator !=(const iterator<TF> & lhs, const iterator<TF> & rhs);

private:
    std::unique_ptr<base_intf<T>> m_up_iterator;
};

template<typename TF>
bool operator ==(const iterator<TF> & lhs, const iterator<TF> & rhs)
{
    return (*lhs.m_up_iterator).equals(*rhs.m_up_iterator);
}

template<typename TF>
bool operator !=(const iterator<TF> & lhs, const iterator<TF> & rhs)
{
    return !(lhs == rhs);
}

}
