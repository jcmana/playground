#pragma once

#include "iterator.hpp"

namespace pmr {

template<typename T>
class random_access_iterator :
    public iterator<T>
{
public:
    class facade :
        public iterator<T>::facade
    {
    public:
        virtual void next(std::size_t n) = 0;
        virtual void prev(std::size_t n) = 0;
        virtual void at(std::ptrdiff_t index) = 0;
    };

public:
    random_access_iterator() = default;

    random_access_iterator(std::unique_ptr<facade> up_facade) :
        iterator<T>(std::move(up_facade))
    {
    }
    random_access_iterator(const random_access_iterator & other) :
        iterator<T>(other.m_up_facade->copy())
    {
    }

    random_access_iterator(random_access_iterator && other) = default;

    random_access_iterator operator  +(int n)
    {
        // Create a copy
        auto copy = (*this);

        // Retrieve facade implementation
        auto facade_ptr = static_cast<facade *>(copy.m_up_facade.get());

        if (facade_ptr == nullptr)
        {
            throw std::exception("facade pointer is nullptr");
        }

        // Increment the iterator
        facade_ptr->next(n);

        return copy;
    }

    random_access_iterator operator  -(int n)
    {
        // Create a copy
        auto copy = (*this);

        // Retrieve facade implementation
        auto facade_ptr = static_cast<facade *>(copy.m_up_facade.get());

        if (facade_ptr == nullptr)
        {
            throw std::exception("facade pointer is nullptr");
        }

        // Decrement the iterator
        facade_ptr->prev(n);

        return copy;
    }
};

} // namepsace pmr
