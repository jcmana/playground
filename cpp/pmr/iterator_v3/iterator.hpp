#pragma once

#include <concepts>

#include "iterator_intf.hpp"

namespace pmr::iterator_v3
{

template<typename T>
class iterator 
{
public:
    iterator() :
        m_up_iterator()
    {
    }

    iterator(std::unique_ptr<T> up_iterator) :
        m_up_iterator(std::move(up_iterator))
    {
    }

	iterator(const iterator & other) :
		m_up_iterator()
	{
	}

	iterator(iterator && other) :
		m_up_iterator()
	{
	}

    const typename T::value_type & operator  *() const requires std::is_base_of_v<input_intf<typename T::value_type>, T>
    {
        return m_up_iterator->const_value_reference();
    }

    typename T::value_type & operator  *() requires std::is_base_of_v<output_intf<typename T::value_type>, T>
    {
        return m_up_iterator->value_reference();
    }

    const typename T::value_type * operator ->() const requires std::is_base_of_v<input_intf<typename T::value_type>, T>
    {
        return &m_up_iterator->const_value_reference();
    }

    typename T::value_type * operator ->() requires std::is_base_of_v<output_intf<typename T::value_type>, T>
    {
        return &m_up_iterator->value_reference();
    }

    iterator & operator ++() requires std::is_base_of_v<forward_intf<typename T::value_type>, T>
    {
        m_up_iterator->increment();
        return (*this);
    }

    iterator operator ++(int) requires std::is_base_of_v<forward_intf<typename T::value_type>, T>
    {
        auto copy = (*this);
        m_up_iterator->increment();
        return copy;
    }

    iterator & operator --() requires std::is_base_of_v<backward_intf<typename T::value_type>, T>
    {
        m_up_iterator->decrement();
        return (*this);
    }

	template<typename TF>
	friend bool operator ==(const iterator<TF> & lhs, const iterator<TF> & rhs);

	template<typename TF>
	friend bool operator !=(const iterator<TF> & lhs, const iterator<TF> & rhs);

private:
	std::unique_ptr<T> m_up_iterator;
};

template<typename T>
bool operator ==(const iterator<T> & lhs, const iterator<T> & rhs)
{
	const auto & lhs_base = static_cast<base_intf<typename T::value_type> &>(*lhs.m_up_iterator);
	const auto & rhs_base = static_cast<base_intf<typename T::value_type> &>(*rhs.m_up_iterator);

	return lhs_base.equals(rhs_base);
}

template<typename T>
bool operator !=(const iterator<T> & lhs, const iterator<T> & rhs)
{
	return !(lhs == rhs);
}

}
