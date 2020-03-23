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

	template<typename TT>
	friend bool operator ==(const forward_iterator<T> & lhs, const forward_iterator<T> & rhs);

	template<typename TT>
	friend bool operator !=(const forward_iterator<T> & lhs, const forward_iterator<T> & rhs)
	{
		return !(lhs == rhs);
	}

	/// \brief		Increments the `forward_iterator`.
	forward_iterator & operator ++();
	/// \brief		Preincrements the `forward_iterator`.
	forward_iterator operator ++(int);
};

#pragma region forward_iterator implementation:

template<typename T>
forward_iterator<T>::forward_iterator(std::unique_ptr<facade> up_iterator) :
	iterator<T>(std::move(up_iterator))
{
}

template<typename TT>
bool operator ==(const forward_iterator<TT> & lhs, const forward_iterator<TT> & rhs)
{
	return (static_cast<const iterator<TT> &>(lhs) == static_cast<const iterator<TT> &>(rhs));
}

template<typename TT>
bool operator !=(const forward_iterator<TT> & lhs, const forward_iterator<TT> & rhs)
{
	return (static_cast<const iterator<TT> &>(lhs) != static_cast<const iterator<TT> &>(rhs));
}

template<typename T>
forward_iterator<T> &
forward_iterator<T>::operator ++()
{
	auto & f = static_cast<facade &>(*m_up_facade);
	f.next();
	return (*this);
}

template<typename T>
forward_iterator<T> 
forward_iterator<T>::operator ++(int)
{
	auto that = (*this);
	operator ++();
	return that;
}

#pragma endregion

} // namespace pmr
