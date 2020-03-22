#pragma once

#include <cassert>
#include <memory>
#include <iterator>

#include "iterator.hpp"

namespace pmr {

/// \brief		Polymorphic forward iterator.
///
/// Functionality is hidden behind `facade` and this class exposes the well-known
/// forward iterator API. `facade` is runtime dependent and therefore conformant with
/// substitution principle.
template<typename T>
class forward_iterator :
	public iterator<T>
{
public:
	/// \brief		Forward iterator facade class.
	///
	/// Forward iterator facade is interface expected to implement actual
	/// iterator functionality, including necessary state.
	class facade :
		public iterator<T>::facade
	{
	public:
		/// \brief		Move to the next element.
		virtual void next() = 0;
	};

	// Standard library compliance aliases:
	using iterator_category = std::forward_iterator_tag;
	using value_type = T;
	using pointer = T *;
	using reference = T &;

public:
	/// \brief		Constructs empty (invalid) `forward_iterator`.
	forward_iterator() = default;
	/// \brief		Constructs `forward_iterator` from `facade`.
	forward_iterator(std::unique_ptr<facade> up_iterator);

	/// \brief		Increments the `forward_iterator`.
	forward_iterator & operator ++();
	/// \brief		Preincrements the `forward_iterator`.
	forward_iterator operator ++(int);

private:
	/// \brief		Forward iterator `facade`. Empty pointer means invalid iterator.
	std::unique_ptr<facade> m_up_facade;
};

#pragma region forward_iterator implementation:

template<typename T>
forward_iterator<T>::forward_iterator(std::unique_ptr<facade> up_iterator) :
	m_up_facade(std::move(up_iterator))
{
}

template<typename T>
forward_iterator<T> &
forward_iterator<T>::operator ++()
{
	m_up_facade->next();
	return (*this);
}

template<typename T>
forward_iterator<T> 
forward_iterator<T>::operator ++(int)
{
	auto that = (*this);
	m_up_facade->next();
	return that;
}

#pragma endregion

} // namespace pmr
