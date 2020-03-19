#pragma once

#include "iterator.hpp"

namespace pmr {
namespace containers {

template<typename T>
class random_access_iterator :
	public iterator<T>
{
public:
	class facade :
		public iterator<T>::facade
	{
	public:
		virtual T & advance(std::size_t index) = 0;
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

		// Advance the iterator
		auto facade_ptr = static_cast<facade *>(copy.m_up_facade.get());
		facade_ptr->advance(n);

		return copy;
	}
};

} // namespace containers
} // namepsace pmr
